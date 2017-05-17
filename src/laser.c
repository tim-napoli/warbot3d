#include "engine/display.h"
#include "math/matrix.h"
#include "laser.h"
#include "3d/material.h"
#include "3d/shader.h"

#include <math.h>
#include <GL/gl.h>

int      nlasers     = 0;
laser_t  lasers [32] = {{{0}}};

int             nlaser_impacts = 0;
laser_impact_t  laser_impacts [128] = {{{0}}};


int nlaser_sparks = 0;
laser_spark_t laser_sparks [128] = {{{{0}}}};

void laser_add (float sx, float sy, float sz, float dx, float dy, float dz, int r, int g, int b) {
    if (nlasers < 32) {
        lasers [nlasers].start.x = sx;
        lasers [nlasers].start.y = sy;
        lasers [nlasers].start.z = sz;
        
        lasers [nlasers].stop.x = dx;
        lasers [nlasers].stop.y = dy;
        lasers [nlasers].stop.z = dz;
        
        lasers [nlasers].r = r;
        lasers [nlasers].g = g;
        lasers [nlasers].b = b;
        
        lasers [nlasers].life = 5;
        
        nlasers++;
    }
    else {
        fprintf (stderr, "laser_add error : No space for more lasers.\n");
    }
}

void lasers_update () {
    int i;
    
    for (i = 0; i < nlasers; i++) {
        if (lasers [i].life == 0) {
            if (nlasers > 1) memcpy (&(lasers [i]), &(lasers [nlasers - 1]), sizeof (laser_t));
            nlasers--;
            i--;
        }
        else {
            lasers [i].life--;
        }
    }
}

void lasers_render () {
    int i;
    
    for (i = 0; i < nlasers; i++) {
        glColor4f (1.0, 1.0, 1.0, 1.0);
        glLineWidth (1.0f);
        glBegin (GL_LINES);
            glVertex3f (lasers [i].start.x, lasers [i].start.y, lasers [i].start.z);
            glVertex3f (lasers [i].stop.x, lasers [i].stop.y, lasers [i].stop.z);
        glEnd ();
    
        glColor4f (lasers [i].r / 255.0, lasers [i].g / 255.0, lasers [i].b / 255.0, 1.0);
        glLineWidth (5.0f);
        glBegin (GL_LINES);
            glVertex3f (lasers [i].start.x, lasers [i].start.y, lasers [i].start.z);
            glVertex3f (lasers [i].stop.x, lasers [i].stop.y, lasers [i].stop.z);
        glEnd ();
    }
    
}



void laser_impact_add (float x, float y, float z, float dx, float dz, int r, int g, int b) {
    int i;

    float l = sqrt (dx * dx + dz * dz);
    dx /= l;
    dz /= l;

    if (nlaser_impacts < 128) {
        laser_impact_t* li = &(laser_impacts [nlaser_impacts]);
        li->position.x = x - dx * 0.5; li->position.y = y; li->position.z = z - dz * 0.5; li->position.w = 0;
        
        vector_t axe = {0, 0, 0, 0};    axe.x = dz; axe.z = -dx;
        vector_normalize (&axe);
        
        matrix_t R;
        matrix_init_rotation (&R, &axe, 20 * 3.14159265 / 180);
        
        li->points [0].x = dx * 0.3; li->points [0].y = 0; li->points [0].z = dz * 0.3; li->points [0].w = 0;
        for (i = 1; i < 17; i++) {
            matrix_apply (&R, &(li->points [i - 1]), &(li->points [i]));
        }
                
        vector_scalar_mul (&axe, 0.01f);
        vector_add (&li->position, &axe);
        /*
        for (i = 0; i < 17; i++) {
            vector_add (&li->points [i], &li->position);
        }
        */
        
        li->life = 100;
        li->r = r;
        li->g = g;
        li->b = b;
        
        nlaser_impacts++;
    }
    else {
        fprintf (stderr, "laser_impact_add error : No room for more laser impacts.\n");
    }
}

void laser_impacts_update () {
    int i;
    
    for (i = 0; i < nlaser_impacts; i++) {
        if (laser_impacts [i].life == 0) {
            if (nlaser_impacts > 1) memcpy (&(laser_impacts [i]), &(laser_impacts [nlaser_impacts - 1]), sizeof (laser_impact_t));
            nlaser_impacts--;
            i--;
        }
        else {
            laser_impacts [i].life--;
        }
        
    }   
}

void laser_impacts_render () {
    int i, j;
    
    float s;
    laser_impact_t* li;
    
    for (i = 0; i < nlaser_impacts; i++) {
        li =  &(laser_impacts [i]);
        s = (li->life / 100.0f);
        
        glPushMatrix ();
        
        glTranslatef (li->position.x, li->position.y, li->position.z);
        
        glColor4f (1, 1, 1, s);
        glBegin (GL_POLYGON);
        for (j = 0; j < 17; j++) {
            glVertex3f (li->points [j].x * 0.4, li->points [j].y * 0.4, li->points [j].z * 0.4);
        }
        glEnd ();
        
        glColor4f (li->r / 255.0, li->g / 255.0, li->b / 255.0, s);
        glBegin (GL_POLYGON);
        for (j = 0; j < 17; j++) {
            glVertex3f (li->points [j].x, li->points [j].y, li->points [j].z);
        }
        glEnd ();
        
        glPopMatrix ();
    }
    
    glColor3ub (255, 255, 255);
}


void laser_spark_init (laser_spark_t* spark, float x, float y, float z, float dx, float dz, int r, int g, int b) {
    // La direction (vitesse initiale) de l'étincelle est calculée aléatoirement comme une rotation de (dx, dz) de -80 à
    // 80 degrés.
    
    int i;

    vector_t    v_init = {dx, 0, dz, 0};
    vector_normalize (&v_init);
    vector_scalar_mul (&v_init, 0.3);
    vector_t    axe = {0, 1, 0, 0};
    matrix_t    R;
    float       angle;
    
    for (i = 0; i < 10; i++) {    
        spark->positions [i].x = x; spark->positions [i].y = y; spark->positions [i].z = z; spark->positions [i].w = 0;
        
        angle = (-80 + rand () % 160) * (3.14159265 / 180);
        matrix_init_rotation (&R, &axe, angle);
        matrix_apply (&R, &v_init, &(spark->speeds [i]));
        spark->speeds [i].y = -0.5 + (rand () % 100) * 0.01;
        if (rand () % 10 == 0) {
            vector_scalar_mul (&(spark->speeds [i]), 2.0);
            if (spark->speeds [i].y < 0) spark->speeds [i].y = -spark->speeds [i].y;
        }
    }
    
    spark->r = r; spark->g = g; spark->b = b;
    spark->life = 200;
}

void laser_spark_update (laser_spark_t* spark) {
    int i;

    for (i = 0; i < 10; i++) {
        vector_add (&spark->positions [i], &spark->speeds [i]);
        spark->speeds [i].y -= 0.05; // Gravité
        spark->speeds [i].x *= 0.9; // Ralentissement
        spark->speeds [i].z *= 0.9;
        spark->life--;
    }
}

void laser_spark_render (laser_spark_t* spark) {
    int i;
    
    shader_update_point_size (current_shader, 1.0);
    
    glColor3f (1, 1, 1);
    glBegin (GL_POINTS);
    for (i = 0; i < 10; i++) {
        glVertex3f (spark->positions [i].x, spark->positions [i].y, spark->positions [i].z);
    }
    glEnd ();
    
    shader_update_point_size (current_shader, 3.0);
    glColor3f (spark->r / 255.0, spark->g / 255.0, spark->b / 255.0);
    glBegin (GL_POINTS);
    for (i = 0; i < 10; i++) {
        glVertex3f (spark->positions [i].x, spark->positions [i].y, spark->positions [i].z);
    }
    glEnd ();
}


void laser_sparks_add (float x, float y, float z, float dx, float dz, int r, int g, int b) {
    if (nlaser_sparks < 128) {
        laser_spark_init (&(laser_sparks [nlaser_sparks]), x, y, z, dx, dz, r, g, b);
        nlaser_sparks++;
    }
    else {
        fprintf (stderr, "laser_sparks_add error : Cannot add a new laser spark.\n");
    }
}

void laser_sparks_update () {
    int i;
    
    for (i = 0; i < nlaser_sparks; i++) {
        laser_spark_update (&(laser_sparks [i]));
        if (laser_sparks [i].life == 0) {
            if (nlaser_sparks > 1) memcpy (&(laser_sparks [i]), &(laser_sparks [nlaser_sparks - 1]), sizeof (laser_spark_t));
            i--;
            nlaser_sparks--;
        }
    }
}

void laser_sparks_render () {
    int i;

    
    
    for (i = 0; i < nlaser_sparks; i++) {
        laser_spark_render (&(laser_sparks [i]));
    }

    glColor3ub (255, 255, 255);
}


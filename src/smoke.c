#include <GL/glew.h>
#include "smoke.h"
#include "engine/bitmap.h"
#include "math/vector.h"

#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLuint smoke_texture = 0;

size_t  n_smokes = 0;
smoke_t smokes [32];

int smoke_init_module () {
    bitmap_t* smoke_bmp = bitmap_load ("data/smoke.bmp");
    if (smoke_bmp == NULL) {
        fprintf (stderr, "smoke_init_module error : Cannot load bitmap 'data/smoke.bmp'\n");
        return 1;
    }
    
    smoke_texture = bitmap_to_texture (smoke_bmp);
    bitmap_delete (smoke_bmp);

    return 0;
}

void smoke_close_module () {
    glDeleteTextures (1, &smoke_texture);
}

void smoke_add (float x, float y, float z) {
    if (n_smokes >= 32) {
        fprintf (stderr, "smoke_add error : Cannot add more smoke.\n");
        return;
    }
    
    smokes [n_smokes].x = x;
    smokes [n_smokes].y = y;
    smokes [n_smokes].z = z;
    
    smokes [n_smokes].ticker = 0;
    smokes [n_smokes].current_frame = 0;
    
    n_smokes++;
}

void smokes_update () {
    size_t i;
    
    for (i = 0; i < n_smokes; i++) {
        smokes [i].ticker = (smokes [i].ticker + 1) % 1;
        if (smokes [i].ticker == 0) {
            smokes [i].y += 0.2;
            smokes [i].current_frame++;
            if (smokes [i].current_frame > SMOKE_TEX_SIZE) {
                memcpy (&smokes [i], &smokes [n_smokes - 1], sizeof (smoke_t));
                n_smokes--;
                i--;
            }
        }
    }
}

void smoke_render (smoke_t* smoke, float dx, float dz) {
    // On affiche la fumée toujours en face de la caméra.
    vector_t diff_v = {smoke->x - dx, 0, smoke->z - dz, 0};
    vector_t up = {0, 1, 0, 0};
    vector_t direction;
    
    vector_normalize (&diff_v);
    vector_cross_product (&direction, &diff_v, &up);
    
    float tx = SMOKE_FRAME_TO_TEX_X(smoke->current_frame) / 7.0;
    float ty = SMOKE_FRAME_TO_TEX_Y(smoke->current_frame) / 7.0;
    
    glColor4f (1, 1, 1, 1.0 - smoke->current_frame / (float) SMOKE_TEX_SIZE);
    
    glBegin (GL_QUADS);
        glTexCoord2f (tx, ty + 1.0 / 7.0);
        glVertex3f (smoke->x - direction.x, smoke->y - 1, smoke->z - direction.z);
    
        glTexCoord2f (tx + 1.0 / 7.0, ty + 1.0 / 7.0);
        glVertex3f (smoke->x + direction.x, smoke->y - 1, smoke->z + direction.z);
        
        glTexCoord2f (tx + 1.0 / 7.0, ty);
        glVertex3f (smoke->x + direction.x, smoke->y + 1, smoke->z + direction.z);
        
        glTexCoord2f (tx, ty);    
        glVertex3f (smoke->x - direction.x, smoke->y + 1, smoke->z - direction.z);
    glEnd ();
}

void smokes_render (float dx, float dz) {
    size_t i;
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, smoke_texture);
    glDisable (GL_LIGHTING);
    
    glColor3f (1, 1, 1);
    for (i = 0; i < n_smokes; i++) {
        smoke_render (&smokes [i], dx, dz);
    }
    
    glDisable (GL_BLEND);
    glEnable (GL_LIGHTING);
}


#include "scene/scene_rendering.h"
#include "engine/display.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include "3d/shader.h"
#include "engine/font.h"
#include "math/matrix.h"

int last_time = 0;
int n_frame = 0;
int last_fps = 0;

void hud_render () {
    glPushMatrix ();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix ();
    glLoadIdentity ();
    glOrtho (0, display_width, 0, display_height, 0, 1);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_LIGHTING);
    glDisable (GL_COLOR_MATERIAL);

    //glBindTexture (GL_TEXTURE_2D, background_texture);
    
    // Points des robots
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float font_size = display_height / 32;
    char string [64];
    sprintf (string, "%d", robots [0]->points);
    font_draw_string (font, string, font_size, font_size, font_size, font_size, 0, 0, 255);
    
    sprintf (string, "%d", robots [1]->points);
    font_draw_string (font, string, font_size, font_size + 2 * font_size, font_size, font_size, 255, 128, 0);
    
    sprintf (string, "%d", robots [2]->points);
    font_draw_string (font, string, font_size, font_size + 4 * font_size, font_size, font_size, 0, 255, 0);
    
    // Vie robot bleu (si mode contrôlé).
    float ratio;
    float x1, x2, x3;
    if (controlled_robot != NULL) {
        ratio = (controlled_robot->life - controlled_robot->damages) / (float) controlled_robot->life;
        x1 = font_size * 1;
        x2 = x1 + (font_size * 9) * ratio;
        x3 = font_size * 10;
    
        glDisable (GL_TEXTURE_2D);
        
        glColor4f (controlled_robot->light.diffuse [0], controlled_robot->light.diffuse [1], controlled_robot->light.diffuse [2], 0.5);
        glBegin (GL_QUADS);
            glVertex2f (x1, display_height - 2 * font_size);
            glVertex2f (x2, display_height - 2 * font_size);
            glVertex2f (x2 + font_size, display_height - font_size);
            glVertex2f (x1 + font_size, display_height - font_size);
        glEnd ();
        
        glColor4f (1.0, 0, 0, 0.5);
        glBegin (GL_QUADS);
            glVertex2f (x2, display_height - 2 * font_size);
            glVertex2f (x3, display_height - 2 * font_size);
            glVertex2f (x3 + font_size, display_height - font_size);
            glVertex2f (x2 + font_size, display_height - font_size);
        glEnd ();
        
        glColor3ub (255, 255, 255);
        glEnable (GL_TEXTURE_2D);
    }
    
    // Compteur de fps
    int current_time;
    current_time = glutGet (GLUT_ELAPSED_TIME);
    n_frame++;
    if (current_time - last_time >= 1000) {
        last_time = current_time;
        last_fps = n_frame;
        n_frame = 0;
    }
    
    sprintf (string, "%d fps", last_fps);
    font_draw_string (font, string, display_width - 7 * font_size, display_height - 2 * font_size, font_size, font_size, 200, 200, 200);
    
    glDisable (GL_BLEND);
    glEnable (GL_LIGHTING);    
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_COLOR_MATERIAL);
    
    
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix ();
}



void update_camera_behind_robot (void) {
    // Calcul de la position de la caméra derrière le robot dans la direction de dx, dz.
    matrix_t R;
    vector_t axe = {0, 1, 0, 0};
    //vector_t eye = {0, 8, -25, 0};
    vector_t eye = {1.2, 4.0, 0.1, 0};
    matrix_init_rotation (&R, &axe, controlled_robot->pelvis->angles [1] * 3.14159265 / 180);
    
    matrix_apply_ex (&R, &eye);
    vector_add (&eye, (vector_t*) controlled_robot->pelvis->position);
    vector_copy (&scene_camera.center, &eye);
    scene_camera.center.x += controlled_robot->dx;
    scene_camera.center.z += controlled_robot->dz;
    
    // Regarde si il existe un mur entre la caméra et le robot contrôlé.
    /*
    float inter_x, inter_z;
    if (level_has_intersection (&scene_level,
                                controlled_robot->pelvis->position [0], controlled_robot->pelvis->position [2],
                                eye.x, eye.z,
                                &inter_x, &inter_z)) {
        eye.x = inter_x + controlled_robot->dx;       
        eye.z = inter_z + controlled_robot->dz;
        eye.y -= 1.0;
    }
    */
    
    
    
    
    //vector_add (&scene_camera.center, (vector_t*) controlled_robot->pelvis->position);
    //scene_camera.center.x += controlled_robot->dx;
    //scene_camera.center.y += 3.0;
    //scene_camera.center.z += controlled_robot->dz;
    
    vector_copy (&scene_camera.eye, &eye);
    
    scene_camera.up.x = 0;
    scene_camera.up.y = 1;
    scene_camera.up.z = 0;
    scene_camera.up.w = 0;
    
    audio_update_listener (eye.x, eye.y, eye.z,
                           controlled_robot->pelvis->position [0] - eye.x, 0, controlled_robot->pelvis->position [2] - eye.z,
                           0, 1, 0);
}

void update_camera (void) {
    if (controlled_robot != NULL) {
        update_camera_behind_robot ();
    }
    else {
        audio_update_listener (scene_camera.eye.x, scene_camera.eye.y, scene_camera.eye.z,
                               (scene_camera.center.x - scene_camera.eye.x), 
                               (scene_camera.center.y - scene_camera.eye.y), 
                               (scene_camera.center.z - scene_camera.eye.z),
                               scene_camera.up.x, scene_camera.up.y, scene_camera.up.z);
    }
    
    camera_use (&scene_camera);
    
    /*
    printf ("%f %f %f\n", scene_camera.center.x - scene_camera.eye.x, 
                          scene_camera.center.y - scene_camera.eye.y,
                          scene_camera.center.z - scene_camera.eye.z);
    */
}




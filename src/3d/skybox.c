#include <GL/glew.h>
#include "engine/bitmap.h"
#include "3d/skybox.h"

#include <stdio.h>

int skybox_init (skybox_t* skybox, const char* sky_bmp) {
    bitmap_t* texture = bitmap_load (sky_bmp);
    if (sky_bmp == NULL) {
        fprintf (stderr, "skybox_init error : Cannot load bitmap %s\n", sky_bmp);
        return 1;
    }
    
    skybox->texture = bitmap_to_texture (texture);
    bitmap_delete (texture);
    return 0;
}

void skybox_deinit (skybox_t* skybox) {
    glDeleteTextures (1, &skybox->texture);
}

void skybox_render (skybox_t* skybox) {
    glDisable (GL_LIGHTING);
    glDisable (GL_COLOR_MATERIAL);

    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, skybox->texture);

    glColor3f (1, 1, 1);

    glBegin (GL_QUADS);
    
    // X-pos
    glTexCoord2f (0.0, 0.5);        glVertex3f (-501, -501,  -500);
    glTexCoord2f (0.33333, 0.5);    glVertex3f ( 501, -501,  -500);
    glTexCoord2f (0.33333, 1.0);    glVertex3f ( 501,  501,  -500);
    glTexCoord2f (0.0, 1.0);        glVertex3f (-501,  501,  -500);
    
    // X-neg
    glTexCoord2f (0.33333, 0.5);    glVertex3f (-501, 501,  500);
    glTexCoord2f (0.0, 0.5);        glVertex3f ( 501, 501,  500);
    glTexCoord2f (0.0, 0.0);        glVertex3f ( 501, -501,  500);
    glTexCoord2f (0.33333, 0.0);    glVertex3f (-501, -501,  500);
    
    // Y-pos
    glTexCoord2f (0.66666, 0.5);     glVertex3f (-501, 500, -501);
    glTexCoord2f (0.66666, 1.0);     glVertex3f ( 501, 500, -501);
    glTexCoord2f (0.33333, 1.0);     glVertex3f ( 501, 500,  501);
    glTexCoord2f (0.33333, 0.5);     glVertex3f (-501, 500,  501);
    
    // Z-pos
    glTexCoord2f (1.0, 1.0);        glVertex3f (-500, 501,  -501);
    glTexCoord2f (0.66666, 1.0);    glVertex3f (-500, 501,   501);
    glTexCoord2f (0.66666, 0.5);    glVertex3f (-500, -501,   501);
    glTexCoord2f (1.0, 0.5);        glVertex3f (-500, -501,  -501);
    
    // Z-neg
    glTexCoord2f (1.0, 0.5);        glVertex3f (500, 501,  501);
    glTexCoord2f (0.66666, 0.5);    glVertex3f (500, 501, -501);
    glTexCoord2f (0.66666, 0.0);    glVertex3f (500, -501, -501);
    glTexCoord2f (1.0, 0.0);        glVertex3f (500, -501,  501);
    
    glEnd ();
    
    glEnable (GL_LIGHTING);
    glEnable (GL_COLOR_MATERIAL);
}


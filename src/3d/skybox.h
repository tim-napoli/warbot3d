/* skybox.h
 *  Handling a skybox.
 */

#ifndef _skybox_h_
#define _skybox_h_

#include <GL/gl.h>

typedef struct skybox {
    GLuint texture;
} skybox_t;

// Initialize the skybox with the given bitmap.
// This bitmap must be a big rectangular texture containing six textures like this :
// +x|+y|+z
// -x|-y|-z
// Where x, y, z are the corresponding face of the skybox, positives or negatives (right, left, up, down, front, back).
// Each sub tetxure must have the same size.
int skybox_init (skybox_t* skybox, const char* sky_bmp);

void skybox_deinit (skybox_t* skybox);

void skybox_render (skybox_t* skybox);

#endif


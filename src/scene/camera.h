/* camera.h
 *  Managing a camera in the space.
 */

#ifndef _camera_h_
#define _camera_h_

#include "math/vector.h"

#include "scene/level.h"

// The camera structure. Ready for gluLookAt.
typedef struct camera {
    vector_t    eye, center, up;
    level_t*    level;              // The level the camera is in (for collisions).
    node_t*     node;
} camera_t;

// Initialize a camera with the specified level, eye, targeted point and orietation.
void camera_init (camera_t* camera, level_t* level,
                  float ex, float ey, float ez, 
                  float cx, float cy, float cz, 
                  float ux, float uy, float uz);

// Move the camera of the specified vector, relative to the direction of the camera.
void camera_move_of (camera_t* camera, float x, float y, float z);

// Rotate the camera of the specified angle (in radients) on y.
void camera_rotate_y (camera_t* camera, float ay);

// Rotate the camera of the specified angle (in radients) on x.
void camera_rotate_x (camera_t* camera, float ax);

// Simply like "gluLookAt"
void camera_use (camera_t* camera);



#endif


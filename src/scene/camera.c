#include "camera.h"
#include "math/matrix.h"

#include <math.h>

#include <GL/glu.h>

void camera_init (camera_t* camera, level_t* level,
                  float ex, float ey, float ez, 
                  float cx, float cy, float cz, 
                  float ux, float uy, float uz) {
    camera->eye.x = ex; 
    camera->eye.y = ey; 
    camera->eye.z = ez; 
    camera->eye.w = 0;
    
    camera->center.x = cx; 
    camera->center.y = cy; 
    camera->center.z = cz; 
    camera->center.w = 0;
    
    camera->up.x = ux; 
    camera->up.y = uy; 
    camera->up.z = uz; 
    camera->up.w = 0;
    
    camera->level = level;
    camera->node = level_node_at (camera->level, camera->eye.x, camera->eye.z);
}

static void __camera_update_node (camera_t* camera) {
    // Check only on connexions.
    node_t* nearest = camera->node;
    float dist_min, dist;
    float dx, dz;
    
    dx = camera->eye.x - nearest->x;
    dz = camera->eye.z - nearest->z;
    dist_min = dx * dx + dz * dz;
    
    size_t i;
    for (i = 0; i < camera->node->nlinks; i++) {
        dx = camera->eye.x - camera->node->links [i]->x;
        dz = camera->eye.z - camera->node->links [i]->z;
        dist = dx * dx + dz * dz;
        if (dist < dist_min) {
            nearest = camera->node->links [i];
            dist_min = dist;
        }
    }
    
    camera->node = nearest;
}

void camera_move_of (camera_t* camera, float x, float y, float z) {
    // Get the camera angle.
    float dx = camera->center.x - camera->eye.x;
    float dz = camera->center.z - camera->eye.z;
    float angle = atan2 (-dz, dx);
    
    // Rotate the vector of this angle.
    matrix_t R;
    vector_t axe = {0, 1, 0, 0};
    matrix_init_rotation (&R, &axe, angle);
    vector_t v_in = {x, y, z, 0}, v_out;
    matrix_apply (&R, &v_in, &v_out);
    
    // Looking for collision with a level wall.
    float col_x, col_y;
    wall_t* line;
    
    line = level_has_intersection (camera->level, camera->eye.x, camera->eye.z, 
                                                          camera->eye.x + v_out.x, 
                                                          camera->eye.z + v_out.z,
                                                          &col_x, &col_y);

    if (!line) {   
        vector_add (&camera->eye, &v_out);
        vector_add (&camera->center, &v_out);
    }
    
    
    node_t* node = level_node_at (camera->level, camera->eye.x, camera->eye.z);
    if (camera->eye.y > node->sector->height) {
        camera->eye.y = node->sector->height - 1;
    }
    
    __camera_update_node (camera);
}

void camera_rotate_y (camera_t* camera, float ay) {
    vector_t    axe = {0, 1, 0, 0};
    matrix_t    R;
    matrix_init_rotation (&R, &axe, ay);
    
    // Calcul du nouveau vecteur "center"
    vector_t v_in, v_out;
    vector_sub_ex (&v_in, &camera->center, &camera->eye);
    matrix_apply (&R, &v_in, &v_out);
    
    //printf ("%.2f %.2f %.2f %.2f -> %.2f %.2f %.2f %.2f\n", v_in.x, v_in.y, v_in.z, v_in.w, v_out.x, v_out.y, v_out.z, v_out.w);
    
    vector_copy (&camera->center, &camera->eye);
    vector_add (&camera->center, &v_out);
    
    // Calcul du nouveau vecteur "up"
    vector_copy (&v_in, &camera->up);
    matrix_apply (&R, &v_in, &camera->up);
}

void camera_rotate_x (camera_t* camera, float ax) {
    // d'abords, calcul l'angle y de la caméra.
    float angle = atan2 (-(camera->center.z - camera->eye.z), camera->center.x - camera->eye.x);
    vector_t    axe_y = {0, 1, 0, 0};
    matrix_t    Ry;
    matrix_init_rotation (&Ry, &axe_y, angle);
    
    // Applique cette matrice à (1, 0, 0).
    vector_t axe_x_in = {0, 0, 1, 0}, axe_x;
    matrix_apply (&Ry, &axe_x_in, &axe_x);
    
    // axe_x est l'axe de rotation à utiliser maintenant.
    matrix_t R;
    matrix_init_rotation (&R, &axe_x, ax);
    
    // Calcul du nouveau vecteur "center"
    vector_t v_in, v_out;
    vector_sub_ex (&v_in, &camera->center, &camera->eye);
    matrix_apply (&R, &v_in, &v_out);
    
    //printf ("%.2f %.2f %.2f %.2f -> %.2f %.2f %.2f %.2f\n", v_in.x, v_in.y, v_in.z, v_in.w, v_out.x, v_out.y, v_out.z, v_out.w);
    
    vector_copy (&camera->center, &camera->eye);
    vector_add (&camera->center, &v_out);
    
    // Calcul du nouveau vecteur "up"
    vector_copy (&v_in, &camera->up);
    matrix_apply (&R, &v_in, &camera->up);
}

void camera_use (camera_t* camera) {
    gluLookAt (camera->eye.x, camera->eye.y, camera->eye.z,
               camera->center.x, camera->center.y, camera->center.z,
               camera->up.x, camera->up.y, camera->up.z);
}


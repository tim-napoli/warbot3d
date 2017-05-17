/* Permits the animation of a vbo_mesh_t.
 * See 3d/rig.h for more details about rigging.
 *
 */

#ifndef _vbo_rig_h_
#define _vbo_rig_h_

#include "3d/mesh/vbo_mesh.h"

typedef struct vbo_rig {
    struct vbo_rig* parent;
    
    float position [4],
          scale [4],
          angles [4];
    
    vbo_mesh_t*         mesh;
    vbo_face_group_t*   group;
    
    size_t              nchilds;
    struct vbo_rig**    childs;
} vbo_rig_t;

vbo_rig_t* vbo_rig_new (vbo_rig_t* parent, 
                        float p [4], float s [4], float a [4], 
                        vbo_mesh_t* mesh, vbo_face_group_t* group, 
                        size_t nchilds);

void vbo_rig_delete (vbo_rig_t* rig);

void vbo_rig_set_child (vbo_rig_t* rig, size_t n, vbo_rig_t* child);

void vbo_rig_render (vbo_rig_t* rig);

// Load model to world matrix of this rig to the current openGL matrix.
void vbo_rig_get_model_matrix (vbo_rig_t* rig);

#endif


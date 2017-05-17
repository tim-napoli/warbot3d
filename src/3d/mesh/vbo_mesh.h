/* vbo_mesh.h
 *  A VBO mesh is a mesh stored in an OpenGL vertex buffer object.
 *  This is usefull to increase performances of a rendering.
 *  To animate a vbo_mesh, instead of using a rig_t, use a vbo_rig_t (3d/mesh/vbo_rig.h).
 */

#ifndef _vbo_mesh_h_
#define _vbo_mesh_h_

#include <GL/glew.h>
#include <GL/gl.h>

#include "3d/mesh.h"

// Store informations about a vbo face group.
// The group have an offset of 'offset' in the VBO and a size of 'size' vertices.
typedef struct vbo_face_group {
    char        name [32];
    GLintptr    offset;
    GLsizei     size;
    material_t* material;
} vbo_face_group_t;

// The vbo mesh structure.
typedef struct vbo_mesh {
    GLuint      data_vbo;
    GLintptr    offset_v, offset_vn, offset_vtc;
    GLsizei     data_size;
    GLuint      index_vbo;
    GLsizei     index_size;
    
    size_t              n_fg;
    vbo_face_group_t*   fg;
    
    material_t* material;
} vbo_mesh_t;

//--------------------------------------------------- allocation -----------------------------------------------------//
// Create a new vbo_mesh. Return NULL on error.
vbo_mesh_t* vbo_mesh_new (mesh_t* mesh);

// Release memory taken by a vbo mesh.
void vbo_mesh_delete (vbo_mesh_t* mesh);

//---------------------------------------------------- accessing -----------------------------------------------------//
// Return the face group named 'name' of a vbo mesh. Return NULL if not found.
vbo_face_group_t* vbo_mesh_get_face_group (vbo_mesh_t* vmesh, const char* name);


//---------------------------------------------------- rendering -----------------------------------------------------//
// Start the rendering of a face group. Must be called before any other rendering call for a face group.
void vbo_mesh_render_face_group_begin (vbo_mesh_t* vmesh);

// Render a face group.
void vbo_mesh_render_face_group (vbo_mesh_t* vmesh, vbo_face_group_t* vfg);

// Finish the rendering of a face group. This should be used before starting rendering of another group.
void vbo_mesh_render_face_group_end ();

// Render a whole vbo mesh.
void vbo_mesh_render (vbo_mesh_t* vmesh);

#endif


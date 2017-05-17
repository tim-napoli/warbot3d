/* mesh.h :
 *  Mesh management.
 */
 
#ifndef _mesh_h_
#define _mesh_h_

#include "3d/material.h"
#include <stdlib.h>

// A vertex is a point of the mesh.
typedef struct vertex {
    float x, y, z;
} vertex_t;

// A vertex normal.
typedef struct vertex_normal {
    float x, y, z;
} vertex_normal_t;

// A vertex texture coordinate.
typedef struct vertex_tex_coord {
    float u, v;
} vertex_tex_coord_t;

// To know what contains a face information.
enum {
    face_use_vertex_normal      = 0x1,
    face_use_vertex_tex_coord   = 0x2
};

// A face is three vertex of the mesh givin' a triangle.
typedef struct face {
    int     flags;
    size_t  v_id [3];
    size_t  vn_id [3];
    size_t  vtc_id [3];
} face_t;

// A face group is a set of face which are forming a coherent part of a model to animate it via a rig.
typedef struct face_group {
    char    name [32];
    size_t  offset;
    size_t  size;
    material_t* material;
} face_group_t;

// The mesh structure. 
// n_ : Number of.
//  v   : vertex
//  vn  : vertex normal
//  vtc : vertex tetxure coordinate
//  f   : face
//  fg  : face group
typedef struct mesh {
    size_t  n_v, n_vn, n_vtc, n_f, n_fg;
    vertex_t*           v;
    vertex_normal_t*    vn;
    vertex_tex_coord_t* vtc;
    face_t*             f;
    face_group_t*       fg;
    
    material_lib_t*		matlib;
    material_t*         material;
    
    size_t     current_face_group;
    size_t     current_face;
} mesh_t;


// Allocation
mesh_t* mesh_new (size_t n_v, size_t n_vn, size_t n_vtc, size_t n_f, size_t n_fg);

void mesh_delete (mesh_t* mesh);


// Contruction
void mesh_set_vertex (mesh_t* mesh, size_t index, vertex_t* v);

void mesh_set_vertex_normal (mesh_t* mesh, size_t index, vertex_normal_t* vn);

void mesh_set_vertex_tex_coord (mesh_t* mesh, size_t index, vertex_tex_coord_t* vtc);

void mesh_begin_face_group (mesh_t* mesh, const char* name);

void mesh_set_current_material (mesh_t* mesh, const char* name);

void mesh_add_face (mesh_t* mesh, int flags, face_t* face);

// Accessing
face_group_t* mesh_get_face_group (mesh_t* mesh, const char* name);

// Rendering
void mesh_face_render (mesh_t* mesh, face_t* face);

void mesh_face_group_render (mesh_t* mesh, face_group_t* fg);

void mesh_render (mesh_t* mesh);

#include "3d/mesh/mesh_io.h"

#include "3d/mesh/vbo_mesh.h"

#endif


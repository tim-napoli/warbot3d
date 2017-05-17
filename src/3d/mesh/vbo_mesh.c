#include <GL/glew.h>
#include "3d/mesh/vbo_mesh.h"

#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------------//
// Construction
//--------------------------------------------------------------------------------------------------------------------//

// Associates a vertex index in the original mesh to index in the VBO.
typedef struct __vbo_build_map {
    size_t vbo_index;
} __vbo_build_map_t;

/*
//---------------------------------------------------- vertex --------------------------------------------------------//
// Check if a vertex array contains a vertex (by coordinates).
// Return 1 if yes, and in this case, set *i_o to the index where the vertex has been found.
static int __array_has_vertex (size_t nv, vertex_t* v, size_t* i_o, float x, float y, float z) {
    size_t index;
    vertex_t* cv;
    
    cv = v;
    for (index = 0; index < nv; index++) {
        if (cv->x == x && cv->y == y && cv->z == z) {
            *i_o = index;
            return 1;
        }
        cv++;
    }
    
    return 0;
}

// Create from the vertices of a mesh a new vertex array where it will not have have duplicates vertices.
// Make the vertex map in the same time (map must be allocated with malloc (nv * sizeof (__vbo_build_map_t)).
static vertex_t* __create_vertex_array (size_t nv, vertex_t* v, size_t* nv_o, __vbo_build_map_t* map) {
    *nv_o = 0;
    vertex_t* vbo_v = malloc (nv * sizeof (vertex_t));
    
    size_t index, index_vbo;
    vertex_t* cv = v;
    for (index = 0; index < nv; index++) {
        if (__array_has_vertex (*nv_o, vbo_v, &index_vbo, cv->x, cv->y, cv->z)) {
            map [index].vbo_index = index_vbo;
        }
        else {
            memcpy (&(vbo_v [*nv_o]), cv, sizeof (vertex_t));
            map [index].vbo_index = *nv_o;
            (*nv_o)++;
        }
        cv++;
    }
    
    vbo_v = realloc (vbo_v, (*nv_o) * sizeof (vertex_t));
    return vbo_v;
}


//------------------------------------------------ vertex normal -----------------------------------------------------//
static int __array_has_vertex_normal (size_t nvn, vertex_normal_t* vn, size_t* i_o, float x, float y, float z) {
    size_t index;
    vertex_normal_t* cvn;
    
    cvn = vn;
    for (index = 0; index < nvn; index++) {
        if (cvn->x == x && cvn->y == y && cvn->z == z) {
            *i_o = index;
            return 1;
        }
        cvn++;
    }
    
    return 0;
}

static vertex_normal_t* __create_vertex_normal_array (size_t nvn, vertex_normal_t* vn, size_t* nvn_o, 
                                                      __vbo_build_map_t* map) {
    *nvn_o = 0;
    vertex_normal_t* vbo_vn = malloc (nvn * sizeof (vertex_normal_t));
    
    size_t index, index_vbo;
    vertex_normal_t* cvn = vn;
    for (index = 0; index < nvn; index++) {
        if (__array_has_vertex_normal (*nvn_o, vbo_vn, &index_vbo, cvn->x, cvn->y, cvn->z)) {
            map [index].vbo_index = index_vbo;
        }
        else {
            memcpy (&(vbo_vn [*nvn_o]), cvn, sizeof (vertex_normal_t));
            map [index].vbo_index = *nvn_o;
            (*nvn_o)++;
        }
        cvn++;
    }
    
    vbo_vn = realloc (vbo_vn, (*nvn_o) * sizeof (vertex_normal_t));
    return vbo_vn;
}


//------------------------------------------ vertex texture coordinates ----------------------------------------------//
static int __array_has_vertex_tex_coord (size_t nvtc, vertex_tex_coord_t* vtc, size_t* i_o, float u, float v) {
    size_t index;
    vertex_tex_coord_t* cvtc;
    
    cvtc = vtc;
    for (index = 0; index < nvtc; index++) {
        if (cvtc->u == u && cvtc->v == v) {
            *i_o = index;
            return 1;
        }
        cvtc++;
    }
    
    return 0;
}

static vertex_tex_coord_t* __create_vertex_tex_coord_array (size_t nvtc, vertex_tex_coord_t* vtc, size_t* nvtc_o, 
                                                            __vbo_build_map_t* map) {
    *nvtc_o = 0;
    vertex_tex_coord_t* vbo_vtc = malloc (nvtc * sizeof (vertex_tex_coord_t));
    
    size_t index, index_vbo;
    vertex_tex_coord_t* cvtc = vtc;
    for (index = 0; index < nvtc; index++) {
        if (__array_has_vertex_tex_coord (*nvtc_o, vbo_vtc, &index_vbo, cvtc->u, cvtc->v)) {
            map [index].vbo_index = index_vbo;
        }
        else {
            memcpy (&(vbo_vtc [*nvtc_o]), cvtc, sizeof (vertex_tex_coord_t));
            map [index].vbo_index = *nvtc_o;
            (*nvtc_o)++;
        }
        cvtc++;
    }
    
    vbo_vtc = realloc (vbo_vtc, (*nvtc_o) * sizeof (vertex_tex_coord_t));
    return vbo_vtc;
}
*/


//------------------------------------------------------ misc --------------------------------------------------------//
/*
static void __begin_mesh_to_vbo_mesh (mesh_t* mesh,
                                      size_t* n_v, vertex_t** v, __vbo_build_map_t** v_map,
                                      size_t* n_vn, vertex_normal_t** vn, __vbo_build_map_t** vn_map,
                                      size_t* n_vtc, vertex_tex_coord_t** vtc, __vbo_build_map_t** vtc_map) {
    *v_map      = malloc (mesh->n_v * sizeof (__vbo_build_map_t));
    *vn_map     = malloc (mesh->n_vn * sizeof (__vbo_build_map_t));
    *vtc_map    = malloc (mesh->n_vtc * sizeof (__vbo_build_map_t));
    
    *v =    __create_vertex_array (mesh->n_v, mesh->v, n_v, *v_map);
    *vn =   __create_vertex_normal_array (mesh->n_vn, mesh->vn, n_vn, *vn_map);
    *vtc =  __create_vertex_tex_coord_array (mesh->n_vtc, mesh->vtc, n_vtc, *vtc_map);
}
*/

// Add specified face to the currently binded vbo.
static void __face_to_vbo (mesh_t* mesh, face_t* face, GLintptr* pos_v, GLintptr* pos_vn, GLintptr* pos_vtc) {
    // Putting vertex
    glBufferSubData (GL_ARRAY_BUFFER, (*pos_v),                         sizeof (vertex_t), 
                     &(mesh->v [face->v_id [0]]));
                     
    glBufferSubData (GL_ARRAY_BUFFER, (*pos_v) + 1 * sizeof (vertex_t), sizeof (vertex_t), 
                     &(mesh->v [face->v_id [1]]));
                     
    glBufferSubData (GL_ARRAY_BUFFER, (*pos_v) + 2 * sizeof (vertex_t), sizeof (vertex_t), 
                     &(mesh->v [face->v_id [2]]));
    *pos_v += 3 * sizeof (vertex_t);
    
    // Putting vertex normals
    if (face->flags & face_use_vertex_normal) {
        glBufferSubData (GL_ARRAY_BUFFER, (*pos_vn),                                sizeof (vertex_normal_t), 
                         &(mesh->vn [face->vn_id [0]]));
                         
        glBufferSubData (GL_ARRAY_BUFFER, (*pos_vn) + 1 * sizeof (vertex_normal_t), sizeof (vertex_normal_t), 
                         &(mesh->vn [face->vn_id [1]]));
                         
        glBufferSubData (GL_ARRAY_BUFFER, (*pos_vn) + 2 * sizeof (vertex_normal_t), sizeof (vertex_normal_t), 
                         &(mesh->vn [face->vn_id [2]]));
        *pos_vn += 3 * sizeof (vertex_normal_t);
    }
    
    // Putting vertex tex coords
    if (face->flags & face_use_vertex_tex_coord) {
        glBufferSubData (GL_ARRAY_BUFFER, (*pos_vtc),                                   sizeof (vertex_tex_coord_t), 
                         &(mesh->vtc [face->vtc_id [0]]));
                         
        glBufferSubData (GL_ARRAY_BUFFER, (*pos_vtc) + 1 * sizeof (vertex_tex_coord_t), sizeof (vertex_tex_coord_t), 
                         &(mesh->vtc [face->vtc_id [1]]));
                         
        glBufferSubData (GL_ARRAY_BUFFER, (*pos_vtc) + 2 * sizeof (vertex_tex_coord_t), sizeof (vertex_tex_coord_t), 
                         &(mesh->vtc [face->vtc_id [2]]));
        *pos_vtc += 3 * sizeof (vertex_tex_coord_t);
    }
}

// Add the specified face to the binded index vbo.
static void __face_to_index_vbo (face_t* face, GLintptr* pos, GLuint* current_vertex) {
    GLuint indexes [3] = {*current_vertex, (*current_vertex) + 1, (*current_vertex) + 2};
    glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, *pos, 3 * sizeof (GLuint), indexes);
    *pos += 3 * sizeof (GLuint);
    (*current_vertex) += 3;
}

static void __face_group_to_vbo (face_group_t* fg, vbo_face_group_t* vfg) {
    strcpy (vfg->name, fg->name);
    vfg->offset = (GLintptr) fg->offset * 3 * sizeof (GLuint);
    vfg->size = (GLsizei) fg->size * 3;
    if (fg->material != NULL) {
        vfg->material = material_copy (fg->material);
    }
    else {
        vfg->material = NULL;
    }
}

vbo_mesh_t* vbo_mesh_new (mesh_t* mesh) {
    vbo_mesh_t* vmesh = malloc (sizeof (vbo_mesh_t));
    
    vmesh->n_fg = mesh->n_fg;
    vmesh->fg = malloc (vmesh->n_fg * sizeof (vbo_face_group_t));
    
    // Création du buffer de données.
    glGenBuffers (1, &vmesh->data_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vmesh->data_vbo);
    
    size_t i;
    vmesh->offset_v = 0;
    vmesh->offset_vn = mesh->n_f * 3 * sizeof (vertex_t);
    vmesh->offset_vtc = vmesh->offset_vn + mesh->n_f * 3 * sizeof (vertex_normal_t);
    vmesh->data_size = (vmesh->offset_vtc + mesh->n_f * 3 * sizeof (vertex_tex_coord_t));
    GLintptr off_v = vmesh->offset_v,
             off_vn = vmesh->offset_vn,
             off_vtc = vmesh->offset_vtc;
    glBufferData (GL_ARRAY_BUFFER, vmesh->data_size, NULL, GL_STATIC_DRAW);
    for (i = 0; i < mesh->n_f; i++) {
        __face_to_vbo (mesh, &(mesh->f [i]), &off_v, &off_vn, &off_vtc);
    }
    
    // Création du buffer d'indexes.
    glGenBuffers (1, &vmesh->index_vbo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vmesh->index_vbo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, mesh->n_f * 3 * sizeof (GLuint), NULL, GL_STATIC_DRAW);
    
    GLintptr off_i = 0;
    GLuint current_vertex = 0;
    vmesh->index_size = 0;
    for (i = 0; i < mesh->n_f; i++) {
        __face_to_index_vbo (&(mesh->f [i]), &off_i, &current_vertex);
        vmesh->index_size += 3;
    }

	
	// Création des groupes de face.
	for (i = 0; i < mesh->n_fg; i++) {
	    __face_group_to_vbo (&(mesh->fg [i]), &(vmesh->fg [i]));
	}
	
	if (mesh->material != NULL) vmesh->material = material_copy (mesh->material);
	else vmesh->material = NULL;
	
	return vmesh;
}

void vbo_mesh_delete (vbo_mesh_t* mesh) {
    // Destruction des buffers.
    glDeleteBuffers (1, &mesh->data_vbo);
    glDeleteBuffers (1, &mesh->index_vbo);
    
    size_t i;
    for (i = 0; i < mesh->n_fg; i++) {
        material_delete (mesh->fg [i].material);
    }
    
    if (mesh->material != NULL) material_delete (mesh->material);
    
    if (mesh->fg != NULL) free (mesh->fg);
    free (mesh);
}

//--------------------------------------------------------------------------------------------------------------------//
// Accessing
//--------------------------------------------------------------------------------------------------------------------//
vbo_face_group_t* vbo_mesh_get_face_group (vbo_mesh_t* vmesh, const char* name) {
    size_t i;
    
    for (i = 0; i < vmesh->n_fg; i++) {
        if (!strcmp (vmesh->fg [i].name, name)) {
            return &(vmesh->fg [i]);
        }
    }
    
    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------//
// Rendering.
//--------------------------------------------------------------------------------------------------------------------//
static void __mesh_render_all (vbo_mesh_t* vmesh) {
    glBindBuffer (GL_ARRAY_BUFFER, vmesh->data_vbo);
    glVertexPointer (3, GL_FLOAT, 0, NULL);
    glNormalPointer (GL_FLOAT, 0, ((GLvoid*) NULL) + vmesh->offset_vn);
    glTexCoordPointer (2, GL_FLOAT, 0, ((GLvoid*) NULL) + vmesh->offset_vtc);
    
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vmesh->index_vbo);
    
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    
    glDrawElements (GL_TRIANGLES, vmesh->index_size, GL_UNSIGNED_INT, 0);
    
    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState (GL_NORMAL_ARRAY);
    glDisableClientState (GL_TEXTURE_COORD_ARRAY);
}

void vbo_mesh_render_face_group_begin (vbo_mesh_t* vmesh) {
    glBindBuffer (GL_ARRAY_BUFFER, vmesh->data_vbo);
    glVertexPointer (3, GL_FLOAT, 0, NULL);
    glNormalPointer (GL_FLOAT, 0, ((GLvoid*) NULL) + vmesh->offset_vn);
    glTexCoordPointer (2, GL_FLOAT, 0, ((GLvoid*) NULL) + vmesh->offset_vtc);
    
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vmesh->index_vbo);
    
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
}

void vbo_mesh_render_face_group_end () {
    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState (GL_NORMAL_ARRAY);
    glDisableClientState (GL_TEXTURE_COORD_ARRAY);
}

void vbo_mesh_render_face_group (vbo_mesh_t* vmesh, vbo_face_group_t* vfg) {
    if (vfg->material != NULL) material_use (vfg->material);
    glDrawElements (GL_TRIANGLES, vfg->size, GL_UNSIGNED_INT, ((GLvoid*) NULL) + vfg->offset);
}

void vbo_mesh_render (vbo_mesh_t* vmesh) {
    size_t i;

    if (vmesh->n_fg == 0) {
        if (vmesh->material != NULL) {
            material_use (vmesh->material);
        }
        __mesh_render_all (vmesh);
    }
    else {
        vbo_mesh_render_face_group_begin (vmesh);
        for (i = 0; i < vmesh->n_fg; i++) {
            vbo_mesh_render_face_group (vmesh, &(vmesh->fg [i]));
        }
        vbo_mesh_render_face_group_end ();
    }
}


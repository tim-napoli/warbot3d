#include <GL/glew.h>
#include <GL/gl.h>

#include "3d/mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Allocation
mesh_t* mesh_new (size_t n_v, size_t n_vn, size_t n_vtc, size_t n_f, size_t n_fg) {
    mesh_t* m = malloc (sizeof (mesh_t));
    
    m->n_v = n_v;
    m->v = malloc (n_v * sizeof (vertex_t));
    
    m->n_vn = n_vn;
    m->vn = malloc (n_vn * sizeof (vertex_normal_t));
    
    m->n_vtc = n_vtc;
    m->vtc = malloc (n_vtc * sizeof (vertex_tex_coord_t));
    
    m->n_f = n_f;
    m->f = malloc (n_f * sizeof (face_t));
    
    m->n_fg = n_fg;
    m->fg = malloc (n_fg * sizeof (face_group_t));
    
    m->matlib = NULL;
    m->material = NULL;
    
    m->current_face_group = 0;
    m->current_face = 0;
    
    return m;
}


void mesh_delete (mesh_t* m) {
    if (m->v)       free (m->v);
    if (m->vn)      free (m->vn);
    if (m->vtc)     free (m->vtc);
    if (m->f)       free (m->f);
    if (m->fg)      free (m->fg);
    if (m->matlib)  material_lib_delete (m->matlib);
    
    free (m);
}



// Construction
void mesh_set_vertex (mesh_t* mesh, size_t index, vertex_t* v) {
    #ifdef DEBUG
    if (index > mesh->n_v) {
        fprintf (stderr, "mesh_set_vertex error : Invalid index %u (have size of %u).\n", index, m->n_v);
        return;
    }
    #endif
    
    memcpy (&(mesh->v [index]), v, sizeof (vertex_t));
}

void mesh_set_vertex_normal (mesh_t* mesh, size_t index, vertex_normal_t* vn) {
    #ifdef DEBUG
    if (index > mesh->n_vn) {
        fprintf (stderr, "mesh_set_vertex_normal error : Invalid index %u (have size of %u).\n", index, m->n_vn);
        return;
    }
    #endif
    
    memcpy (&(mesh->vn [index]), vn, sizeof (vertex_normal_t));
}

void mesh_set_vertex_tex_coord (mesh_t* mesh, size_t index, vertex_tex_coord_t* vtc) {
    #ifdef DEBUG
    if (index > mesh->n_vtc) {
        fprintf (stderr, "mesh_set_vertex_tex_coord error : Invalid index %u (have size of %u).\n", index, m->n_vtc);
        return;
    }
    #endif
    
    memcpy (&(mesh->vtc [index]), vtc, sizeof (vertex_tex_coord_t));
}

void mesh_begin_face_group (mesh_t* mesh, const char* name) {
    #ifdef DEBUG
    if (mesh->current_face_group == mesh->n_fg) {
        frptinf (stderr, "mesh_begin_face_group error : Cannot add more face group to the mesh.\n"
    }
    
    if (mesh->current_face_group == 0 && mesh->current_face > 0) {
        fprintf (stdout, "mesh_begin_face_group warning : A first group has been began although some faces are already "
                         "presents in the mesh.\n");
    }
    #endif

    strcpy (mesh->fg [mesh->current_face_group].name, name);
    mesh->fg [mesh->current_face_group].offset = mesh->current_face;
    mesh->fg [mesh->current_face_group].size = 0;
    mesh->fg [mesh->current_face_group].material = NULL;
    
    mesh->current_face_group++;
}

void mesh_set_current_material (mesh_t* mesh, const char* name) {
    #ifdef DEBUG
    if (mesh->matlib == NULL) {
        fprintf (stderr, "mesh_set_current_material error : Mesh has no material library.\n");
        return;
    }
    #endif
    
    material_t* material = material_lib_get (mesh->matlib, name);
    if (material == NULL) {
        fprintf (stderr, "mesh_set_current_material error : Mesh has no material %s in its library.\n", name);
        return;
    }
    
    if (mesh->current_face_group > 0) {
        mesh->fg [mesh->current_face_group - 1].material = material;
    }
    else {
        mesh->material = material;
    }
}

void mesh_add_face (mesh_t* mesh, int flags, face_t* face) {
    #ifdef DEBUG
    if (mesh->current_face == mesh->n_f) {
        fprintf (stderr, "mesh_add_face error : Mesh has no more room for a face.\n");
        return;
    }
    #endif

    memcpy (&mesh->f [mesh->current_face], face, sizeof (face_t));
    mesh->current_face++;
    
    if (mesh->current_face_group > 0) {
        mesh->fg [mesh->current_face_group - 1].size++;
    }
}


// Accessing
face_group_t* mesh_get_face_group (mesh_t* mesh, const char* name) {
    face_group_t *fg = mesh->fg,
                 *fg_max = mesh->fg + mesh->n_fg;
    
    for (; fg < fg_max; fg++) {
        if (!strcmp (fg->name, name)) {
            return fg;
        }
    }
    
    return NULL;
}


// Rendering
void mesh_face_render (mesh_t* mesh, face_t* face) {
    vertex_t* v;
    vertex_normal_t* vn;
    vertex_tex_coord_t* vtc;

    glBegin (GL_TRIANGLES);
            if ((face->flags & (face_use_vertex_normal | face_use_vertex_tex_coord))) {
                vn = &(mesh->vn [face->vn_id [0]]);
                vtc = &(mesh->vtc [face->vtc_id [0]]);
                v = &(mesh->v [face->v_id [0]]);
                glTexCoord2f (vtc->u, vtc->v);
                glNormal3f (vn->x, vn->y, vn->z);
                glVertex3f (v->x, v->y, v->z);
                
                vn = &(mesh->vn [face->vn_id [1]]);
                vtc = &(mesh->vtc [face->vtc_id [1]]);
                v = &(mesh->v [face->v_id [1]]);
                glTexCoord2f (vtc->u, vtc->v);
                glNormal3f (vn->x, vn->y, vn->z);
                glVertex3f (v->x, v->y, v->z);
                
                vn = &(mesh->vn [face->vn_id [2]]);
                vtc = &(mesh->vtc [face->vtc_id [2]]);
                v = &(mesh->v [face->v_id [2]]);
                glTexCoord2f (vtc->u, vtc->v);
                glNormal3f (vn->x, vn->y, vn->z);
                glVertex3f (v->x, v->y, v->z);
            }
            else if (face->flags & face_use_vertex_normal) {
                vn = &(mesh->vn [face->vn_id [0]]);
                v = &(mesh->v [face->v_id [0]]);
                glNormal3f (vn->x, vn->y, vn->z);
                glVertex3f (v->x, v->y, v->z);
                
                vn = &(mesh->vn [face->vn_id [1]]);
                v = &(mesh->v [face->v_id [1]]);
                glNormal3f (vn->x, vn->y, vn->z);
                glVertex3f (v->x, v->y, v->z);
                
                vn = &(mesh->vn [face->vn_id [2]]);
                v = &(mesh->v [face->v_id [2]]);
                glNormal3f (vn->x, vn->y, vn->z);
                glVertex3f (v->x, v->y, v->z);
            }
            else if (face->flags & face_use_vertex_tex_coord) {
                vtc = &(mesh->vtc [face->vtc_id [0]]);
                v = &(mesh->v [face->v_id [0]]);
                glTexCoord2f (vtc->u, vtc->v);
                glVertex3f (v->x, v->y, v->z);
                
                vtc = &(mesh->vtc [face->vtc_id [1]]);
                v = &(mesh->v [face->v_id [1]]);
                glTexCoord2f (vtc->u, vtc->v);
                glVertex3f (v->x, v->y, v->z);
                
                vtc = &(mesh->vtc [face->vtc_id [2]]);
                v = &(mesh->v [face->v_id [2]]);
                glTexCoord2f (vtc->u, vtc->v);
                glVertex3f (v->x, v->y, v->z);
            }
            else {
                v = &(mesh->v [face->v_id [0]]);
                glVertex3f (v->x, v->y, v->z);
                
                v = &(mesh->v [face->v_id [1]]);
                glVertex3f (v->x, v->y, v->z);
                
                v = &(mesh->v [face->v_id [2]]);
                glVertex3f (v->x, v->y, v->z);
            }
    glEnd ();
}

void mesh_face_group_render (mesh_t* mesh, face_group_t* fg) {
    face_t* it = &(mesh->f [fg->offset]);
    face_t* it_max = it + fg->size;

    if (fg->material != NULL) {
        material_use (fg->material);
    }
    
    for (; it < it_max; it++) {
        mesh_face_render (mesh, it);
    }
}

void mesh_render (mesh_t* mesh) {
    face_group_t    *it_fg, *it_fg_max;
    face_t          *it_f, *it_f_max;
    
    if (mesh->current_face_group > 0) {
        it_fg = mesh->fg;
        it_fg_max = it_fg + mesh->n_fg;
        for (; it_fg < it_fg_max; it_fg++) {
            mesh_face_group_render (mesh, it_fg);
        }
    }
    else {
        if (mesh->material != NULL) {
            material_use (mesh->material);
        }
        
        it_f = mesh->f;
        it_f_max = it_f + mesh->n_f;
        for (; it_f < it_f_max; it_f++) {
            mesh_face_render (mesh, it_f);
        }
    }
}


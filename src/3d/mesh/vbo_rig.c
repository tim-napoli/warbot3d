#include "3d/mesh/vbo_rig.h"

#include <stdlib.h>
#include <string.h>

vbo_rig_t* vbo_rig_new (vbo_rig_t* parent, 
                        float p [4], float s [4], float a [4], 
                        vbo_mesh_t* mesh, vbo_face_group_t* group, 
                        size_t nchilds) {
    vbo_rig_t* rig = malloc (sizeof (vbo_rig_t));
    
    rig->parent = parent;
    memcpy (rig->position, p, 4 * sizeof (float));
    memcpy (rig->scale, s, 4 * sizeof (float));
    memcpy (rig->angles, a, 4 * sizeof (float));
    
    rig->mesh = mesh;
    rig->group = group;
    
    rig->nchilds = nchilds;
    rig->childs = malloc (nchilds * sizeof (vbo_rig_t*));
    memset (rig->childs, 0, nchilds * sizeof (vbo_rig_t*));

    
    return rig;
}

void vbo_rig_delete (vbo_rig_t* rig) {
    size_t i;
    for (i = 0; i < rig->nchilds; i++) {
        if (rig->childs [i] != NULL) vbo_rig_delete (rig->childs [i]);
    }
    
    free (rig->childs);
    free (rig);
}

void vbo_rig_set_child (vbo_rig_t* rig, size_t n, vbo_rig_t* child) {
    rig->childs [n] = child;
}

void vbo_rig_render (vbo_rig_t* rig) {
    size_t i;

    if (rig->parent == NULL) {
        vbo_mesh_render_face_group_begin (rig->mesh);
    }

    glPushMatrix ();
        glTranslatef (rig->position [0], rig->position [1], rig->position [2]);
        
        glRotatef (rig->angles [2], 0, 0, 1);
        glRotatef (rig->angles [1], 0, 1, 0);
        glRotatef (rig->angles [0], 1, 0, 0);
        
        // La position de la racine est relative au monde, pas les autres.
        if (rig->parent != NULL) {
            glTranslatef (-rig->position [0], -rig->position [1], -rig->position [2]);
        }
        
        // Rendering
        vbo_mesh_render_face_group (rig->mesh, rig->group);
        
        // Childs...
        for (i = 0; i < rig->nchilds; i++) {
            if (rig->childs [i] != NULL) {
                vbo_rig_render (rig->childs [i]);
            }
        }
    glPopMatrix ();
    
    if (rig->parent == NULL) {
        vbo_mesh_render_face_group_end ();
    }
}

void vbo_rig_get_model_matrix (vbo_rig_t* rig) {
    glLoadIdentity ();
    
    glTranslatef (rig->position [0], rig->position [1], rig->position [2]);
    
    glRotatef (rig->angles [2], 0, 0, 1);
    glRotatef (rig->angles [1], 0, 1, 0);
    glRotatef (rig->angles [0], 1, 0, 0);
        
    if (rig->parent != NULL) {
        glTranslatef (-rig->position [0], -rig->position [1], -rig->position [2]);
    }
}


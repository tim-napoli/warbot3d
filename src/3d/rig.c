#include "3d/rig.h"

#include <stdlib.h>
#include <string.h>

rig_t* rig_new (rig_t* parent, float p [4], float s [4], float a [4], mesh_t* mesh, face_group_t* group, uint32_t nchilds) {
    rig_t* rig = malloc (sizeof (rig_t));
    
    rig->parent = parent;
    
    memcpy (rig->position, p, 4 * sizeof (float));
    memcpy (rig->scale, s, 4 * sizeof (float));
    memcpy (rig->angles, a, 4 * sizeof (float));
    
    rig->mesh = mesh;
    rig->group = group;
    rig->nchilds = nchilds;
    if (rig->nchilds > 0) {
        rig->childs = malloc (nchilds * sizeof (rig_t*));
        memset (rig->childs, 0, nchilds * sizeof (rig_t*));
    }
    else {
        rig->childs = NULL;
    }
    
    return rig;
}

void rig_delete (rig_t* rig) {
    int i;

    if (rig->nchilds > 0) {
        for (i = 0; i < rig->nchilds; i++) {
            if (rig->childs [i] != NULL) {
                rig_delete (rig->childs [i]);
            }
        }
        
        free (rig->childs);
    }
    
    free (rig);
}

void rig_set_child (rig_t* rig, uint32_t n, rig_t* child) {
    rig->childs [n] = child;
}


void rig_render (rig_t* rig) {
    int i;

    glPushMatrix ();
        // First we need to do the rotation and scale, so :
        // - We will do the scale.
        // - We will translate the group to the origin (using its position).
        // - Make rotation
        // - Translation to the position.
        // Scale
        //glScalef (rig->scale [0], rig->scale [1], rig->scale [2]);
        // Translation to the origin.
        //
        // Rotation.
        //if (rig->parent != NULL) {
            glTranslatef (rig->position [0], rig->position [1], rig->position [2]);
        //}
        
        
        glRotatef (rig->angles [2], 0, 0, 1);
        glRotatef (rig->angles [1], 0, 1, 0);
        glRotatef (rig->angles [0], 1, 0, 0);
        
        if (rig->parent != NULL) {
            glTranslatef (-rig->position [0], -rig->position [1], -rig->position [2]);
        }
        
        // Rendering
        mesh_face_group_render (rig->mesh, rig->group);
        
        // Childs...
        for (i = 0; i < rig->nchilds; i++) {
            if (rig->childs [i] != NULL) {
                rig_render (rig->childs [i]);
            }
        }
    glPopMatrix ();
}


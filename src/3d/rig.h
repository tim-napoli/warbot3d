/* rig.h
 *  A rig is a tree structure associated to a mesh.
 *  Each rig node is associated to a mesh face group, and give it a rotation point (jointure) and angles.
 *  The root node position give the position of the whole mesh in the world.
 */

#ifndef _rig_h_
#define _rig_h_

#include "3d/mesh.h"

typedef struct rig {
    struct rig* parent;

	float position [4];     // The position is relative to the world if this rig node is a root, or to its parent if it 
	                        // is a child.
	float scale [4];        // The scale of the rig (this will be applied from the "center" of the rig ?)
	float angles [4];       // The angles of the rig (which orientation it has).
	
	mesh_t*             mesh;       // Mesh.
	face_group_t*	group;		// Mesh Face Group linked to this rig.
	
	uint32_t		nchilds;	// Childs.
	struct rig**	childs;
} rig_t;

// Create a new rig in memory.
rig_t* rig_new (rig_t* parent, float position [4], float scale [4], float angles [4], mesh_t* mesh, 
                face_group_t* group, uint32_t nchilds);

// Delete a rig from memory.
void rig_delete (rig_t* rig);

// Set the rig's child at offset n.
void rig_set_child (rig_t* rig, uint32_t n, rig_t* child);

// Render a rig and its childrens.
void rig_render (rig_t* rig);

#include "3d/mesh/vbo_rig.h"

#endif


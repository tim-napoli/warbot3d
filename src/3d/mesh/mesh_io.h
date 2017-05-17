#ifndef _mesh_io_h_
#define _mesh_io_h_

#include "3d/mesh.h"

// Load a wavefront obj file in a mesh.
// Return NULL on error (print error message on stderr).
mesh_t* mesh_load_obj (const char* filename);

#endif


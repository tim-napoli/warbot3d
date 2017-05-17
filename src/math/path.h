#ifndef _path_h_
#define _path_h_

#include "math/vector.h"

typedef struct path {
    vector_t p1, p2, p3, p4;
} path_t;

void path_init (path_t* path, vector_t* p1, vector_t* p2, vector_t* p3, vector_t* p4);

void path_get_point_at (path_t* path, int t, vector_t* out);

#endif


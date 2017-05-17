#include "math/matrix.h"

#include <math.h>
#include <string.h>

void matrix_copy_c (matrix_t* dst, matrix_t* src) {
    memcpy (dst, src, sizeof (matrix_t));
}

static matrix_t __matrix_identity = {
    {1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f}    
};

static matrix_t __matrix_empty = {{0.0f}};

void matrix_init_identity (matrix_t* m) {
    matrix_copy (m, &__matrix_identity);
}

void matrix_init_translation (matrix_t* m, float x, float y, float z) {
    matrix_init_identity (m);

    m->c [3] = x;
    m->c [7] = y;
    m->c [11] = z;
}

void matrix_init_scale (matrix_t* m, float x, float y, float z) {
    matrix_copy (m, &__matrix_empty);
    m->c [0] = x;
    m->c [5] = y;
    m->c [10] = z;
    m->c [15] = 1;
}

void matrix_init_rotation (matrix_t* m, vector_t* u, float a) {
    float c = cos (a);
    float s = sin (a);
    
    vector_normalize (u);
    
    float ux2 = u->x * u->x;
    float uy2 = u->y * u->y;
    float uz2 = u->z * u->z;

    m->c [0] = ux2 + (1 - ux2) * c;
    m->c [1] = u->x * u->y * (1 - c) - u->z * s;
    m->c [2] = u->x * u->z * (1 - c) + u->y * s;
    m->c [3] = 0;
    
    m->c [4] = u->x * u->y * (1 - c) + u->z * s;
    m->c [5] = uy2 + (1 - uy2) * c;
    m->c [6] = u->y * u->z * (1 - c) - u->x * s;
    m->c [7] = 0;
    
    m->c [8] = u->x * u->z * (1 - c) - u->y * s;
    m->c [9] = u->y * u->z * (1 - c) + u->x * s;
    m->c [10] = uz2  + (1 - uz2) * c;
    m->c [11] = 0;
    
    m->c [12] = 0;
    m->c [13] = 0;
    m->c [14] = 0;
    m->c [15] = 0;
}

void matrix_init_rotation_around (matrix_t* m, vector_t* p, vector_t* u, float a) {
/*
    matrix_t a, b, c;

    matrix_init_translation (&a, -p->x, -p->y, -p->z);
    matrix_init_rotation (&b, u, a);
    matrix_init_translation (&c, p->x, p->y, p->z);
    
    matrix_mul_ex (m, &a, &b);
    matrix_mul (m, &b);
    */
}

void matrix_add_c (matrix_t* a, matrix_t* b) {
    a->c [0] += b->c [0];
    a->c [1] += b->c [1];
    a->c [2] += b->c [2];
    a->c [3] += b->c [3];
    
    a->c [4] += b->c [4];
    a->c [5] += b->c [5];
    a->c [6] += b->c [6];
    a->c [7] += b->c [7];
    
    a->c [8] += b->c [8];
    a->c [9] += b->c [9];
    a->c [10] += b->c [10];
    a->c [11] += b->c [11];
    
    a->c [12] += b->c [12];
    a->c [13] += b->c [13];
    a->c [14] += b->c [14];
    a->c [15] += b->c [15];
}

void matrix_add_ex_c (matrix_t* res, matrix_t* a, matrix_t* b) {
    res->c [0] = a->c [0] + b->c [0];
    res->c [1] = a->c [1] + b->c [1];
    res->c [2] = a->c [2] + b->c [2];
    res->c [3] = a->c [3] + b->c [3];
    
    res->c [4] = a->c [4] + b->c [4];
    res->c [5] = a->c [5] + b->c [5];
    res->c [6] = a->c [6] + b->c [6];
    res->c [7] = a->c [7] + b->c [7];
    
    res->c [8] = a->c [8] + b->c [8];
    res->c [9] = a->c [9] + b->c [9];
    res->c [10] = a->c [10] + b->c [10];
    res->c [11] = a->c [11] + b->c [11];
    
    res->c [12] = a->c [12] + b->c [12];
    res->c [13] = a->c [13] + b->c [13];
    res->c [14] = a->c [14] + b->c [14];
    res->c [15] = a->c [15] + b->c [15];
}

#define __swap(x,y,t)     t = x; x = y; y = t;
void matrix_transpose_c (matrix_t* matrix) {
    float t;

    __swap (matrix->c [1], matrix->c [4], t);
    __swap (matrix->c [2], matrix->c [8], t);
    __swap (matrix->c [3], matrix->c [12], t);
    __swap (matrix->c [6], matrix->c [9], t);
    __swap (matrix->c [7], matrix->c [13], t);
    __swap (matrix->c [11], matrix->c [14], t);
}

void matrix_transpose_ex_c (matrix_t* res, matrix_t* m) {
    res->c [0] = m->c [0];
    res->c [1] = m->c [4];
    res->c [2] = m->c [8];
    res->c [3] = m->c [12];

    res->c [4] = m->c [1];
    res->c [5] = m->c [5];
    res->c [6] = m->c [9];
    res->c [7] = m->c [13];
    
    res->c [8] = m->c [2];
    res->c [9] = m->c [6];
    res->c [10] = m->c [10];
    res->c [11] = m->c [14];
    
    res->c [12] = m->c [3];
    res->c [13] = m->c [7];
    res->c [14] = m->c [11];
    res->c [15] = m->c [15];
}

void matrix_mul_c (matrix_t* a, matrix_t* b) {
    matrix_t c;
    
    matrix_copy (&c, a);
    
    a->c [0] = c.c [0] * b->c [0] 
                 + c.c [1] * b->c [4]
                 + c.c [2] * b->c [8]
                 + c.c [3] * b->c [12];
    
    a->c [1] = c.c [0] * b->c [1] 
                 + c.c [1] * b->c [5]
                 + c.c [2] * b->c [9]
                 + c.c [3] * b->c [13];
              
    a->c [2] = c.c [0] * b->c [2] 
                 + c.c [1] * b->c [6]
                 + c.c [2] * b->c [10]
                 + c.c [3] * b->c [14];
    
    a->c [3] = c.c [0] * b->c [3] 
                 + c.c [1] * b->c [7]
                 + c.c [2] * b->c [11]
                 + c.c [3] * b->c [15];
    
    
    a->c [4] = c.c [4] * b->c [0] 
                 + c.c [5] * b->c [4]
                 + c.c [6] * b->c [8]
                 + c.c [7] * b->c [12];
    
    a->c [5] = c.c [4] * b->c [1] 
                 + c.c [5] * b->c [5]
                 + c.c [6] * b->c [9]
                 + c.c [7] * b->c [13];
              
    a->c [6] = c.c [4] * b->c [2] 
                 + c.c [5] * b->c [6]
                 + c.c [6] * b->c [10]
                 + c.c [7] * b->c [14];
    
    a->c [7] = c.c [4] * b->c [3] 
                 + c.c [5] * b->c [7]
                 + c.c [6] * b->c [11]
                 + c.c [7] * b->c [15];
    
    
    a->c [8] = c.c [8] * b->c [0] 
                 + c.c [9] * b->c [4]
                 + c.c [10] * b->c [8]
                 + c.c [11] * b->c [12];
    
    a->c [9] = c.c [8] * b->c [1] 
                 + c.c [9] * b->c [5]
                 + c.c [10] * b->c [9]
                 + c.c [11] * b->c [13];
              
    a->c [10] = c.c [8] * b->c [2] 
                 + c.c [9] * b->c [6]
                 + c.c [10] * b->c [10]
                 + c.c [11] * b->c [14];
    
    a->c [11] = c.c [8] * b->c [3] 
                 + c.c [9] * b->c [7]
                 + c.c [10] * b->c [11]
                 + c.c [11] * b->c [15];
    
    
    a->c [12] = c.c [12] * b->c [0] 
                 + c.c [13] * b->c [4]
                 + c.c [14] * b->c [8]
                 + c.c [15] * b->c [12];
    
    a->c [13] = c.c [12] * b->c [1] 
                 + c.c [13] * b->c [5]
                 + c.c [14] * b->c [9]
                 + c.c [15] * b->c [13];
              
    a->c [14] = c.c [12] * b->c [2] 
                 + c.c [13] * b->c [6]
                 + c.c [14] * b->c [10]
                 + c.c [15] * b->c [14];
    
    a->c [15] = c.c [12] * b->c [3] 
                 + c.c [13] * b->c [7]
                 + c.c [14] * b->c [11]
                 + c.c [15] * b->c [15];
}


void matrix_mul_ex_c (matrix_t* res, matrix_t* a, matrix_t* b) {
 
    res->c [0] = a->c [0] * b->c [0] 
                   + a->c [1] * b->c [4]
                   + a->c [2] * b->c [8]
                   + a->c [3] * b->c [12];
    
    res->c [1] = a->c [0] * b->c [1] 
                   + a->c [1] * b->c [5]
                   + a->c [2] * b->c [9]
                   + a->c [3] * b->c [13];
              
    res->c [2] = a->c [0] * b->c [2] 
                   + a->c [1] * b->c [6]
                   + a->c [2] * b->c [10]
                   + a->c [3] * b->c [14];
    
    res->c [3] = a->c [0] * b->c [3] 
                   + a->c [1] * b->c [7]
                   + a->c [2] * b->c [11]
                   + a->c [3] * b->c [15];
    
    
    res->c [4] = a->c [4] * b->c [0] 
                   + a->c [5] * b->c [4]
                   + a->c [6] * b->c [8]
                   + a->c [7] * b->c [12];
    
    res->c [5] = a->c [4] * b->c [1] 
                   + a->c [5] * b->c [5]
                   + a->c [6] * b->c [9]
                   + a->c [7] * b->c [13];
              
    res->c [6] = a->c [4] * b->c [2] 
                   + a->c [5] * b->c [6]
                   + a->c [6] * b->c [10]
                   + a->c [7] * b->c [14];
    
    res->c [7] = a->c [4] * b->c [3] 
                   + a->c [5] * b->c [7]
                   + a->c [6] * b->c [11]
                   + a->c [7] * b->c [15];
    
    
    res->c [8] = a->c [8] * b->c [0] 
                   + a->c [9] * b->c [4]
                   + a->c [10] * b->c [8]
                   + a->c [11] * b->c [12];
    
    res->c [9] = a->c [8] * b->c [1] 
                   + a->c [9] * b->c [5]
                   + a->c [10] * b->c [9]
                   + a->c [11] * b->c [13];
              
    res->c [10] = a->c [8] * b->c [2] 
                   + a->c [9] * b->c [6]
                   + a->c [10] * b->c [10]
                   + a->c [11] * b->c [14];
    
    res->c [11] = a->c [8] * b->c [3] 
                   + a->c [9] * b->c [7]
                   + a->c [10] * b->c [11]
                   + a->c [11] * b->c [15];
    
    
    res->c [12] = a->c [12] * b->c [0] 
                   + a->c [13] * b->c [4]
                   + a->c [14] * b->c [8]
                   + a->c [15] * b->c [12];
    
    res->c [13] = a->c [12] * b->c [1] 
                   + a->c [13] * b->c [5]
                   + a->c [14] * b->c [9]
                   + a->c [15] * b->c [13];
              
    res->c [14] = a->c [12] * b->c [2] 
                   + a->c [13] * b->c [6]
                   + a->c [14] * b->c [10]
                   + a->c [15] * b->c [14];
    
    res->c [15] = a->c [12] * b->c [3] 
                   + a->c [13] * b->c [7]
                   + a->c [14] * b->c [11]
                   + a->c [15] * b->c [15];
}


void matrix_apply_c (matrix_t* a, vector_t* in, vector_t* out) {
    out->x = a->c [0] * in->x + a->c [1] * in->y + a->c [2] * in->z + a->c [3] * in->w;
    out->y = a->c [4] * in->x + a->c [5] * in->y + a->c [6] * in->z + a->c [7] * in->w;
    out->z = a->c [8] * in->x + a->c [9] * in->y + a->c [10] * in->z + a->c [11] * in->w;
    out->w = a->c [12] * in->x + a->c [13] * in->y + a->c [14] * in->z + a->c [15] * in->w;
}

void matrix_apply_ex_c (matrix_t* a, vector_t* in_out) {
    vector_t in = {in_out->x, in_out->y, in_out->z, in_out->w};

    in_out->x = a->c [0] * in.x + a->c [1] * in.y + a->c [2] * in.z + a->c [3] * in.w;
    in_out->y = a->c [4] * in.x + a->c [5] * in.y + a->c [6] * in.z + a->c [7] * in.w;
    in_out->z = a->c [8] * in.x + a->c [9] * in.y + a->c [10] * in.z + a->c [11] * in.w;
    in_out->w = a->c [12] * in.x + a->c [13] * in.y + a->c [14] * in.z + a->c [15] * in.w;
}


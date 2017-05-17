#ifndef _matrix_h_
#define _matrix_h_

#include "math/vector.h"


typedef struct matrix {
    float c [16]; // [Horizontaly] [Verticaly]
} matrix_t;




#if defined(SSE41) || defined(SSE42)
    #define matrix_copy             matrix_copy_sse
    #define matrix_add              matrix_add_sse
    #define matrix_add_ex           matrix_add_ex_sse
    #define matrix_transpose        matrix_transpose_sse
    #define matrix_transpose_ex     matrix_transpose_ex_sse
    #define matrix_mul              matrix_mul_sse2
    #define matrix_mul_ex           matrix_mul_ex
    #define matrix_apply            matrix_apply_sse2
#elif defined(SSE2)
    #define matrix_copy             matrix_copy_sse
    #define matrix_add              matrix_add_sse
    #define matrix_add_ex           matrix_add_ex_sse
    #define matrix_transpose        matrix_transpose_sse
    #define matrix_transpose_ex     matrix_transpose_ex_sse
    #define matrix_mul              matrix_mul_sse2
    #define matrix_mul_ex           matrix_mul_ex_sse2
    #define matrix_apply            matrix_apply_sse2
#elif defined(SSE)
    #define matrix_copy             matrix_copy_sse
    #define matrix_add              matrix_add_sse
    #define matrix_add_ex           matrix_add_ex_sse
    #define matrix_transpose        matrix_transpose_sse
    #define matrix_transpose_ex     matrix_transpose_ex_sse
    #define matrix_mul              matrix_mul_c
    #define matrix_mul_ex           matrix_mul_ex_c
    #define matrix_apply            matrix_apply_c
#else
    #define matrix_copy             matrix_copy_c
    #define matrix_add              matrix_add_c
    #define matrix_add_ex           matrix_add_ex_c
    #define matrix_transpose        matrix_transpose_c
    #define matrix_transpose_ex     matrix_transpose_ex_c
    #define matrix_mul              matrix_mul_c
    #define matrix_mul_ex           matrix_mul_ex_c
    #define matrix_apply            matrix_apply_c
    #define matrix_apply_ex         matrix_apply_ex_c
#endif


void matrix_copy (matrix_t* dst, matrix_t* src);


void matrix_init_identity (matrix_t* m);

void matrix_init_translation (matrix_t* m, float x, float y, float z);

void matrix_init_scale (matrix_t* m, float x, float y, float z);

void matrix_init_rotation (matrix_t* m, vector_t* u, float a);

void matrix_init_rotation_around (matrix_t* m, vector_t* p, vector_t* u, float a);


void matrix_transpose (matrix_t* m);

void matrix_transpose_ex (matrix_t* res, matrix_t* m);


void matrix_add (matrix_t* a, matrix_t* b);

void matrix_add_ex (matrix_t* res, matrix_t* a, matrix_t* b);


void matrix_mul (matrix_t* a, matrix_t* b);

void matrix_mul_ex (matrix_t* res, matrix_t* a, matrix_t* b);


void matrix_scalar_mul (matrix_t* a, float s);

void matrix_scalar_mul_ex (matrix_t* res, matrix_t* a, float s);


void matrix_apply (matrix_t* a, vector_t* in, vector_t* out);

void matrix_apply_ex (matrix_t* a, vector_t* in_out);

#endif


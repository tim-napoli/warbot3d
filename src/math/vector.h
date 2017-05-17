#ifndef _vector_h_
#define _vector_h_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
    float x, y, z, w;
} vector_t;

extern vector_t vector_zero;

extern void vector_add_sse (vector_t* u, vector_t* v);
extern void vector_add_ex_sse (vector_t* res, vector_t* u, vector_t* v);

extern void vector_sub_sse (vector_t* u, vector_t* v);
extern void vector_sub_ex_sse (vector_t* res, vector_t* u, vector_t* v);

extern void vector_scalar_mul_sse (vector_t* u, float f);
extern void vector_scalar_mul_ex_sse (vector_t* res, vector_t* u, float f);

extern float vector_length_sse (vector_t* u);
extern float vector_length_sse2 (vector_t* u);
extern float vector_length_sse41 (vector_t* u);

extern float vector_dot_product_sse2 (vector_t* u, vector_t* v);
extern float vector_dot_product_sse41 (vector_t* u, vector_t* v);

extern void vector_cross_product_sse2 (vector_t* w, vector_t* u, vector_t* v);

extern void vector_normalize_sse (vector_t* u);
extern void vector_normalize_sse2 (vector_t* u);

extern void vector_normalize_ex_sse (vector_t* w, vector_t* u);
extern void vector_normalize_ex_sse2 (vector_t* w, vector_t* u);


#if defined(SSE41) || defined(SSE42)
    #define vector_length           vector_length_sse41
    #define vector_dot_product      vector_dot_product_sse41
    #define vector_normalize        vector_normalize_sse2
    #define vector_normalize_ex     vector_normalize_ex_sse2
#endif
#if defined(SSE2) && !(defined(SSE41) || defined(SSE42))
    #define vector_length           vector_length_sse2
    #define vector_dot_product      vector_dot_product_sse2
    #define vector_normalize        vector_normalize_sse2
    #define vector_normalize_ex     vector_normalize_ex_sse2
#endif
#if defined(SSE)
    #define vector_add              vector_add_sse
    #define vector_add_ex           vector_add_ex_sse

    #define vector_scalar_mul       vector_scalar_mul_sse
    #define vector_scalar_mul_ex    vector_scalar_mul_ex_sse
        
    #define vector_sub              vector_sub_sse
    #define vector_sub_ex           vector_sub_ex_sse

    #if !(defined(SSE41) || defined(SSE42) || defined(SSE2))
        #define vector_length           vector_length_sse
        #define vector_dot_product      vector_dot_product_c
        #define vector_normalize        vector_normalize_sse
        #define vector_normalize_ex     vector_normalize_ex_sse
    #endif
    
    #define vector_cross_product    vector_cross_product_sse
#else

    #define vector_add              vector_add_c
    #define vector_add_ex           vector_add_ex_c

    #define vector_scalar_mul       vector_scalar_mul_c
    #define vector_scalar_mul_ex    vector_scalar_mul_ex_c
        
    #define vector_sub              vector_sub_c
    #define vector_sub_ex           vector_sub_ex_c

    #define vector_length           vector_length_c

    #define vector_dot_product      vector_dot_product_c
    #define vector_cross_product    vector_cross_product_c
    #define vector_normalize        vector_normalize_c
    #define vector_normalize_ex     vector_normalize_ex_c
#endif


// Copy v in u (v and u are vector_t*).
#define vector_copy(u,v)	memcpy (u, v, sizeof (vector_t));

// Make the addition of two vectors.
void vector_add (vector_t* u, vector_t* v);

void vector_add_ex (vector_t* res, vector_t* u, vector_t* v);

// Make the substraction of two vectors.
void vector_sub (vector_t* u, vector_t* v);

void vector_sub_ex (vector_t* res, vector_t* u, vector_t* v);

// Scalar multilication.
void vector_scalar_mul (vector_t* v, float s);

void vector_scalar_mul_ex (vector_t* res, vector_t* v, float s);

// Vector length
float vector_length (vector_t* v);

// Check if the vector is normalized
#define vector_normalized(v)	(vector_length (v) == 1.0f)

// Normalize the vector
void vector_normalize (vector_t* u);

void vector_normalize_ex (vector_t* res, vector_t* u);

float vector_dot_product (vector_t* u, vector_t* v);

void vector_cross_product (vector_t* res, vector_t* u, vector_t* v);

float vector_get_angle (vector_t* u, vector_t* v);

void vector_print (FILE* out, vector_t* u);

#endif

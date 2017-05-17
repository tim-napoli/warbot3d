#include <math.h>
#include "math/vector.h"

vector_t vector_zero = {.0f, .0f, .0f, .0f};

void vector_add_c (vector_t* u, vector_t* v) {
    u->x += v->x;
    u->y += v->y;
    u->z += v->z;
    u->w += v->w;
}

void vector_add_ex_c (vector_t* res, vector_t* u, vector_t* v) {
    res->x = u->x + v->x;
    res->y = u->y + v->y;
    res->z = u->z + v->z;
    res->w = u->w + v->w;
}

void vector_sub_c (vector_t* u, vector_t* v) {
    u->x -= v->x;
    u->y -= v->y;
    u->z -= v->z;
    u->w -= v->w;
}

void vector_sub_ex_c (vector_t* res, vector_t* u, vector_t* v) {
    res->x = u->x - v->x;
    res->y = u->y - v->y;
    res->z = u->z - v->z;
    res->w = u->w - v->w;
}

void vector_scalar_mul_c (vector_t* v, float s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
    v->w *= s;
}

void vector_scalar_mul_ex_c (vector_t* res, vector_t* v, float s) {
    res->x = v->x * s;
    res->y = v->y * s;
    res->z = v->z * s;
    res->w = v->w * s;
}

float vector_length_c (vector_t* v) {
    float length = sqrt (v->x * v->x + v->y * v->y + v->z * v->z);
    return length;
}

void vector_normalize_c (vector_t* u) {
    float s = 1.0f / vector_length (u);
    vector_scalar_mul (u, s);
}

void vector_normalize_ex_c (vector_t* res, vector_t* u) {
    float s = 1.0f / vector_length (u);
    vector_scalar_mul_ex (res, u, s);
}

float vector_dot_product_c (vector_t* u, vector_t* v) {
    return u->x * v->x + u->y * v->y + u->z * v->z;
}

void vector_cross_product_c (vector_t* res, vector_t* u, vector_t* v) {
    res->x = u->y * v->z - u->z * v->y;
    res->y = u->z * v->x - u->x * v->z;
    res->z = u->x * v->y - u->y * v->x;
    res->w = 0.0f;
}

float vector_get_angle (vector_t* u, vector_t* v) {
    float s, c;
    vector_t cross;
    
    vector_cross_product (&cross, u, v);
    
    s = vector_length (&cross);
    c = vector_dot_product (u, v);

    return atan2 (s, c);
}

void vector_print (FILE* out, vector_t* u) {
    fprintf (out, "(%.5f, %.5f, %.5f, %.5f)", u->x, u->y, u->z, u->w);
}


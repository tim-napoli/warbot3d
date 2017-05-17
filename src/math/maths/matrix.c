#include "matrix.h"

#include <string.h>
#include <math.h>

float matrix_identity [16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

void matrix_copy (float* a, const float* b) {
    memcpy (a, b, 16 * sizeof (float));
}

void matrix_init_identity (float* m) {
    matrix_copy (m, matrix_identity);
}

void matrix_init_translation (float* m, float x, float y, float z) {
    matrix_copy (m, matrix_identity);
    
    m [MATRIX_W1] = x;
    m [MATRIX_W2] = y;
    m [MATRIX_W3] = z;
}

void matrix_init_rotation (float* m, float ax, float ay, float az, float angle) {
    float c = cos (angle),
          s = sin (angle);
    
    float ax2 = ax * ax;
    float ay2 = ay * ay;
    float az2 = az * az;
    
    m [MATRIX_X1] = ax2 + (1 - ax2) * c;
    m [MATRIX_Y1] = ax * ay * (1 - c) - az * s;
    m [MATRIX_Z1] = ax * az * (1 - c) + ay * s;
    m [MATRIX_W1] = 0.0f;
    
    m [MATRIX_X2] = ax * ay * (1 - c) + az * s;
    m [MATRIX_Y2] = ay2 + (1 - ay2) * c;
    m [MATRIX_Z2] = ay * az * (1 - c) - ax * s;
    m [MATRIX_W2] = 0.0f;
    
    m [MATRIX_X3] = ax * az * (1 - c) - ay * s;
    m [MATRIX_Y3] = ay * az * (1 - c) + ax * s;
    m [MATRIX_Z3] = az2 + (1 - az2) * c;
    m [MATRIX_W3] = 0.0f;
    
    m [MATRIX_X4] = 0.0f;
    m [MATRIX_Y4] = 0.0f;
    m [MATRIX_Z4] = 0.0f;
    m [MATRIX_W4] = 1.0f;
}

void matrix_init_scale (float* m, float sx, float sy, float sz) {
    matrix_copy (m, matrix_identity);
    
    m [MATRIX_X1] = sx;
    m [MATRIX_Y2] = sy;
    m [MATRIX_Z3] = sz;
}

void matrix_add (float* a, const float* b) {
    int i;
    
    for (i = 0; i < 16; i++) {
        a [i] += b [i];
    }
}

void matrix_add_to (float* a, const float* b, const float* c) {
    int i;
    
    for (i = 0; i < 16; i++) {
        a [i] = b [i] + c [i];
    }
}

void matrix_mul_to (float* a, const float* b, const float* c) {
    int i, j;
    
    a [MATRIX_X1] = b [0] * c [0] + b [1] * c [4] + b [2] * c [8] + b [3] * c [12];
    a [MATRIX_X2] = b [0] * c [1] + b [1] * c [5] + b [2] * c [9] + b [3] * c [13];
    a [MATRIX_X3] = b [0] * c [2] + b [1] * c [6] + b [2] * c [10] + b [3] * c [14];
    a [MATRIX_X4] = b [0] * c [3] + b [1] * c [7] + b [2] * c [11] + b [3] * c [15];
    
    a [MATRIX_Y1] = b [4] * c [0] + b [5] * c [4] + b [6] * c [8] + b [7] * c [12];
    a [MATRIX_Y2] = b [4] * c [1] + b [5] * c [5] + b [6] * c [9] + b [7] * c [13];
    a [MATRIX_Y3] = b [4] * c [2] + b [5] * c [6] + b [6] * c [10] + b [7] * c [14];
    a [MATRIX_Y4] = b [4] * c [3] + b [5] * c [7] + b [6] * c [11] + b [7] * c [15];
    
    a [MATRIX_Z1] = b [8] * c [0] + b [9] * c [4] + b [10] * c [8] + b [11] * c [12];
    a [MATRIX_Z2] = b [8] * c [1] + b [9] * c [5] + b [10] * c [9] + b [11] * c [13];
    a [MATRIX_Z3] = b [8] * c [2] + b [9] * c [6] + b [10] * c [10] + b [11] * c [14];
    a [MATRIX_Z4] = b [8] * c [3] + b [9] * c [7] + b [10] * c [11] + b [11] * c [15];
    
    a [MATRIX_W1] = b [12] * c [0] + b [13] * c [4] + b [14] * c [8] + b [15] * c [12];
    a [MATRIX_W2] = b [12] * c [1] + b [13] * c [5] + b [14] * c [9] + b [15] * c [13];
    a [MATRIX_W3] = b [12] * c [2] + b [13] * c [6] + b [14] * c [10] + b [15] * c [14];
    a [MATRIX_W4] = b [12] * c [3] + b [13] * c [7] + b [14] * c [11] + b [15] * c [15];
}

void matrix_mul (float* a, const float* b) {
    float c [16];
    
    matrix_copy (c, a);
    matrix_mul_to (a, c, b);
}

void matrix_scalar_mul (float* m, float s) {
    int i;
    for (i = 0; i < 16; i++) {
        m [i] *= s;
    }
}

void matrix_scalar_mul_to (float* a, const float* b, float s) {
    int i;
    for (i = 0; i < 16; i++) {
        a [i] = b [i] * s;
    }
}

#define __swap(x,y,t)     t = x; x = y; y = t;
void matrix_transpose (float* m) {
    float t;

    __swap (m [1], m [4], t);
    __swap (m [2], m [8], t);
    __swap (m [3], m [12], t);
    __swap (m [6], m [9], t);
    __swap (m [7], m [13], t);
    __swap (m [11], m [14], t);
}

void matrix_transpose_ex (float* a, const float* b) {
    a [0] = b [0];
    a [1] = b [4];
    a [2] = b [8];
    a [3] = b [12];
    
    a [4] = b [1];
    a [5] = b [5];
    a [6] = b [9];
    a [7] = b [13];
    
    a [8] = b [2];
    a [9] = b [6];
    a [10] = b [10];
    a [11] = b [14];
    
    a [12] = b [3];
    a [13] = b [7];
    a [14] = b [11];
    a [15] = b [15];
}

//  0  1  2  3
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15
void matrix_apply (const float* m, const float* in, float* out) {
    out [0] = m [0] * in [0] + m [4] * in [1] + m [8] * in [2] + m [12] * in [3];
    out [1] = m [1] * in [0] + m [5] * in [1] + m [9] * in [2] + m [13] * in [3];
    out [2] = m [2] * in [0] + m [6] * in [1] + m [10] * in [2] + m [14] * in [3];
    out [3] = m [3] * in [0] + m [7] * in [1] + m [11] * in [2] + m [15] * in [3];
}

static float __det3(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
    return (x1 * y2 * z3 + y1 * z2 * x3 + z1 * x2 * y3 - x3 * y2 * z1 - y3 * z2 * x1 - z3 * x2 * y1);
}

float matrix_determinant (const float* m) {
    return m [MATRIX_X1] * __det3 (m [MATRIX_Y2], m [MATRIX_Z2], m [MATRIX_W2], 
                                   m [MATRIX_Y3], m [MATRIX_Z3], m [MATRIX_W3], 
                                   m [MATRIX_Y4], m [MATRIX_Z4], m [MATRIX_W4])
         - m [MATRIX_Y1] * __det3 (m [MATRIX_X2], m [MATRIX_Z2], m [MATRIX_W2], 
                                   m [MATRIX_X3], m [MATRIX_Z3], m [MATRIX_W3], 
                                   m [MATRIX_X4], m [MATRIX_Z4], m [MATRIX_W4])
         + m [MATRIX_Z1] * __det3 (m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_W2], 
                                   m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_W3], 
                                   m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_W4])
         - m [MATRIX_W1] * __det3 (m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_Z2], 
                                   m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_Z3], 
                                   m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_Z4]);
}

void matrix_to_cofactored (float* a, const float* m) {
    a [MATRIX_X1] = __det3 (m [MATRIX_Y2], m [MATRIX_Z2], m [MATRIX_W2],
                            m [MATRIX_Y3], m [MATRIX_Z3], m [MATRIX_W3],
                            m [MATRIX_Y4], m [MATRIX_Z4], m [MATRIX_W4]);
    
    a [MATRIX_X2] = -__det3 (m [MATRIX_Y1], m [MATRIX_Z1], m [MATRIX_W1],
                            m [MATRIX_Y3], m [MATRIX_Z3], m [MATRIX_W3],
                            m [MATRIX_Y4], m [MATRIX_Z4], m [MATRIX_W4]);
    
    a [MATRIX_X3] = __det3 (m [MATRIX_Y1], m [MATRIX_Z1], m [MATRIX_W1],
                            m [MATRIX_Y2], m [MATRIX_Z2], m [MATRIX_W2],
                            m [MATRIX_Y4], m [MATRIX_Z4], m [MATRIX_W4]);
                            
    a [MATRIX_X4] = -__det3 (m [MATRIX_Y1], m [MATRIX_Z1], m [MATRIX_W1],
                            m [MATRIX_Y2], m [MATRIX_Z2], m [MATRIX_W2],
                            m [MATRIX_Y3], m [MATRIX_Z3], m [MATRIX_W3]);
     
                            
    a [MATRIX_Y1] = -__det3 (m [MATRIX_X2], m [MATRIX_Z2], m [MATRIX_W2],
                            m [MATRIX_X3], m [MATRIX_Z3], m [MATRIX_W3],
                            m [MATRIX_X4], m [MATRIX_Z4], m [MATRIX_W4]);
    
    a [MATRIX_Y2] = __det3 (m [MATRIX_X1], m [MATRIX_Z1], m [MATRIX_W1],
                            m [MATRIX_X3], m [MATRIX_Z3], m [MATRIX_W3],
                            m [MATRIX_X4], m [MATRIX_Z4], m [MATRIX_W4]);
    
    a [MATRIX_Y3] = -__det3 (m [MATRIX_X1], m [MATRIX_Z1], m [MATRIX_W1],
                            m [MATRIX_X2], m [MATRIX_Z2], m [MATRIX_W2],
                            m [MATRIX_X4], m [MATRIX_Z4], m [MATRIX_W4]);
                            
    a [MATRIX_Y4] = __det3 (m [MATRIX_X1], m [MATRIX_Z1], m [MATRIX_W1],
                            m [MATRIX_X2], m [MATRIX_Z2], m [MATRIX_W2],
                            m [MATRIX_X3], m [MATRIX_Z3], m [MATRIX_W3]);
     
                            
    a [MATRIX_Z1] = __det3 (m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_W2],
                            m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_W3],
                            m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_W4]);
    
    a [MATRIX_Z2] = -__det3 (m [MATRIX_X1], m [MATRIX_Y1], m [MATRIX_W1],
                            m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_W3],
                            m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_W4]);
    
    a [MATRIX_Z3] = __det3 (m [MATRIX_X1], m [MATRIX_Y1], m [MATRIX_W1],
                            m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_W2],
                            m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_W4]);
                            
    a [MATRIX_Z4] = -__det3 (m [MATRIX_X1], m [MATRIX_Y1], m [MATRIX_W1],
                            m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_W2],
                            m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_W3]);
       
                            
    a [MATRIX_W1] = -__det3 (m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_Z2],
                            m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_Z3],
                            m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_Z4]);
    
    a [MATRIX_W2] = __det3 (m [MATRIX_X1], m [MATRIX_Y1], m [MATRIX_Z1],
                            m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_Z3],
                            m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_Z4]);
    
    a [MATRIX_W3] = -__det3 (m [MATRIX_X1], m [MATRIX_Y1], m [MATRIX_Z1],
                            m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_Z2],
                            m [MATRIX_X4], m [MATRIX_Y4], m [MATRIX_Z4]);
                            
    a [MATRIX_W4] = __det3 (m [MATRIX_X1], m [MATRIX_Y1], m [MATRIX_Z1],
                            m [MATRIX_X2], m [MATRIX_Y2], m [MATRIX_Z2],
                            m [MATRIX_X3], m [MATRIX_Y3], m [MATRIX_Z3]);
}

int matrix_to_inverse (float* a, const float* b) {
    float t [16];
    
    float det = matrix_determinant (b);
    if (det == 0.0f) {
        return 1;
    }
    
    matrix_to_cofactored (t, b);
    matrix_transpose (t);
    matrix_scalar_mul_to (a, t, 1.0f / det);
    
    return 0;
}


#ifndef _matrix_h_
#define _matrix_h_

// Matrix components indices.
enum {
    MATRIX_X1 = 0, MATRIX_X2, MATRIX_X3, MATRIX_X4,
    MATRIX_Y1, MATRIX_Y2, MATRIX_Y3, MATRIX_Y4,
    MATRIX_Z1, MATRIX_Z2, MATRIX_Z3, MATRIX_Z4,
    MATRIX_W1, MATRIX_W2, MATRIX_W3, MATRIX_W4
};

extern float matrix_identity [16];


void matrix_copy (float* a, const float* b);


void matrix_init_identity (float* m);

void matrix_init_translation (float* m, float x, float y, float z);

void matrix_init_rotation (float* m, float ax, float ay, float az, float angle);

void matrix_init_scale (float* m, float sx, float sy, float sz);


void matrix_add (float* a, const float* b);

void matrix_add_to (float* a, const float* b, const float* c);

void matrix_mul_to (float* a, const float* b, const float* c);

void matrix_mul (float* a, const float* b);

void matrix_scalar_mul (float* m, float s);

void matrix_scalar_mul_to (float* a, const float* b, float s);


void matrix_transpose (float* m);

void matrix_transpose_ex (float* a, const float* b);


void matrix_apply (const float* m, const float* in, float* out);


float matrix_determinant (const float* m);

void matrix_to_cofactored (float* a, const float* m);

int matrix_to_inverse (float* a, const float* b);

#endif


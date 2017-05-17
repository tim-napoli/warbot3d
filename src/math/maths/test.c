#include "matrix.h"

#include <stdio.h>

void matrix_print (float* a) {
    printf ("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", 
            a [0], a [1], a [2], a [3], a [4], a [5], a [6], a [7], a [8], a [9], a [10], a [11], a [12], a [13], a [14], 
            a [15]
           );
}

void vector_print (const char* name, float* v) {
    printf ("%s : %f %f %f %f\n", name, v [0], v [1], v [2], v [3]);
}

int main (int argc, char** argv) {
    float a [16], b [16];
    float c [16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        2, 0, 2, 1
    };

    float det = 0.0f;
    float u [4] = {1.0f, 1.0f, 1.0f, 1.0f}, v [4];
    int i;
    
    matrix_init_rotation (c, 0, 1, 0, 2.14);
    
    matrix_print (c);
    vector_print ("u", u);
    matrix_apply (c, u, v);
    vector_print ("u'", v);
    
    for (i = 0; i < atoi (argv [1]); i++) {
        matrix_to_inverse (a, c);
    }
    
    matrix_apply (a, v, u);
    vector_print ("u''", u);
    
   // printf ("%f\n", det);

    return 0;
}


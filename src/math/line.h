#ifndef _line_h_
#define _line_h_

float line_pow_length (float ax, float az, float bx, float bz);

float line_length (float ax, float az, float bx, float bz);

int line_get_intersection (float Ax1, float Az1, float Ax2, float Az2,
                           float Bx1, float Bz1, float Bx2, float Bz2, 
                           float* xo, float* zo);

#endif


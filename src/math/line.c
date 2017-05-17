#include "math/line.h"

#include <math.h>
#include <stdlib.h>

float line_pow_length (float ax, float az, float bx, float bz) {
    float dx = bx - ax;
    float dz = bz - az;
    
    return dx * dx + dz * dz;
}

float line_length (float ax, float az, float bx, float bz) {
    float dx = bx - ax;
    float dz = bz - az;
    
    return sqrt (dx * dx + dz * dz);
}

int line_get_intersection (float Ax1, float Az1, float Ax2, float Az2,
                           float Bx1, float Bz1, float Bx2, float Bz2, 
                           float* xo, float* zo) {
                           
    float qx = Ax1,
          qz = Az1,
          sx = Ax2 - Ax1,
          sz = Az2 - Az1;
    
    float px = Bx1,
          pz = Bz1,
          rx = Bx2 - Bx1,
          rz = Bz2 - Bz1;
    
    float qpx = qx - px,
          qpz = qz - pz;
    float pqx = px - qx,
          pqz = pz - qz;
    float r_cross_s = rx * sz - rz * sx;
    float qp_cross_s = (qpx * sz - qpz * sx);
    float qp_cross_r = (qpx * rz - qpz * rx);
    
    float qp_dot_r = qpx * rx + qpz * rz,
          pq_dot_s = pqx * sx + pqz * sz;
          
    float t = 0, u = 0;
    
    if (r_cross_s == 0) {
        if (qp_cross_r != 0) {
            return 0;
        }
        else {
            float l = rx * rx + rz * rz;
            if (    (0 <= qp_dot_r && qp_dot_r <= l)
                ||  (0 <= pq_dot_s && pq_dot_s <= l)) {
                if (xo != NULL) *xo = -1;
                if (zo != NULL) *zo = -1;
                return 2;
            }
        }
        
        return 0;
    }
    else {
        t = qp_cross_s / r_cross_s;
        u = qp_cross_r / r_cross_s;
        
        if (t >= 0 && t <= 1.0 && u >= 0 && u <= 1.0) {
            if (xo != NULL) *xo = px + t * rx;
            if (zo != NULL) *zo = pz + t * rz;
            
            return 1;
        }
    }
    
    return 0;
}

#ifdef TEST

#define NITER   10000000

int main (int argc, char** argv) {
    size_t i, sum = 0;
    
    float x, z;
    
    for (i = 0; i < NITER + (i >> 1); i++) {
        sum += line_get_intersection (0, 0, 10, 10, 5, 5, -5, -5, &x, &z);
    }
    
    printf ("sum : %zu\n", sum);
    
    return 0;
}

#endif



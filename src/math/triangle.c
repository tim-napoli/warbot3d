#include "math/triangle.h"

#include <math.h>
#include <stdlib.h>

#define triangle_area(tx1,tz1,tx2,tz2,tx3,tz3)  \
    (0.5 * abs (tx1 * tz2 - tx1 * tz3 + tx2 * tz3 - tx2 * tz1 + tx3 * tz1 - tx3 * tz2))

int triangle_contains (float tx1, float tz1, float tx2, float tz2, float tx3, float tz3, float x, float y) {
    float abc = triangle_area (tx1, tz1, tx2, tz2, tx3, tz3),
          abo = triangle_area (tx1, tz1, tx2, tz2, x, y),
          bco = triangle_area (tx2, tz2, tx3, tz3, x, y),
          aco = triangle_area (tx1, tz1, tx3, tz3, x, y);
    
    return abc == abo + bco + aco;
}


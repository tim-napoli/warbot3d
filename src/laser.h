/* laser.h
 *  Managing laser rays, laser impacts on walls and sparks.
 */

#ifndef _laser_h_
#define _laser_h_

#include "math/vector.h"

// A laser ray.
typedef struct laser {
    vector_t start, stop;
    int      life;
    int r, g, b;
} laser_t;

//extern int      nlasers;
//extern laser_t  lasers [32];

void laser_add (float sx, float sy, float sz, float dx, float dy, float dz, int r, int g, int b);

void lasers_update ();

void lasers_render ();


// laser_impact:
//  A laser impact is an impact of a laser ray on a wall.
typedef struct laser_impact {
    vector_t    position;   // The position of the impact.
    vector_t    points [17];  // The direction of the impact (same as the wall).
    int r, g, b;
    int life;
} laser_impact_t;

//extern int nlaser_impacts;
//extern laser_impact_t impacts [128];

void laser_impact_add (float x, float y, float z, float dx, float dz, int r, int g, int b);

void laser_impacts_update ();

void laser_impacts_render ();


// laser_spark:
typedef struct laser_spark {
    vector_t positions [10];
    vector_t speeds [10];
    int r, g, b;
    int life;
} laser_spark_t;

void laser_spark_init (laser_spark_t* spark, float x, float y, float z, float dx, float dz, int r, int g, int b);

void laser_spark_update (laser_spark_t* spark);

void laser_spark_render (laser_spark_t* spark);

//extern int nlaser_sparks;
//extern laser_spark_t laser_sparks [128];

void laser_sparks_add (float x, float y, float z, float dx, float dz, int r, int g, int b);

void laser_sparks_update ();

void laser_sparks_render ();

#endif


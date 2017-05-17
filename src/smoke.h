#ifndef _smoke_h_
#define _smoke_h_

#define SMOKE_TEX_WIDTH     7
#define SMOKE_TEX_HEIGHT    7
#define SMOKE_TEX_SIZE      46

#define SMOKE_FRAME_TO_TEX_X(n)    \
    (n % SMOKE_TEX_WIDTH)
    
#define SMOKE_FRAME_TO_TEX_Y(n)    \
   (SMOKE_TEX_HEIGHT - n / SMOKE_TEX_WIDTH)

typedef struct smoke {
    float x, y, z;
    int ticker;
    int current_frame;
} smoke_t;

int smoke_init_module ();

void smoke_close_module ();


void smoke_add (float x, float y, float z);

void smokes_update ();

void smokes_render (float dx, float dz);


#endif


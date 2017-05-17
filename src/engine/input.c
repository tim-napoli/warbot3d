#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>

#include "engine/input.h"

/***************************************************** keyboard *******************************************************/
key_func_t key_binds [255]          = {0};
void* key_data [255]                = {NULL};

key_func_t key_special_binds [255]  = {0};
void* key_special_data [255]        = {NULL};

key_func_t key_special_up_binds [255]   = {0};
void* key_special_up_data [255]         = {NULL};

int  key_pressed [255] = {0};

void input_init (void) {
    memset (key_binds, 0, 255 * sizeof (key_func_t));
    memset (key_data, 0, 255 * sizeof (void*));
    memset (key_special_binds, 0, 255 * sizeof (key_func_t));
    memset (key_special_data, 0, 255 * sizeof (void*));
    memset (key_special_up_binds, 0, 255 * sizeof (key_func_t));
    memset (key_special_up_data, 0, 255 * sizeof (void*));
}

void keyboard_special_function (int key, int x, int y) {
    //printf ("Key : %d\n", key);
    
    key_pressed [key] = 1;
    
    if (key_special_binds [key] != NULL) {
        key_special_binds [key] (key_special_data [key]);
    }
}

void keyboard_special_up_function (int key, int x, int y) {
    key_pressed [key] = 0;

    //printf ("Key : %d\n", key);    
    if (key_special_up_binds [key] != NULL) {
        key_special_up_binds [key] (key_special_up_data [key]);
    }
}

void keyboard_function (unsigned char key, int x, int y) {
    if (key_binds [key] != NULL) {
        key_binds [key] (key_data [key]);
    }
}


void keyboard_register_bind (unsigned char key_code, key_func_t bind, void* data) {
    key_binds [key_code] = bind;
    key_data [key_code] = data;
}


void keyboard_unregister_bind (unsigned char key_code) {
    key_binds [key_code] = NULL;
    key_data [key_code] = NULL;
}

void keyboard_register_special_bind (int key_code, key_func_t bind, void* data) {
    key_special_binds [key_code] = bind;
    key_special_data [key_code] = data;
}

void keyboard_unregister_special_bind (int key_code) {
    key_special_binds [key_code] = NULL;
    key_special_data [key_code] = NULL;
}

void keyboard_register_special_up_bind (int key_code, key_func_t bind, void* data) {
    key_special_up_binds [key_code] = bind;
    key_special_up_data [key_code] = data;
}

void keyboard_unregister_special_up_bind (int key_code) {
    key_special_up_binds [key_code] = NULL;
    key_special_up_data [key_code] = NULL;
}

/****************************************************** mouse *********************************************************/
int                 num_mouse_move_binds                     = 0;
mouse_move_func_t   mouse_move_binds [MAX_MOUSE_FUNCTION]    = {NULL};
void*               mouse_move_data [MAX_MOUSE_FUNCTION]     = {NULL};
int                 num_mouse_click_binds                    = 0;
mouse_click_func_t  mouse_click_binds [MAX_MOUSE_FUNCTION]   = {NULL};
void*               mouse_click_data [MAX_MOUSE_FUNCTION]    = {NULL};
int                 num_mouse_release_binds                  = 0;
mouse_click_func_t  mouse_release_binds [MAX_MOUSE_FUNCTION] = {NULL};
void*               mouse_release_data [MAX_MOUSE_FUNCTION]  = {NULL};
int                 mouse_last_x = 0, mouse_last_y = 0;

void mouse_move_function (int x, int y) {
    int i;
    static int count = 0;
    
    for (i = 0; i < num_mouse_move_binds; i++) {
        mouse_move_binds [i] (mouse_move_data [i], x, y, x - mouse_last_x, y - mouse_last_y);
    }
    
    if (count == 0) {
        glutWarpPointer (400, 300);
        mouse_last_x = 400;
        mouse_last_y = 300;
    }
    else {
        mouse_last_x = x;
        mouse_last_y = y;
    }
    
    count = (count + 1) % 2;
}

void mouse_click_function (int button, int state, int x, int y) {
    int i;
    
    if (state == GLUT_DOWN) {
        for (i = 0; i < num_mouse_click_binds; i++) {
            mouse_click_binds [i] (mouse_click_data [i], button, x, y);
        }
    }
    else {
        for (i = 0; i < num_mouse_release_binds; i++) {
            mouse_release_binds [i] (mouse_release_data [i], button, x, y);
        }
    }
}


void mouse_move_register_bind (mouse_move_func_t function, void* data) {
    if (num_mouse_move_binds < MAX_MOUSE_FUNCTION) {
        mouse_move_binds [num_mouse_move_binds] = function;
        mouse_move_data [num_mouse_move_binds++] = data;
    }
    else {
        fprintf (stderr, "mouse_move_register_bind : Cannot register more binds.\n");
    }
}

void mouse_move_unregister_bind (mouse_move_func_t function, void* data) {
    int i;
    
    for (i = 0; i < num_mouse_move_binds; i++) {
        if (mouse_move_binds [i] == function && mouse_move_data [i] == data) {
            mouse_move_binds [i] = mouse_move_binds [num_mouse_move_binds - 1];
            mouse_move_data [i] = mouse_move_data [num_mouse_move_binds - 1];
            mouse_move_binds [num_mouse_move_binds - 1] = NULL;
            mouse_move_data [num_mouse_move_binds - 1] = NULL;
            num_mouse_move_binds--;
        }
    }
}

void mouse_click_register_bind (mouse_click_func_t function, void* data) {
    if (num_mouse_click_binds < MAX_MOUSE_FUNCTION) {
        mouse_click_binds [num_mouse_click_binds] = function;
        mouse_click_data [num_mouse_click_binds++] = data;
    }
    else {
        fprintf (stderr, "mouse_click_register_bind : Cannot register more binds.\n");
    }
}

void mouse_click_unregister_bind (mouse_click_func_t function, void* data) {
    int i;
    
    for (i = 0; i < num_mouse_click_binds; i++) {
        if (mouse_click_binds [i] == function && mouse_click_data [i] == data) {
            mouse_click_binds [i] = mouse_click_binds [num_mouse_click_binds - 1];
            mouse_click_data [i] = mouse_click_data [num_mouse_click_binds - 1];
            mouse_click_binds [num_mouse_click_binds - 1] = NULL;
            mouse_click_data [num_mouse_click_binds - 1] = NULL;
            num_mouse_click_binds--;
        }
    }
}

void mouse_release_register_bind (mouse_click_func_t function, void* data) {
    if (num_mouse_release_binds < MAX_MOUSE_FUNCTION) {
        mouse_release_binds [num_mouse_release_binds] = function;
        mouse_release_data [num_mouse_release_binds++] = data;
    }
    else {
        fprintf (stderr, "mouse_release_register_bind : Cannot register more binds.\n");
    }
}

void mouse_release_unregister_bind (mouse_click_func_t function, void* data) {
    int i;
    
    for (i = 0; i < num_mouse_release_binds; i++) {
        if (mouse_release_binds [i] == function && mouse_release_data [i] == data) {
            mouse_release_binds [i] = mouse_release_binds [num_mouse_release_binds - 1];
            mouse_release_data [i] = mouse_release_data [num_mouse_release_binds - 1];
            mouse_release_binds [num_mouse_release_binds - 1] = NULL;
            mouse_release_data [num_mouse_release_binds - 1] = NULL;
            num_mouse_release_binds--;
        }
    }
}



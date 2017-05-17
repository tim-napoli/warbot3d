/* input.h
 *  Binding input events to functions.
 */

#ifndef _input_h_
#define _input_h_

void input_init (void);

/***************************************************** keyboard *******************************************************/

typedef enum {
    // Special
    KEYC_RETURN = 13,
    KEYC_ESCAPE = 27,
    KEYC_SPACE = 32,
    // Numeric
    KEYC_0 = 48,
    KEYC_1, KEYC_2, KEYC_3, KEYC_4, KEYC_5, KEYC_6, KEYC_7, KEYC_8, KEYC_9,
    // Alpha
    KEYC_A = 97,
    KEYC_B, KEYC_C, KEYC_D, KEYC_E, KEYC_F, KEYC_G, KEYC_H, KEYC_I, KEYC_J, KEYC_K, KEYC_L, KEYC_M, KEYC_N, KEYC_O, 
    KEYC_P, KEYC_Q, KEYC_R, KEYC_S, KEYC_T, KEYC_U, KEYC_V, KEYC_W, KEYC_X, KEYC_Y, KEYC_Z
} keycode_t;

// A keyboard bind is function assigned to a key number.
typedef void (*key_func_t) (void*);

extern int key_pressed [255];

void keyboard_function (unsigned char key, int x, int y);

void keyboard_special_function (int key, int x, int y);

void keyboard_special_up_function (int key, int x, int y);

void keyboard_register_bind (unsigned char key_code, key_func_t bind, void* data);

void keyboard_unregister_bind (unsigned char key_code);

void keyboard_register_special_bind (int key_code, key_func_t bind, void* data);

void keyboard_unregister_special_bind (int key_code);

void keyboard_register_special_up_bind (int key_code, key_func_t bind, void* data);

void keyboard_unregister_special_up_bind (int key_code);



/****************************************************** mouse *********************************************************/

// A mouse_move_func_t is a function which can be called when the mouse move (arguments are x, y, diff_x, diff_y).
typedef void (*mouse_move_func_t) (void*, int, int, int, int);

// A mouse_click_func_t is a function called when the user click a button (arguments are x, y, button).
typedef void (*mouse_click_func_t) (void*, int, int, int);

#define MAX_MOUSE_FUNCTION  32

extern  int                 num_mouse_move_binds;
extern  mouse_move_func_t   mouse_move_binds [MAX_MOUSE_FUNCTION];
extern  void*               mouse_move_data [MAX_MOUSE_FUNCTION];
extern  int                 num_mouse_click_binds;
extern  mouse_click_func_t  mouse_click_binds [MAX_MOUSE_FUNCTION];
extern  void*               mouse_click_data [MAX_MOUSE_FUNCTION];
extern  mouse_click_func_t  mouse_release_binds [MAX_MOUSE_FUNCTION];
extern  void*               mouse_release_data [MAX_MOUSE_FUNCTION];
extern  int                 mouse_last_x, mouse_last_y;

void mouse_move_function (int x, int y);

void mouse_click_function (int button, int state, int x, int y);

void mouse_move_register_bind (mouse_move_func_t function, void* data);

void mouse_move_unregister_bind (mouse_move_func_t function, void* data);

void mouse_click_register_bind (mouse_click_func_t function, void* data);

void mouse_click_unregister_bind (mouse_click_func_t function, void* data);

void mouse_release_register_bind (mouse_click_func_t function, void* data);

void mouse_release_unregister_bind (mouse_click_func_t function, void* data);

#endif


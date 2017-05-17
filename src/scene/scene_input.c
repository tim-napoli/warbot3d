#include "scene/scene_input.h"
#include "engine/input.h"
#include "math/matrix.h"

#include <GL/glut.h>

static void robot_control_forward (robot_t* robot) {
    // Calcul du vecteur de direction
    robot->sx = robot->dx;
    robot->sz = robot->dz;
    
    robot_request_action (robot, ROBOT_ACTION_WALK);
}

static void robot_control_left (robot_t* robot) {
    // Calcul du vecteur de direction
    vector_t    d = {0.0, 0.0, 1.0};
    vector_t    a = {0.0, 1.0, 0.0};
    matrix_t    R;
    
    matrix_init_rotation (&R, &a, ((robot->pelvis->angles [1] + 90) * 3.14159265) / 180);
    matrix_apply_ex (&R, &d);
    
    robot->sx = d.x;
    robot->sz = d.z;
    
    robot_request_action (robot, ROBOT_ACTION_WALK);
}

static void robot_control_right (robot_t* robot) {
    vector_t    d = {0.0, 0.0, 1.0};
    vector_t    a = {0.0, 1.0, 0.0};
    matrix_t    R;
    
    matrix_init_rotation (&R, &a, ((robot->pelvis->angles [1] - 90) * 3.14159265) / 180);
    matrix_apply_ex (&R, &d);
    
    robot->sx = d.x;
    robot->sz = d.z;
    
    robot_request_action (robot, ROBOT_ACTION_WALK);
}


static void robot_control_backward (robot_t* robot) {
    // Calcul du vecteur de direction
    vector_t    d = {0.0, 0.0, -1.0};
    vector_t    a = {0.0, 1.0, 0.0};
    matrix_t    R;
    
    matrix_init_rotation (&R, &a, ((robot->pelvis->angles [1]) * 3.14159265) / 180);
    matrix_apply_ex (&R, &d);
    
    robot->sx = d.x;
    robot->sz = d.z;
    
    robot_request_action (robot, ROBOT_ACTION_WALK);
}


void robot_control_move (robot_t* robot) {
    if (robot->action != ROBOT_ACTION_DIE) {
        if (key_pressed [GLUT_KEY_UP]) {
            robot_control_forward (robot);
            robot->repeat_walk = 1;
        }
        else if (key_pressed [GLUT_KEY_LEFT]) {
            robot_control_left (robot);
            robot->repeat_walk = 1;
        }
        else if (key_pressed [GLUT_KEY_RIGHT]) {
            robot_control_right (robot);
            robot->repeat_walk = 1;
        }
        else if (key_pressed [GLUT_KEY_DOWN]) {
            robot_control_backward (robot);
            robot->repeat_walk = 1;
        }
        else {
            if (robot->action == ROBOT_ACTION_WALK) robot_request_action (robot, ROBOT_ACTION_WAIT);
            robot->repeat_walk = 0;
        }
    }
}

void robot_control_rotate (robot_t* robot, int x, int y, int dx, int dy) {
    if (robot->action != ROBOT_ACTION_DIE) {
        robot_rotate_of (robot,  -dx * 0.01);
   }
}


void robot_control_start_shoot (robot_t* robot, int x, int y) {
    if (robot->action != ROBOT_ACTION_DIE) {
        robot_request_action (robot, ROBOT_ACTION_SHOOT);
        robot->repeat_shoot = 1;
    }
}

void robot_control_stop_shoot (robot_t* robot, int x, int y) {
    robot->repeat_shoot = 0;
}


void camera_move_up (camera_t* camera) {
    node_t* node = level_node_at (&scene_level, camera->eye.x, camera->eye.z);
    if (camera->eye.y + 3 < node->sector->height) {
        camera->eye.y += 3;
        camera->center.y += 3;
    }
}

void camera_move_down (camera_t* camera) {
    if (camera->eye.y > 3) {
        camera->eye.y -= 3;
        camera->center.y -= 3;
    }
}

void camera_move (camera_t* camera) {
    if (key_pressed [GLUT_KEY_UP]) {
        camera_move_of (camera, 3, 0, 0);
    }
    if (key_pressed [GLUT_KEY_LEFT]) {
        camera_move_of (camera, 0, 0, -3);
    }
    if (key_pressed [GLUT_KEY_RIGHT]) {
        camera_move_of (camera, 0, 0, 3);
    }
    if (key_pressed [GLUT_KEY_DOWN]) {
        camera_move_of (camera, -3, 0, 0);
    }
    if (key_pressed [GLUT_KEY_PAGE_UP]) {
        camera_move_up (camera);
    }
    if (key_pressed [GLUT_KEY_PAGE_DOWN]) {
        camera_move_down (camera);
    }
}

void camera_turn (camera_t* camera, int x, int y, int dx, int dy) {
    if (dx != 0) camera_rotate_y (camera, -dx * 0.01);
    if (dy != 0) camera_rotate_x (camera, -dy * 0.01);
}

void switch_fullscreen (void* dat) {
    if (fullscreen == 0) {
        fullscreen = 1;
        glutFullScreen ();
    }
    else {
        fullscreen = 0;
        glutReshapeWindow (800, 600);
    }
}


static void __set_robot_control () {
    //keyboard_register_special_bind (GLUT_KEY_UP, (key_func_t) robot_control_forward, (void*) good_robot);
    //keyboard_register_special_bind (GLUT_KEY_DOWN, (key_func_t) robot_control_backward, (void*) good_robot);
    //keyboard_register_special_bind (GLUT_KEY_LEFT, (key_func_t) robot_control_left, (void*) good_robot);
    //keyboard_register_special_bind (GLUT_KEY_RIGHT, (key_func_t) robot_control_right, (void*) good_robot);
    
    //keyboard_register_special_up_bind (GLUT_KEY_UP, (key_func_t) robot_stop_walk, (void*) good_robot);
    
    mouse_move_register_bind ((mouse_move_func_t) robot_control_rotate, (void*) controlled_robot);
    mouse_click_register_bind ((mouse_click_func_t) robot_control_start_shoot, (void*) controlled_robot);
    mouse_release_register_bind ((mouse_click_func_t) robot_control_stop_shoot, (void*) controlled_robot);
}

static void __unset_robot_control () {
    //keyboard_unregister_special_bind (GLUT_KEY_UP);
    //keyboard_unregister_special_bind (GLUT_KEY_DOWN);
    //keyboard_unregister_special_bind (GLUT_KEY_LEFT);
    //keyboard_unregister_special_bind (GLUT_KEY_RIGHT);
    
    mouse_move_unregister_bind ((mouse_move_func_t) robot_control_rotate, (void*) controlled_robot);
    mouse_click_unregister_bind ((mouse_click_func_t) robot_control_start_shoot, (void*) controlled_robot);
    mouse_release_unregister_bind ((mouse_click_func_t) robot_control_stop_shoot, (void*) controlled_robot);
}


void set_camera_control () {
    mouse_move_register_bind ((mouse_move_func_t) camera_turn, (void*) &scene_camera);
}

void unset_camera_control () {
   mouse_move_unregister_bind ((mouse_move_func_t) camera_turn, (void*) &scene_camera);
}

void switch_freelook (void* dat) {
    static int current = 0;
    if (controlled_robot == NULL) {
        controlled_robot = robots [current];
        unset_camera_control ();
        __set_robot_control ();
        if (controlled_robot->action != ROBOT_ACTION_DIE) {
            robot_request_action (controlled_robot, ROBOT_ACTION_WAIT);
        }
    }
    else if (current == 2) {
        if (controlled_robot->action != ROBOT_ACTION_DIE) {
            robot_request_action (controlled_robot, ROBOT_ACTION_WAIT);
        }
    
        current = 0;
        controlled_robot = NULL;
        set_camera_control ();
    }
    else {
        __unset_robot_control ();
        if (controlled_robot->action != ROBOT_ACTION_DIE) {
            robot_request_action (controlled_robot, ROBOT_ACTION_WAIT);
        }
        
        current++;
        controlled_robot = robots [current];
        
        __set_robot_control ();
        if (controlled_robot->action != ROBOT_ACTION_DIE) {
            robot_request_action (controlled_robot, ROBOT_ACTION_WAIT);
        }
    }
}


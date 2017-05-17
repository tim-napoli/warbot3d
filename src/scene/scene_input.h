#ifndef _scene_input_h_
#define _scene_input_h_

#include "scene/scene_data.h"

// Handling robot moves if one is controlled.
void robot_control_move (robot_t* robot);

void camera_move_up (camera_t* camera);

void camera_move_down (camera_t* camera);

void camera_move (camera_t* camera);

void camera_turn (camera_t* camera, int x, int y, int dx, int dy);

void switch_fullscreen (void* dat);

void set_camera_control ();

void unset_camera_control ();

void switch_freelook (void* dat);

#endif

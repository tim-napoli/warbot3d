#ifndef _scene_data_h_
#define _scene_data_h_

#include "scene/level.h"
#include "scene/camera.h"
#include "robot.h"
#include "engine/font.h"

// The camera used through the game.
extern camera_t    scene_camera;

extern int freelook;

// Set to 1 if the video mode is fullscreen, 0 else.
extern int fullscreen;

// The level used through the game.
extern level_t scene_level;

// The font used through the game.
extern font_t* font;

// A basic shader just rendering colors.
extern shader_t* base_shader;

// Robots used in the game.
enum {
    blue_robot = 0,
    orange_robot,
    green_robot,
    number_of_robot
};

extern robot_t* robots [3];

// Currently controlled robot. NULL if no one.
extern robot_t* controlled_robot;

// Shader quality.
enum {
    shader_level_low = 0,
    shader_level_medium,
    shader_level_high
};

extern int shader_level;

#endif


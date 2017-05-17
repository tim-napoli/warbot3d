/* robot.h
 *  Managing a robot.
 */

#ifndef _robot_h_
#define _robot_h_

#include "3d/mesh.h"
#include "3d/rig.h"
#include "math/vector.h"
#include "engine/bitmap.h"
#include "engine/audio.h"
#include "3d/light.h"
#include "3d/shader.h"

#include "scene/level.h"

enum {
    robot_color_blue = 0,
    robot_color_orange,
    robot_color_green,
    robot_color_max
};

extern GLuint robot_textures [robot_color_max];

extern vbo_mesh_t*  robot_vbo_mesh;

enum {
    robot_sound_laser1 = 0,
    robot_sound_laser2,
    robot_sound_laser3,
    robot_sound_laser4,
    robot_sound_laser5,
    robot_sound_laser6,
    robot_sound_die,
    robot_sound_reborn,
    robot_sound_max
};

extern shader_t* robot_shader;

// See what is a struct robot below;
typedef struct robot robot_t;

// A robot has animations (modification of the robot rig (or skeleton) across a time). An animation is associated with
// a robot_action_function (see below).
typedef int (*robot_animation_t) (robot_t*);

// A robot has actions. An action function is a function which will handle the robot during an animation (exemple : 
// updating robot position during walk).
typedef int (*robot_action_t) (robot_t*);

// Different actions of a robot.
enum {
    ROBOT_ACTION_WAIT = 0,
    ROBOT_ACTION_WALK,
    ROBOT_ACTION_SHOOT,
    ROBOT_ACTION_DIE,
    ROBOT_ACTION_SIZE
};

// The robot speed.
#define ROBOT_SPEED 1.0f

// The number of other robots.
#define ROBOT_NUM_OTHERS    2

// Data attached to an action (too heavy for how much its useful)
typedef struct robot_action_data {
    vector_t v [4];
    int      i [10];
} robot_action_data_t;

// The big robot_t structure.
struct robot {
    vbo_rig_t *pelvis,              // Is the root node of the skeleton.
              *right_upper_leg,
              *left_knee,
              *pincher1,
              *pincher2,
              *head,
              *right_foot,
              *left_upper_leg,
              *left_foot,
              *right_knee,
              *left_pincher2,
              *left_pincher1,
              *blaster,
              *left_elbow,
              *right_elbow,
              *right_shoulder,
              *left_shoulder,
              *torso;
    
    GLuint texture;
    
    int                     action;
    robot_action_t          actions [ROBOT_ACTION_SIZE];
    robot_animation_t       animations [ROBOT_ACTION_SIZE];
    int                     ticker;
    
    
    // Actions special data.
    int repeat_shoot;
    int repeat_walk;
    float dx, dz;
    float sx, sz;
    struct robot* target;
    

    light_t                 light;
    
    int life;
    int damages;
    
    int points;
    
    ALuint audio_source;
    
    // For the AI
    node_t* current_node;
    node_path_t* goal;
};

// Initialize the module (loading mesh & texture). Return 1 if something is wrong.
int robot_init_module ();

// Close the robot module.
void robot_close_module ();

//----------------------------------------------------- General ------------------------------------------------------//
// Create a new robot.
robot_t* robot_new (float x, float y, float z, float ay, GLuint texture, light_t* light);

// Delete a created robot.
void robot_delete (robot_t* robot);

// Render a robot.
void robot_render (robot_t* robot);

// Updating a robot.
//  Will play current action and animation of the robot.
//  Loop animation if its fucntion return true.
//  Go to ROBOT_ACTION_WAIT action if the action function return true. 
void robot_update (robot_t* robot);



//----------------------------------------------- Robot manipulation -------------------------------------------------//
// Store the plane position of the robot in *x and *z.
void robot_get_position (robot_t* robot, float* x, float* z);

// Rotate the robot to make him looking the given point.
void robot_rotate_to (robot_t* robot, float x, float z);

// Rotate the robot of the specified angle.
void robot_rotate_of (robot_t* robot, float angle);

// Set rig angles and position to default.
void robot_reset_rig (robot_t* robot);

// Request a new action to play for the robot.
void robot_request_action (robot_t* robot, int action);

// Check if the line {(x1, y1), (x2, y2)} is intersecting the robot cylinder. 
// Return 1 if yes. In this case, xo and zo get the intersection point coordinates. If there is no intersection, xo and
// zo are undefined. 
int robot_has_intersection (robot_t* robot, float x1, float z1, float x2, float z2, float* xo, float* zo);


//------------------------------------------------- Game functions ---------------------------------------------------//
// Respawn a robot (choose a random node and put him it with default values).
void robot_respawn (robot_t* robot);

// Update the current node of a robot (set the current node as the nearest of each link and the current node itself).
void robot_update_node (robot_t* robot);

// Check if it exists a point of view to target an other robot.
robot_t* robot_find_target (robot_t* robot);

// Handling shoot (create lasers, impacts, sparks, smoke, update points & life, play laser sound).
void robot_shoot (robot_t* robot);

//---------------------------------------------------- Robot AI ------------------------------------------------------//
// How the computer play a robot.
void robot_ai (robot_t* robot);


//---------------------------------------------------- Actions -------------------------------------------------------//
// Wait
int robot_action_wait (robot_t* robot);

int robot_animation_wait (robot_t* robot);

// Walk .
void robot_stop_walk (robot_t* robot);

int robot_action_walk (robot_t* robot);

int robot_animation_walk (robot_t* robot);

// Shoot animation.
void robot_play_laser_sound (robot_t* robot);

int robot_action_shoot (robot_t* robot);

int robot_animation_shoot (robot_t*);

// Die animation.
int robot_action_die (robot_t* robot);

int robot_animation_die (robot_t*);



#endif


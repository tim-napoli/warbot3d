#include "scene/level.h"
#include "engine/display.h"
#include "scene.h"
#include "scene/camera.h"
#include "3d/skybox.h"
#include "engine/input.h"
#include "robot.h"
#include "scene/scene_data.h"
#include "laser.h"
#include "3d/shader.h"
#include "engine/font.h"
#include "smoke.h"

#include "math/matrix.h"

#include <stdio.h>
#include <GL/glut.h>

//--------------------------------------------------------------------------------------------------------------------//
// Globals
//--------------------------------------------------------------------------------------------------------------------//

camera_t    scene_camera;
int freelook = 1;
int fullscreen = 0;

level_t scene_level = {0};

skybox_t scene_skybox;

robot_t* robots [3] = {NULL};
robot_t* controlled_robot = NULL;

shader_t* base_shader = NULL;
shader_t* laser_shader = NULL;
int shader_level = shader_level_high;

font_t* font = NULL;

//--------------------------------------------------------------------------------------------------------------------//
// Inputs
//--------------------------------------------------------------------------------------------------------------------//
#include "scene/scene_input.h"

//--------------------------------------------------------------------------------------------------------------------//
// Initialization
//--------------------------------------------------------------------------------------------------------------------//
#include "scene/level_creation.h"

void scene_init (int argc, char** argv) {
    printf ("OpenGL initialization...\n");
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_TEXTURE_2D);
    glEnable (GL_COLOR_MATERIAL);
    glEnable (GL_LIGHTING);
    glEnable(GL_MULTISAMPLE);
    glEnable (GL_PROGRAM_POINT_SIZE);
    glEnable (GL_CULL_FACE);
    glutSetCursor (GLUT_CURSOR_NONE);

    glClearColor (0.7f, 0.7f, 0.7f, 1.0f);

    int display_nodes = 0;
    int i;
    for (i = 1; i < argc; i++) {
        if (sscanf (argv [i], "-display_nodes=%d\n", &display_nodes) == 1) {
        
        }
        else if (sscanf (argv [i], "-shader_level=%d\n", &shader_level) == 1) {
        
        }
    }

    printf ("Level creation...\n");
    if (level_init_module (display_nodes)) {
        fprintf (stderr, "scene_init error : Cannot initialize level module.\n");
        exit (1);
    }
    
    level_creation (&scene_level);
    level_to_vbo (&scene_level);
    
    skybox_init (&scene_skybox, "data/skybox.bmp");
    
    printf ("Robot creation...\n");
    if (robot_init_module ()) {
        fprintf (stderr, "scene_init error : cannot initialize the robot module.\n");
        exit (1);
    }
    
    light_t light;
    
    light_init (&light, GL_LIGHT0,
                0, 0, 0, 0,
                0.0, 0.0, 0.5,
                0, 0, 0,
                0.25, 0.5, 1.0,
                0.002, 0.01);
    robots [blue_robot] = robot_new (0, 9.2, 10, 0, robot_textures [robot_color_blue], &light);
    robot_respawn (robots [blue_robot]);
    
    light_init (&light, GL_LIGHT1,
                0, 0, 0, 0,
                0.5, 0.25, 0.0,
                0, 0, 0,
                1.0, 0.5, 0.25,
                0.002, 0.01);
    robots [orange_robot] = robot_new (0, 9.2, 10, 0, robot_textures [robot_color_orange], &light);
    robot_respawn (robots [orange_robot]);
    
    light_init (&light, GL_LIGHT2,
                0, 0, 0, 0,
                0.0, 0.5, 0.0,
                0, 0, 0,
                0.25, 1.0, 0.5,
                0.002, 0.01);
    robots [green_robot] = robot_new (0, 9.2, 10, 0, robot_textures [robot_color_green], &light);
    robot_respawn (robots [green_robot]);
    
    
    /*
    for (i = 0; i < number_of_robots; i++) {
        robots [i] = robot_new (0, 10, 0, 0, robot_texture, ...);
    }
    */
    
    camera_init (&scene_camera, &scene_level,
                 0, 10, 0, 
                 0, 10, 10,
                 0, -1, 0);
    set_camera_control ();
    
    keyboard_register_bind ('f', switch_fullscreen, NULL);
    keyboard_register_bind ('\t', switch_freelook, NULL);
    
    base_shader = shader_load ("data/shaders/base.vert", "data/shaders/base.frag");
    if (base_shader == NULL) {
        fprintf (stderr, "scene_init error : Cannot load the base shader.\n");
        exit (1);
    }
    
    laser_shader = shader_load ("data/shaders/laser.vert", "data/shaders/laser.frag");
    if (laser_shader == NULL) {
        fprintf (stderr, "scene_init error : cannot laod the laser shader.\n");
        exit (1);
    }
    
    font = font_load ("data/font.bmp", 5, 12);
    if (font == NULL) {
        fprintf (stderr, "scene_init error : Cannot laod font.\n");
        exit (1);
    }
    
    if (smoke_init_module ()) {
        fprintf (stderr, "scene_init error : Cannot open smoke module.\n");
        exit (1);
    }
}

//--------------------------------------------------------------------------------------------------------------------//
// Updating
//--------------------------------------------------------------------------------------------------------------------//
void scene_update (void) {
    if (controlled_robot == NULL) {
        //robot_ai (good_robot);
        camera_move (&scene_camera);
    }
    else {
        robot_control_move (controlled_robot);
    }
    
    size_t i;
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != NULL) {
            robot_update (robots [i]);
            if (robots [i] != controlled_robot) robot_ai (robots [i]);
        }
    }
    
    lasers_update ();
    laser_impacts_update ();
    laser_sparks_update ();
    
    smokes_update ();
    
    /*
    vector_t axe = {0.0, 0.0, 1.0, 0.0};
    matrix_t R;
    matrix_init_rotation (&R, &axe, 3.14159265 * 0.001);
    vector_t cpos;
    vector_copy (&cpos, &scene_level.light.position);
    matrix_apply (&R, &cpos, &scene_level.light.position);
    */
}

//--------------------------------------------------------------------------------------------------------------------//
// Rendering
//--------------------------------------------------------------------------------------------------------------------//
#include "scene/scene_rendering.h"
#include <sys/time.h>

static long nticks = 0;
static long render_time_sum = 0; 

void scene_render (void) {
    
    size_t i;
    struct timeval tbeg, tend;
    gettimeofday (&tbeg, NULL);

    if (!display_use_shader) {
        for (i = 0; i < number_of_robot; i++) {
            light_update_position (&robots [i]->light);
        }
    }

    glLoadIdentity ();
    
    update_camera ();
    
    glColor3ub (255, 255, 255);
    
    glPushMatrix ();
    glTranslatef (scene_camera.eye.x, scene_camera.eye.y, scene_camera.eye.z);
    skybox_render (&scene_skybox);
    glPopMatrix ();
    
    vector_t eye_dir;
    vector_sub_ex (&eye_dir, &scene_camera.center, &scene_camera.eye);
    level_render (&scene_level, scene_camera.eye.x, scene_camera.eye.y, scene_camera.eye.z,
                                eye_dir.x, eye_dir.y, eye_dir.z);
    
    shader_select (robot_shader);
    for (i = 0; i < 3; i++) {
        if (robots [i] != NULL) {
            shader_update_light_world_position (robot_shader, &robots [i]->light);
        }
    }
    shader_update_light_world_position (robot_shader, &scene_level.light);
    shader_send_uniforms (robot_shader, shader_send_lights_wpos);
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != NULL) {
            robot_render (robots [i]);
        }
    }
    shader_release ();
    

    
    shader_select (laser_shader);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    lasers_render ();
    laser_impacts_render ();
    laser_sparks_render ();
    glDisable (GL_BLEND);
    shader_release ();
    
    smokes_render (scene_camera.eye.x, scene_camera.eye.z);
    
    hud_render ();
    
    
    glLoadIdentity ();
    
    gettimeofday (&tend, NULL);
    //printf ("Temps de rendu (µs) : %d\n", tend.tv_usec - tbeg.tv_usec);
    if (nticks > 0) render_time_sum += (tend.tv_sec - tbeg.tv_sec) * 1000000 + tend.tv_usec - tbeg.tv_usec;
    nticks++;
    
#ifdef PROFILING
    if (nticks == 1000) exit (0);
#endif
}

//--------------------------------------------------------------------------------------------------------------------//
// Unitialization
//--------------------------------------------------------------------------------------------------------------------//
void scene_deinit (void) {
    size_t i;
    
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != NULL) robot_delete (robots [i]);
    }
    
    shader_delete (base_shader);
    shader_delete (laser_shader);
    font_delete (font);

    skybox_deinit (&scene_skybox);
    robot_close_module ();
    level_deinit_module ();
    smoke_close_module ();
    
    printf ("Average rendering time : %ldµs\n", render_time_sum / nticks);
    printf ("Good bye !\n");
}


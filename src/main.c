#include "engine/display.h"
#include "engine/input.h"
#include "engine/audio.h"

#include "scene.h"

#include <stdio.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glut.h>

void update (int v) {
    if (glutGetWindow () != 0) {
        scene_update ();
        glutPostRedisplay ();
        glutTimerFunc (16, update, 0);
    }   
}

void init (int argc, char** argv) {
    glutInit (&argc, argv);
    input_init ();
    display_init (argv [0], 1024, 768, scene_render, argc, argv);
    
    glutIgnoreKeyRepeat (1);
    glutKeyboardFunc (keyboard_function);
    glutSpecialFunc (keyboard_special_function);
    glutSpecialUpFunc (keyboard_special_up_function);
    glutMotionFunc (mouse_move_function);
    glutPassiveMotionFunc (mouse_move_function);
    glutMouseFunc (mouse_click_function);
    glutReshapeFunc (display_reshape);
    glutDisplayFunc (display_update);
    glutTimerFunc (16, update, 0);
    //glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    //atexit (display_close);
    
    // Setting the mouse position
    glutWarpPointer (400, 300);
    
    
    // Audio
    audio_init ();
    atexit (audio_close);
    
    
    
    // Reading seed.
    unsigned long t = time (NULL);
    int i;
    for (i = 1; i < argc; i++) {
        sscanf (argv [i], "-seed=%lu", &t);
    }
    printf ("Using seed %lu\n", t);
    srand ((time_t) t);
    
    scene_init (argc, argv);
    
    atexit (scene_deinit);
    glutMainLoop ();
    //scene_deinit ();
}

#include <math.h>

int main (int argc, char** argv) {

    init (argc, argv);

    return 0;
}


#include <GL/glew.h>
#include "engine/display.h"
#include "engine/input.h"

#include <GL/glut.h>
#include <stdio.h>

int window_id;

int display_width, display_height;
display_rendering_func_t    rendering_function;

int display_use_shader = 1;

static void __handle_command_line (int argc, char** argv, int* width, int* height) {
    int i;
    
    for (i = 1; i < argc; i++) {
        if (sscanf (argv [i], "-shader=%d", &display_use_shader) == 1) {
           
        }
        else if (sscanf (argv [i], "-width=%d", width) == 1) {
        
        }
        else if (sscanf (argv [i], "-height=%d", height) == 1) {
        
        }
    }
}

void display_init (const char* name, int width, int height, display_rendering_func_t rf,
                   int argc, char** argv) {
    __handle_command_line (argc, argv, &width, &height);               
    
    display_width = width;
    display_height = height;

    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize (width, height);
    window_id = glutCreateWindow (name);
    display_reshape (width, height);
    
    keyboard_register_bind (KEYC_ESCAPE, (void (*) (void*)) display_close, 0);
    
    GLenum err = glewInit ();
    if (err != GLEW_OK) {
        fprintf (stderr, "display init error : cannot load GLEW : %s", glewGetErrorString (err));
    }
    
    rendering_function = rf;
}

void display_reshape (int width, int height) {
    display_width = width;
    display_height = height;

    glViewport (0, 0, width, height);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (55, width / (float) height, 0.1f, 1000.0f);
    
    glMatrixMode (GL_MODELVIEW);
}

void display_close () {
    glutDestroyWindow (window_id);
}

void display_update () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    
    rendering_function ();
    
    glutSwapBuffers ();
}


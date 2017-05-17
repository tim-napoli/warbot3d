/* display.h
 *  Managing the display (glut wrapper).
 */

#ifndef _display_h_
#define _display_h_

#include <GL/gl.h>

// The window identifier.
extern int window_id;

// The window size (might be useful).
extern int display_width, display_height;

extern int display_use_shader;

// A rendering function.
typedef void (*display_rendering_func_t) (void);

// Initialize the display with name and size.
void display_init (const char* name, int width, int height, display_rendering_func_t rf, int argc, char** argv);

// Called when the window is resized.
void display_reshape (int width, int height);

// Close the window.
void display_close ();

// Redraw the scene on the screen.
void display_update ();

#endif


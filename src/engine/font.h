/* font.h
 *  To display text at screen.
 */
 
#ifndef _font_h_
#define _font_h_
 
#include <GL/gl.h>
 
typedef struct font {
    GLuint  texture;
    int cw, ch;         // Size of a character.
    int tw, th;         // Size of the texture.
} font_t;

// Create a font from a bitmap font file.
// The bitmap must be a 20x5 grid of ascii characters. Each characters must have the same size, given by char_width and
// char_height.
// The characters must be ordered from ' ' (ascii 32) and '~' (ascii 126).
font_t* font_load (const char* bmp_path, int char_width, int char_height);

// Delete a font from memory.
void font_delete (font_t* font);

// Draw a string using the given font.
void font_draw_string (font_t* font, const char* string, float x, float y, float w, float h, int r, int g, int b);

#endif


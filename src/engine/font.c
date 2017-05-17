#include "engine/font.h"
#include "engine/bitmap.h"

#include <stdio.h>
#include <stdlib.h>

font_t* font_load (const char* bmp_path, int char_width, int char_height) {
    font_t* font = malloc (sizeof (font_t));
    
    bitmap_t* bitmap = bitmap_load (bmp_path);
    if (bitmap == NULL) {
        fprintf (stderr, "font_load error : Cannot load bitmap %s : %s\n", bmp_path, bitmap_error);
        free (font);
        return NULL;
    }
    
    font->texture = bitmap_to_texture (bitmap);
        
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    font->cw = char_width;
    font->ch = char_height;
    
    font->tw = bitmap->w;
    font->th = bitmap->h;
    
    bitmap_delete (bitmap);
    
    return font;
}

void font_delete (font_t* font) {
    free (font);
}

// Il y a 20 caractères par lignes.
// On commence à compter à partir de ' ' = ascii (32)
// On finit de compter à '~' = ascii (126) 
#define char_to_column(c) ((c >= ' ' && c <= '~') ? (c - ' ') % 20 : -1);
#define char_to_line(c) ((c >= ' ' && c <= '~') ? (c - ' ') / 20 : -1);

static void __font_draw_char (font_t* font, char c, float x, float y, float w, float h) {
    int column = char_to_column (c);
    int line = (font->th / font->ch - 1) - char_to_line (c);
    
    glBegin (GL_QUADS);
        glTexCoord2f ((column * font->cw) / (float) font->tw,       (line * font->ch) / (float) font->th);
        glVertex2f (x, y);
        
        glTexCoord2f (((column + 1) * font->cw - 1) / (float) font->tw, (line * font->ch) / (float) font->th);
        glVertex2f (x + w, y);
        
        glTexCoord2f (((column + 1) * font->cw - 1) / (float) font->tw, ((line + 1) * font->ch) / (float) font->th);
        glVertex2f (x + w, y + h);
        
        glTexCoord2f ((column * font->cw) / (float) font->tw,       ((line + 1) * font->ch) / (float) font->th);
        glVertex2f (x, y + h);
    glEnd ();
    
}

void font_draw_string (font_t* font, const char* string, float x, float y, float w, float h, int r, int g, int b) {
    int i;
    
    glBindTexture (GL_TEXTURE_2D, font->texture);
    
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_ADD);
    glColor3ub (r, g, b);
    for (i = 0; string [i] != '\0'; i++) {
        __font_draw_char (font, string [i], x + (i * (w + 1)), y, w, h);
    }
    glColor3ub (255, 255, 255);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


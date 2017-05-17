/* bitmap.h :
 *      Ce fichier permet de charger en mémoire un fichier bitmap.
 *
 *      Pour l'instant, on ne peut charger que des fichiers 24 ou 32 bits (32 bits pas sûr !), avec un en-tête d'information
 *      de type INFO_HEADER ou CORE_HEADER.
 *
 *      Permet de créer une texture 2d opengl à partir du bitmap (bitmap_to_texture).
 *
 *      Pour toute erreur, on peut consulter la variable globale 'bitmap_error'.
 */

#ifndef _io_bitmap_h_
#define _io_bitmap_h_

#include <stdint.h>

#include <GL/gl.h>

// Variable stockant un message d'erreur en cas de problème avec l'une des fonctions.
extern char bitmap_error [512];

// La structure d'un bitmap.
typedef struct bitmap {
    uint32_t w, h;
    uint32_t bpp;
    void*    pixel_buffer;
} bitmap_t;

// Charge un fichier en mémoire ('path' est le chemin du fichier).
// Retourne 'NULL' en cas d'erreur.
bitmap_t* bitmap_load (const char* path);

// Libère la mémoire allouée au bitmap (validité d'une texture générée dans ce cas ?).
void bitmap_delete (bitmap_t* bmp);

// Génère une texture opengl à partir d'un bitmap.
// Retourne l'identifiant OpenGL de la texture.
GLuint bitmap_to_texture (bitmap_t* bmp);

#endif


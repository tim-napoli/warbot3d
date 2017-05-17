#include <GL/glew.h>
#include "engine/bitmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char bitmap_error [512] = "";

typedef struct __bitmap_header {
    uint8_t     header_field [2];
    uint32_t    file_size;
    uint16_t    reserved [2];
    uint32_t    pixel_array_offset;
} __bitmap_header_t;

/*
static void __print_bitmap_header (__bitmap_header_t* bh) {
    printf ("= Bitmap header =\n");
    printf ("Header field : %c%c\n", bh->header_field [0], bh->header_field [1]);
    printf ("File size : %u bytes\n", bh->file_size);
    printf ("Reserved fields : 0x%x 0x%x\n", bh->reserved [0], bh->reserved [1]);
    printf ("Pixel array offset : 0x%x\n", bh->pixel_array_offset);
    printf ("Size : %lu\n", sizeof (__bitmap_header_t));
}
*/

enum {
    BMP_RGB = 0,
    BMP_RLE8,
    BMP_RLE4,
    BMP_BITFIELDS,
    BMP_JPEG,
    BMP_PNG,
    BMP_ALPHABITFIELDS
};

typedef struct __bitmap_info_header {
    uint32_t    header_size;
    int32_t     bmp_w, bmp_h;
    uint16_t    color_planes;
    uint16_t    bpp;
    uint32_t    compression_method;
    uint32_t    image_size;
    int32_t     res_w, res_h;
    uint32_t    palette_size;
    uint32_t    important_colors;
} __bitmap_info_header_t;

/*
static void __print_bitmap_info_header (__bitmap_info_header_t* bih) {
    printf ("= Bitmap info header =\n");
    printf ("Header size : %u\n", bih->header_size);
    printf ("Bitmap width : %d\n", bih->bmp_w);
    printf ("Bitmap height : %d\n", bih->bmp_h);
    printf ("Number of color planes : %d\n", bih->color_planes);
    printf ("Bits per pixel : %d\n", bih->bpp);
    printf ("Compression method : %u\n", bih->compression_method);
    printf ("Image size : %u\n", bih->image_size);
    printf ("Resolution : %d %d\n", bih->res_w, bih->res_h);
    printf ("Palette size : %u\n", bih->palette_size);
    printf ("Important color : 0x%x\n", bih->important_colors);
}
*/

typedef struct __bitmap_core_header {
    uint32_t    header_size;
    uint32_t     bmp_w, bmp_h;
    uint16_t    color_planes;
    uint16_t    bpp;
} __bitmap_core_header_t;

/*
static void __print_bitmap_core_header (__bitmap_core_header_t* bch) {
    printf ("= Bitmap core header =\n");
    printf ("Header size : %u\n", bch->header_size);
    printf ("Bitmap width : %d\n", bch->bmp_w);
    printf ("Bitmap height : %d\n", bch->bmp_h);
    printf ("Number of color planes : %d\n", bch->color_planes);
    printf ("Bits per pixel : %d\n", bch->bpp);
}
*/

typedef union __bitmap_dib {
    __bitmap_info_header_t bih;
    __bitmap_core_header_t bch;
} __bitmap_dib_t;

bitmap_t* bitmap_load (const char* path) {
    uint8_t           bmp_header [14];
    uint8_t           bmp_dib [120];
    bitmap_t*         bmp;
    
    __bitmap_info_header_t* bmp_ih = (__bitmap_info_header_t*) bmp_dib;

    FILE* f = fopen (path, "rb");
    if (f == NULL) {
        sprintf (bitmap_error, "File %s can not be found.", path);
        return NULL;
    }
    
    // Lecture de l'en-tête et de l'en-tête d'informations du bitmap.
    fread (bmp_header, 14, 1, f);
    fread (bmp_dib, 4, 1, f);                       // Récupère la taille de l'en-tête.
    fseek (f, -4, SEEK_CUR);
    fread (bmp_dib, bmp_ih->header_size, 1, f);

    //__print_bitmap_info_header (bmp_ih);

    if (bmp_ih->header_size != 40 && bmp_ih->header_size != 12) {
        sprintf (bitmap_error, "Bitmap file %s is not a supported bitmap format (must have an INFO_HEADER or a CORE_HEADER).", path);
        fclose (f);
        return NULL;
    }
    
    if (bmp_ih->bpp < 16) {
        sprintf (bitmap_error, "Bitmap file %s must have a color depth of 16, 24 or 32.", path);
        fclose (f);
        return NULL;
    }
    
    // Création du bitmap
    bmp = malloc (sizeof (bitmap_t));
    bmp->w = bmp_ih->bmp_w;
    bmp->h = bmp_ih->bmp_h;
    bmp->bpp = bmp_ih->bpp;
    bmp->pixel_buffer = malloc (bmp->w * bmp->h * (bmp->bpp >> 3));
    
    fseek (f, bmp_ih->palette_size, SEEK_CUR);
    fread (bmp->pixel_buffer, bmp->w * bmp->h * (bmp->bpp >> 3), 1, f);
    
    fclose (f);
    return bmp;
}

void bitmap_delete (bitmap_t* bmp) {
    free (bmp->pixel_buffer);
    free (bmp);
}

GLuint bitmap_to_texture (bitmap_t* bmp) {
    GLuint texture_id;
    
    glGenTextures (1, &texture_id);
    glBindTexture (GL_TEXTURE_2D, texture_id);
    
    if (bmp->bpp == 24) {
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, bmp->w, bmp->h, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp->pixel_buffer);
    }
    else if (bmp->bpp == 32) {
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, bmp->w, bmp->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, bmp->pixel_buffer);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    return texture_id;
}



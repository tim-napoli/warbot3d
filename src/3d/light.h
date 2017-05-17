/* light.h :
 *  Gestion de l'éclairage.
 *  Permet de définir de nouvelles lumières OpenGL.
 */

#ifndef _light_h_
#define _light_h_

#include "math/vector.h"

// Structure définissant une lumière.
typedef struct light {
    int num;            // Light num.
    vector_t position;  // Position.
    
    float specular [4];  // Colors.
    float ambient  [4];
    float diffuse  [4];
    
    float quadratic_att;
    float linear_att;
} light_t;

extern int enabled_lights [8];

// Initialise une nouvelle lumière. Les paramètres sont :
//  light           : l'adresse de la lumière à initialiser.
//  num             : le numéro de la lumière (parmis GL_LIGHT[0-7]).
//  p<x, y, z>      : la position de la lumière dans la scène.
//  spec_<r, g, b>  : la couleur de la lumière spéculaire.
//  amb_<r, g, b>   : la couleur de la lumière ambiante.
//  diff_<r, g, b>  : la couleur de la lumière diffuse.
void light_init (light_t* light, int num, 
                 float px, float py, float pz, int directional, 
                 float spec_r, float spec_g, float spec_b, 
                 float amb_r, float amb_g, float amb_b, 
                 float diff_r, float diff_g, float diff_b,
                 float quadratic_att, float linear_att);

// Met à jour les paramètres de la lumière.
void light_update (light_t* light);

void light_update_position (light_t* light);

// Demande à OpenGL d'activer la lumière.
void light_enable (light_t* light);

// Demande à OpenGL de désactiver la lumière.
void light_disable (light_t* light);


#endif


#include "engine/display.h"
#include "3d/light.h"

#include <string.h>
#include <GL/gl.h>

int enabled_lights [8] = {0};

void light_init (light_t* light, int num, 
                 float px, float py, float pz, int directional, 
                 float spec_r, float spec_g, float spec_b, 
                 float amb_r, float amb_g, float amb_b, 
                 float diff_r, float diff_g, float diff_b,
                 float quadratic_att, float linear_att) {
    light->num = num;
    light->position.x = px;
    light->position.y = py;
    light->position.z = pz;
    light->position.w = (directional) ? 0.0f : 1.0f;
    
    light->specular [0] = spec_r * ((display_use_shader) ? 1.0 : 0.5);
    light->specular [1] = spec_g * ((display_use_shader) ? 1.0 : 0.5);
    light->specular [2] = spec_b * ((display_use_shader) ? 1.0 : 0.5);
    light->specular [3] = 1.0f;
    
    light->ambient [0] = amb_r * ((display_use_shader) ? 1.0 : 0.5);
    light->ambient [1] = amb_g * ((display_use_shader) ? 1.0 : 0.5);
    light->ambient [2] = amb_b * ((display_use_shader) ? 1.0 : 0.5);
    light->ambient [3] = 1.0f;
    
    light->diffuse [0] = diff_r * ((display_use_shader) ? 1.0 : 0.5);
    light->diffuse [1] = diff_g * ((display_use_shader) ? 1.0 : 0.5);
    light->diffuse [2] = diff_b * ((display_use_shader) ? 1.0 : 0.5);
    light->diffuse [3] = 1.0f;
    
    light->quadratic_att = quadratic_att;
    light->linear_att = linear_att;
}

void light_update_position (light_t* light) {
    glLightfv (light->num, GL_POSITION, (float*) &(light->position));
}

void light_enable (light_t* light) {
    enabled_lights [light->num - GL_LIGHT0] = 1;
    glEnable (light->num);

    glLightfv (light->num, GL_POSITION, (float*) &(light->position));
    glLightfv (light->num, GL_AMBIENT, light->ambient);
    glLightfv (light->num, GL_DIFFUSE, light->diffuse);
    glLightfv (light->num, GL_SPECULAR, light->specular);
    glLightf (light->num, GL_LINEAR_ATTENUATION, light->linear_att);
    glLightf (light->num, GL_QUADRATIC_ATTENUATION, light->quadratic_att);
}

void light_disable (light_t* light) {
    enabled_lights [light->num - GL_LIGHT0] = 0;
    glDisable (light->num);
}


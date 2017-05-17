/* shader.h
 *  Managing shader.
 *  This module permits to load shaders, compile them and use them.
 *
 *  If the program is launched with the argument '-shader=0', then no of theses functions will have effect when called.
 *
 *  Some functions give access to default uniforms.
 *  To update uniforms, use the "shader_update_*" functions.
 *  Some of the must then call "shader_send_uniforms" with good uniform filter to update value of the uniform in the
 *  shader (for performances). This will be sayed in the doc. Else, the uniform must be considered like directly 
 *  updated.
 *
 *  The list of uniforms are :
 *      int light_states [8]    : Set to one when a light is enabled or zero else.
 *      Sampler2D color_map     : The texture used with the shader.
 *      Sampler2D normal_map    : The normal map used with the shader.
 *      vec3 tangent            : A 3d vector if tangent space must be computed.
 *      mat4 world_matrix       : The model to world matrix.
 *      vec3 camera_wpos        : The camera world position.
 *      vec3 lights_wpos [8]    : The world position of each light.
 *      float point_size        : The current point size.
 */

#ifndef _shader_h_
#define _shader_h_

#include "3d/light.h"

#include <GL/gl.h>

typedef struct shader {
    GLuint  v_shader;
    GLuint  f_shader;
    GLuint  program;
    
    // Uniforms id
    GLint u_light_states;
    GLint u_normal_map;
    GLint u_color_map;
    GLint u_tangent;
    GLint u_camera_wpos;
    GLint u_camera_dir;
    GLint u_lights_wpos;
    GLint u_world_matrix;
    GLint u_point_size;
    
    // Uniform values.
    int     d_light_states [8];
    GLuint  d_color_map;
    GLuint  d_normal_map;
    float   d_tangent [3];
    float   d_camera_wpos [3];
    float   d_lights_wpos [24];
} shader_t;

// The current shader is the last shader called with shader_select(). NULL at default.
extern shader_t* current_shader;

// Load a new shader from files.
// Return NULL on error, and print compilation message on stderr.
shader_t*   shader_load (const char* vertex_path, const char* frag_path);

// Delete a shader from memory.
void shader_delete (shader_t* shader);

// Set the current shader to use when rendering.
void shader_select (shader_t* shader);

// Pass to fixed pipeline mode.
void shader_release ();


// Update the state of each light.
void shader_update_light_states (shader_t* shader);

// Update the current color map.
void shader_update_color_map (shader_t* shader, GLuint texture);

// Update the current normal map.
void shader_update_normal_map (shader_t* shader, GLuint texture);

// Update the current tangent.
void shader_update_tangent (shader_t* shader, float tx, float ty, float tz);

// Update the current model to world matrix.
void shader_update_world_matrix (shader_t* shader);

// Update the camera world position.
void shader_update_camera_world_position (shader_t* shader, float x, float y, float z);

// Update the camera direction.
void shader_update_camera_direction (shader_t* shader, float x, float y, float z);

// Set the new light world position.
// This function must be followed by a call of shader_send_uniforms (shader, shader_send_lights_wpos) to take effect.
void shader_update_light_world_position (shader_t* shader, light_t* light);

// Update the current point_size.
void shader_update_point_size (shader_t* shader, float size);


enum {
    shader_send_lights_wpos = 0x10,
    shader_send_all = 0xffffffff
};

// Send the filtered uniforms to the shader. See preious enumeration to know which flags give to filter to update what.
void shader_send_uniforms (shader_t* shader, int filter);



#endif


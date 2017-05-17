#include <GL/glew.h>
#include <GL/gl.h>

#include "3d/shader.h"
#include "engine/display.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

shader_t* current_shader = NULL;

int __load_shader_source (GLuint shader, const char* path) {
    GLchar* buffer;
    char err_buffer [BUFSIZ];
    
    // Reading file in buffer.
    FILE* f = fopen (path, "rb");
    if (f == NULL) {
        fprintf (stderr, "__load_shader_source error : cannot open file %s.\n", path);
        return 1;
    }
    
    GLint size;
    fseek (f, 0, SEEK_END);
    size = (GLint) ftell (f);
    buffer = malloc ((size + 1) * sizeof (GLchar));
    fseek (f, 0, SEEK_SET);
    if (fread (buffer, size, 1, f) != 1) {
        fprintf (stderr, "__load_shader_source error : error during %s reading.\n", path);
        return 1;
    }
    buffer [size] = '\0';
    fclose (f);

    // Give shader code to openGL & compiling
    glShaderSource (shader, 1, (const GLchar**) &buffer, &size);
    glCompileShader (shader);
    
    free (buffer);
    
    // Checking success
    GLint err_code;
    GLint err_size;
    glGetShaderiv (shader, GL_COMPILE_STATUS, &err_code);
    if (err_code != GL_TRUE) {
        glGetShaderInfoLog (shader, BUFSIZ - 1, &err_size, err_buffer);
        err_buffer [err_size] = '\0';
        fprintf (stderr, "__load_shader_source error : error during %s compilation :\n%s\n", path, err_buffer);
        return 1;
    }
    
    return 0;
}

shader_t* shader_load (const char* vertex_path, const char* frag_path) {
    char err_buffer [BUFSIZ];

    shader_t* shader = malloc (sizeof (shader_t));

    shader->v_shader = glCreateShader (GL_VERTEX_SHADER);
    if (__load_shader_source (shader->v_shader, vertex_path)) {
        fprintf (stderr, "shader_load error : error during vertex shader creation.\n");
        glDeleteShader (shader->v_shader);
        free (shader);
        return NULL;
    }
    
    shader->f_shader = glCreateShader (GL_FRAGMENT_SHADER);
    if (__load_shader_source (shader->f_shader, frag_path)) {
        fprintf (stderr, "shader_load error : error during fragment shader creation.\n");
        glDeleteShader (shader->v_shader);
        glDeleteShader (shader->f_shader);
        free (shader);
        return NULL;
    }
    
    // Creating shader program.
    shader->program = glCreateProgram ();
    glAttachShader (shader->program, shader->v_shader);
    glAttachShader (shader->program, shader->f_shader);
    glLinkProgram (shader->program);
    
    // Checking for error.
    GLint err_code;
    GLint err_size;
    glGetProgramiv (shader->program, GL_LINK_STATUS, &err_code);
    if (err_code != GL_TRUE) {
        glGetShaderInfoLog (shader->v_shader, BUFSIZ - 1, &err_size, err_buffer);
        err_buffer [err_size] = '\0';
        fprintf (stderr, "shader_load error : linking error (vertex shader %s) :\n%s\n", vertex_path, err_buffer);
        
        glDetachShader (shader->program, shader->v_shader);
        glDetachShader (shader->program, shader->f_shader);
        glDeleteProgram (shader->program);
        glDeleteShader (shader->v_shader);
        glDeleteShader (shader->f_shader);
        free (shader);
        return NULL;
    }
    
    
    // Uniform creation.
    shader->u_light_states = glGetUniformLocation (shader->program, "light_states");
    shader->u_color_map = glGetUniformLocation (shader->program, "color_map");
    shader->u_normal_map = glGetUniformLocation (shader->program, "normal_map");
    shader->u_tangent = glGetUniformLocation (shader->program, "tangent");
    shader->u_camera_wpos = glGetUniformLocation (shader->program, "camera_wpos");
    shader->u_camera_dir = glGetUniformLocation (shader->program, "camera_dir");
    shader->u_lights_wpos = glGetUniformLocation (shader->program, "lights_wpos");
    shader->u_world_matrix = glGetUniformLocation (shader->program, "world_matrix");
    shader->u_point_size = glGetUniformLocation (shader->program, "point_size");
    
    return shader;
}

void shader_delete (shader_t* shader) {
    glUseProgram (0);
    
    glDetachShader (shader->program, shader->v_shader);
    glDetachShader (shader->program, shader->f_shader);
    glDeleteProgram (shader->program);
    glDeleteShader (shader->v_shader);
    glDeleteShader (shader->f_shader);
    free (shader);
}

void shader_select (shader_t* shader) {
    if (display_use_shader) {
        glUseProgram (shader->program);
        current_shader = shader;
    }
}

void shader_release () {
    if (display_use_shader) glUseProgram (0);
    current_shader = NULL;
}


void shader_update_light_states (shader_t* shader) {
    if (display_use_shader)  {
        /*
        shader->d_light_states [0] = glIsEnabled (GL_LIGHT0);
        shader->d_light_states [1] = glIsEnabled (GL_LIGHT1);
        shader->d_light_states [2] = glIsEnabled (GL_LIGHT2);
        shader->d_light_states [3] = glIsEnabled (GL_LIGHT3);
        shader->d_light_states [4] = glIsEnabled (GL_LIGHT4);
        shader->d_light_states [5] = glIsEnabled (GL_LIGHT5);
        shader->d_light_states [6] = glIsEnabled (GL_LIGHT6);
        shader->d_light_states [7] = glIsEnabled (GL_LIGHT7);
        */
        
        glUniform1iv (shader->u_light_states, 8, enabled_lights);
    }
}

void shader_update_color_map (shader_t* shader, GLuint texture) {
    if (shader->d_color_map != texture) {
        shader->d_color_map = texture;
    
        if (display_use_shader) {
            glActiveTexture (GL_TEXTURE0);
            glBindTexture (GL_TEXTURE_2D, shader->d_color_map);
            glBindSampler (GL_TEXTURE_2D, shader->d_color_map);
            glUniform1i (shader->u_color_map, 0);
        }
        else {
            glActiveTexture (GL_TEXTURE0);
            glBindTexture (GL_TEXTURE_2D, shader->d_color_map);
        }
    }
}

void shader_update_normal_map (shader_t* shader, GLuint texture) {
    if (shader->d_normal_map != texture) {
        shader->d_normal_map = texture;
        
        if (display_use_shader) {
            glActiveTexture (GL_TEXTURE1);
            glBindTexture (GL_TEXTURE_2D, shader->d_normal_map);
            glBindSampler (GL_TEXTURE_2D, shader->d_normal_map);
            glUniform1i (shader->u_normal_map, 1);
        }
    }
}

void shader_update_tangent (shader_t* shader, float tx, float ty, float tz) {
    if (display_use_shader) {
        shader->d_tangent [0] = tx;
        shader->d_tangent [1] = ty;
        shader->d_tangent [2] = tz;
        
        glUniform3fv (shader->u_tangent, 1, shader->d_tangent);
   }
}

void shader_update_camera_world_position (shader_t* shader, float x, float y, float z) {
    if (display_use_shader) {
        shader->d_camera_wpos [0] = x;
        shader->d_camera_wpos [1] = y;
        shader->d_camera_wpos [2] = z;
        
        glUniform3fv (shader->u_camera_wpos, 1, shader->d_camera_wpos);
   }
}

void shader_update_camera_direction (shader_t* shader, float x, float y, float z) {
    float d [3] = {x, y, z};

    if (display_use_shader) {
        glUniform3fv (shader->u_camera_dir, 1, d);
   }
}

void shader_update_light_world_position (shader_t* shader, light_t* light) {
    if (display_use_shader) {
        shader->d_lights_wpos [(light->num - GL_LIGHT0) * 3] = light->position.x;
        shader->d_lights_wpos [(light->num - GL_LIGHT0) * 3 + 1] = light->position.y;
        shader->d_lights_wpos [(light->num - GL_LIGHT0) * 3 + 2] = light->position.z;
    }
    
    //glUniform3fv (shader->u_lights_wpos, 8, shader->d_lights_wpos);
}

void shader_update_world_matrix (shader_t* shader) {
    if (display_use_shader) {
        GLfloat model [16];
        glGetFloatv (GL_MODELVIEW_MATRIX, model); 
        glUniformMatrix4fv (shader->u_world_matrix, 1, GL_FALSE, model);
    }
}

void shader_update_point_size (shader_t* shader, float size) {
    if (display_use_shader) glUniform1f (shader->u_point_size, size);
}

void shader_send_uniforms (shader_t* shader, int filter) {
    if (display_use_shader) {
        if (filter & shader_send_lights_wpos) {
            glUniform3fv (shader->u_lights_wpos, 8, shader->d_lights_wpos);
        }
   }
}

#ifdef TEST_SHADER

#include <GL/glut.h>

int main (int argc, char** argv) {
    if (argc < 2) {
        printf ("Usage : test_shader shader_file\n");
        return 1;
    }
    
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
    glutInitWindowSize (20, 20);
    glutCreateWindow ("tshader");
    //glutMainLoop ();
    
    GLenum err = glewInit ();
    if (err != GLEW_OK) {
        fprintf (stderr, "display init error : cannot load GLEW : %s\n", glewGetErrorString (err));
    }
    
    if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader)
    {
        printf("No GLSL support\n");
        exit(1);
    }
    
    shader_t* shader = shader_load (argv [1], argv [2]);
    
    if (shader != NULL) shader_delete (shader);

    return 0;
}

#endif


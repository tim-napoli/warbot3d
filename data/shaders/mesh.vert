// Lights are :
// 0 1 2 for positionals lights.
// 6 for ambient light.
// 7 for directional light.

uniform int light_states [8];
uniform vec3 lights_wpos [8];

uniform mat4 world_matrix;

varying vec3 N;

varying vec3 vertex;

void main (void) {
    vertex = vec3 (world_matrix * gl_Vertex);

    // Handling positional lights.
    
    // Directional light
    N = normalize (vec3 (world_matrix * vec4 (gl_Normal, 0.0)));
    
    gl_TexCoord [0] = gl_MultiTexCoord0;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}


// Lights are :
// 0 1 2 for positionals lights.
// 6 for ambient light.
// 7 for directional light.

uniform int     light_states [8];
uniform vec3    lights_wpos [8];
uniform vec3    camera_wpos;
uniform vec3    tangent;

varying vec3 N, T, B, E;

varying vec3 vertex;

void main (void) {
    gl_TexCoord [0] = gl_MultiTexCoord0;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    vertex = vec3 (gl_Vertex);
    N = normalize (gl_Normal);
    T = normalize (tangent);
    B = cross (N, T);
    
    vec3 Et = gl_ModelViewMatrix * gl_Vertex;  // Et is the vector going from the camera to the vertex.
    E.x = dot (Et, T);
    E.y = dot (Et, B);
    E.z = dot (Et, N);
    E = normalize (E);
}


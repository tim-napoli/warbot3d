varying vec3 v;

uniform float point_size;

void main (void) {
    v = gl_Vertex.xyz;
    gl_Position = ftransform ();
    gl_FrontColor = gl_Color;
    gl_PointSize = point_size;
}


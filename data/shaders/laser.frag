varying vec3 v;

float rand(vec2 n)
{
    return 0.5 + 0.5 * fract (sin (dot (n.xy, vec2 (12.9898, 78.233))) * 43758.5453);
}

void main (void) {
    float x = rand (v.xy);
    gl_FragColor = (x * gl_Color);
}


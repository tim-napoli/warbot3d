uniform int light_states [8];
uniform vec3 lights_wpos [8];

varying vec3 N;

uniform sampler2D color_map;

varying vec3 vertex;


void main (void) {
    vec4 tex_color = texture2D (color_map, gl_TexCoord [0].st);

    float d;
    vec3 L;
    float lambertTerm;
    
    vec4 color = vec4 (0, 0, 0, 0);

    // Lumières positionelles.
    
    
    if (light_states [0] == 1) {
        L = lights_wpos [0] - vertex.xyz;
        d = dot (L, L);

        L = normalize (L);
        lambertTerm = max (dot (N, L), 0.0);
        color +=  vec4 (gl_LightSource [0].diffuse *  gl_FrontMaterial.diffuse * lambertTerm) / (0.002 * d);
    }
    
    if (light_states [1] == 1) {
        L = lights_wpos [1] - vertex.xyz;
        d = dot (L, L);
        L = normalize (L);
        lambertTerm = max (dot (N, L), 0.0);
        color +=  vec4 (gl_LightSource [1].diffuse *  gl_FrontMaterial.diffuse * lambertTerm) / (0.002 * d);
    }
    
    if (light_states [2] == 1) {
        L = lights_wpos [2] - vertex.xyz;
        d = dot (L, L);
        L = normalize (L);
        lambertTerm = max (dot (N, L), 0.0);
        color +=  vec4 (gl_LightSource [2].diffuse *  gl_FrontMaterial.diffuse * lambertTerm) / (0.002 * d);
    }
    
    // Lumière directionelle
    if (light_states [6] == 1) {
        L = normalize (lights_wpos [6]);
  
        lambertTerm = max (dot (L, N), 0.0);

        color += vec4 (gl_LightSource [6].diffuse *  gl_FrontMaterial.diffuse * lambertTerm
                      + gl_LightSource [6].ambient * gl_FrontMaterial.ambient);
    }
    
    // Lumière ambiante
    if (light_states [7] == 1) {
        color += vec4 (gl_LightSource [7].ambient * gl_FrontMaterial.ambient);
    }
    
    // Multiplication par la couleur de texture (distribution).
    gl_FragColor = color * tex_color;
}


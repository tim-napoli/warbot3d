uniform int light_states [8];
uniform vec3 lights_wpos [8];

varying vec3 N, E, T, B;

uniform sampler2D color_map;
uniform sampler2D normal_map;
uniform vec3 tangent;

varying vec3 vertex;


void main (void) {
    vec4 tex_color = texture2D (color_map, gl_TexCoord [0].st);
    vec4 tex_normal = texture2D (normal_map, gl_TexCoord [0].st);
    vec3 light_direction;
    float light_att;
    float d, dd;
    vec3 L, Lt, Ldist;
    float lambertTerm;
    
    vec3 Nn = tex_normal.rgb * 2.0 - 1.0;
    
    
    vec4 color = vec4 (0, 0, 0, 0);

    // Lumières positionelles.
    
    
    if (light_states [0] == 1) {
        L = (lights_wpos [0] - vertex.xyz);
        
        // On simplifie le calcul de l'atténuation en n'utilisant que le coeficient d'ordre 2 
        // (pas de longueur à calculer).
        // Cela permet d'utiliser uniquement le produit scalaire plutôt que la longueur du vecteur.
        d = dot (L, L);
        light_att = 0.002 * d;

        Lt.x = dot (L, T);
        Lt.y = dot (L, B);
        Lt.z = dot (L, N);
        Ldist = normalize (Lt);
        L = normalize (Lt);
        lambertTerm = max (dot (Nn, L), 0.0);
        gl_FragColor +=  vec4 (gl_LightSource [0].diffuse *  gl_FrontMaterial.diffuse * lambertTerm) / light_att;
        
        vec3 R = reflect (-Ldist, Nn);
        float specular = dot (E, R);
        if (specular >= 0 && specular <= 1) {
            specular = pow (specular, gl_FrontMaterial.shininess);
            gl_FragColor += vec4 ((gl_LightSource [0].diffuse * 0.5 * gl_FrontMaterial.specular * specular) / light_att);
        }
    }
    
    if (light_states [1] == 1) {
        L = (lights_wpos [1] - vertex.xyz);
        
        d = dot (L, L);
        light_att = 0.002 * d;

        Lt.x = dot (L, T);
        Lt.y = dot (L, B);
        Lt.z = dot (L, N);
        Ldist = normalize (Lt);
        L = normalize (Lt);
        lambertTerm = max (dot (Nn, L), 0.0);
        gl_FragColor +=  vec4 (gl_LightSource [1].diffuse *  gl_FrontMaterial.diffuse * lambertTerm) / light_att;
        
        vec3 R = reflect (-Ldist, Nn);
        float specular = dot (E, R);
        if (specular >= 0 && specular <= 1) {
            specular = pow (specular, gl_FrontMaterial.shininess);
            gl_FragColor += vec4 ((gl_LightSource [1].diffuse * 0.5 * gl_FrontMaterial.specular * specular) / light_att);
        }
    }
    
    if (light_states [2] == 1) {
        L = (lights_wpos [2] - vertex.xyz);
        
        d = dot (L, L);
        light_att = 0.002 * d;

        Lt.x = dot (L, T);
        Lt.y = dot (L, B);
        Lt.z = dot (L, N);
        Ldist = normalize (Lt);
        L = normalize (Lt);
        lambertTerm = max (dot (Nn, L), 0.0);
        gl_FragColor +=  vec4 (gl_LightSource [2].diffuse * 0.5 * gl_FrontMaterial.diffuse * lambertTerm) / light_att;
        
        vec3 R = reflect (-Ldist, Nn);
        float specular = dot (E, R);
        if (specular >= 0 && specular <= 1) {
            specular = pow (specular, gl_FrontMaterial.shininess);
            gl_FragColor += vec4 ((gl_LightSource [2].diffuse * gl_FrontMaterial.specular * specular) / light_att);
        }
    }
    
    // Lumière directionelle
    if (light_states [6] == 1) {
        L = lights_wpos [6];
        
        Lt.x = dot (L, T);
        Lt.y = dot (L, B);
        Lt.z = dot (L, N);
        
        
        L = normalize (Lt);
        //L = normalize (tmpL);
  
        lambertTerm = max (dot (L, Nn), 0.0);

        gl_FragColor += vec4 (gl_LightSource [6].diffuse *  gl_FrontMaterial.diffuse * lambertTerm);
        gl_FragColor += gl_LightSource [6].ambient * gl_FrontMaterial.ambient;
        if (dot (L, Nn) > 0) {
            float specular = pow (clamp (dot (reflect (L, Nn), E), 0.0, 1.0), gl_FrontMaterial.shininess);
            gl_FragColor += vec4 ((gl_LightSource [6].specular * gl_FrontMaterial.specular) * specular);
        }
    }
    
    // Lumière ambiante
    if (light_states [7] == 1) {
        //L = normalize (vec3 (1, -1.0, 1));
        //lambertTerm = max (dot (Nn, L), 0.0);
        //gl_FragColor += gl_LightSource [7].diffuse *  gl_FrontMaterial.diffuse * lambertTerm;
        
        gl_FragColor += gl_LightSource [7].ambient * gl_FrontMaterial.ambient;
    }
    
    // Multiplication par la couleur de texture (distribution).
    gl_FragColor *= tex_color;
}


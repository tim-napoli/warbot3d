#include "scene/level_creation.h"

void level_creation (level_t* level) {
    level_init (level, 20, 300);
    
    light_init (&level->light, GL_LIGHT6,
                -1.24, 7.19, -6.86, 1,
                0.3, 0.51, 0.51,
                0.15, 0.18, 0.18,
                0.3, 0.51, 0.51,
                0.0005, 0.0005);
                
    
    sector_t* s;
    
    level_vertex_t v;
    
    // Pièce centrale.
    s = level_add_sector (level, 25, level_texture_stone_floor, -1, 0, 0, 0);
    
    // Façade nord
    v.x = -45;  v.z = 45;
    sector_begin (s, &v);
    
    v.x = -15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -10; v.z = 50;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 10;
    sector_add_wall (s, &v, 0, -1, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 15; v.z = 45;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 45;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Façade ouest
    v.z = 15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 50; v.z = 10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 45; v.z = -15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 45; v.z = -45;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Façade sud
    v.x = 15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 10; v.z = -50;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -10;
    sector_add_wall (s, &v, 0, -1, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -15; v.z = -45;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -45;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Façade est
    v.z = -15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -10; v.x = -50;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = 10;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = 15; v.x = -45;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    sector_end (s, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    
    
    // Couloir nord.
    s = level_add_sector (level, 20, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = -10;  v.z = 50;
    sector_begin (s, &v);
    
    v.z = 70;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.z = 90;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.x = 30;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.z = 70;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = 10;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.z = 50;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    sector_end (s, 0, -1, 0, 0, -1, 0, 0);
    
    
    
    s = level_add_sector (level, 20, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = -10;  v.z = 70;
    sector_begin (s, &v);
    
    v.x = -30; v.z = 80;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -60; v.z = 80;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -70; v.z = 70;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -90; v.z = 70;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.x = -70; v.z = 110;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -20; v.z = 110;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -10; v.z = 90;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    sector_end (s, 0, -1, 0, 0, -1, 0, 0);
    
    
    s = level_add_sector (level, 20, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = -70; v.z = 70;
    sector_begin (s, &v);
    
    v.x = -70; v.z = 15;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -90; v.z = 15;
    sector_add_wall (s, &v, 0, -1, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -90; v.z = 70;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    sector_end (s, 0, -1, 0, 0, -1, 0, 0);
    
    
    // Couloir extérieur est.
    s = level_add_sector (level, 25, level_texture_stone_floor, -1, 0, 0, 0);
    
    v.x = -50;
    v.z = 10;
    sector_begin (s, &v);
    
    v.x = -50; v.z = -10;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.x = -90; v.z = -10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -95; v.z = -15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -115; v.z = -15;
    sector_add_wall (s, &v, 0, -1, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -120; v.z = -10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -120; v.z = 10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -95; v.z = 10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -90; v.z = 15;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -70; v.z = 15;
    sector_add_wall (s, &v, 0, -1, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -65; v.z = 10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    sector_end (s, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Petit couloir sud-est
    s = level_add_sector (level, 20, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = -115; v.z = -15;
    sector_begin (s, &v);
    
    v.x = -95;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -90;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -115;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);

    sector_end (s, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    
    // Petit couloir sud.
    s = level_add_sector (level, 20, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = -10; v.z = -50;
    sector_begin (s, &v);
    
    v.x = 10;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -90;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    v.x = -10;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);    
    
    sector_end (s, wall_blocking, level_texture_tech_wall, 0, 0, -1, 0, 0);
    
    // Pièce à pilier (I)
    s = level_add_sector (level, 25, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = -95; v.z = -90;
    sector_begin (s, &v);
    
    v.x = -10;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    v.x = 10;
    sector_add_wall (s, &v, 0, -1, 0, 0, level_texture_tech_wall, 0, 0);
    
    v.x = 40;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    // Premier demi-pilier
    v.x = 20;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -125;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Second
    v.x = -10;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -125;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -20;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Troisième
    v.x = -40;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -125;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -50;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Quatrième
    v.x = -70;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -125;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -80;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Cinquième
    v.x = -100;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -125;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -110;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    
    // Fin
    v.x = -130;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -90;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    v.x = -115;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    sector_end (s, 0, -1, 0, 0, level_texture_tech_wall, 0, 0);
    
    
    // Pièce pilier (II)
    s = level_add_sector (level, 25, level_texture_tech_floor, level_texture_ceil, 0.20, .20, 0.20);
    
    v.x = 40; v.z = -130;
    sector_begin (s, &v);
    
    v.z = -170;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    v.x = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_tech_wall, 0, 0, level_texture_tech_wall, 0, 0);
    
    // Cinquième
    v.x = -110;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -135;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -100;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Quatrième
    v.x = -80;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -135;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -70;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Troisième
    v.x = -50;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -135;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -40;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Second
    v.x = -20;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -135;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = -10;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Premier
    v.x = 10;
    sector_add_wall (s, &v, 0, -1, 0, 0, -1, 0, 0);
    
    v.z = -135;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.x = 20;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    v.z = -130;
    sector_add_wall (s, &v, wall_blocking, level_texture_stone_wall, 0, 0, level_texture_stone_wall, 0, 0);
    
    // Fin
    sector_end (s, 0, -1, 0, 0, -1, 0, 0);
    
    
    
    // Couloir extèrieur est.
    
    
    
    
    level_generate_nodes (level);
}


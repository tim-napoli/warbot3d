#include "scene/level.h"
#include <math.h>

#include "3d/shader.h"
#include "3d/material.h"
#include "engine/display.h"
#include "engine/bitmap.h"
#include "math/line.h"
#include "math/triangle.h"

#include "scene/scene_data.h"

#include <GL/glut.h>

//#include "3d/texture.h"

//--------------------------------------------------------------------------------------------------------------------//
// global
//--------------------------------------------------------------------------------------------------------------------//
char level_bitmap_names [level_texture_max] [64] = {
    "data/tech_wall.bmp",
    "data/stone_wall.bmp",
    "data/tech_floor.bmp",
    "data/stone_floor.bmp",
    "data/ceil.bmp"
};

char level_normal_bitmap_names [level_texture_max] [64] = {
    "data/tech_wall_normal.bmp",
    "data/stone_wall_normal.bmp",
    "data/tech_floor_normal.bmp",
    "data/stone_floor_normal.bmp",
    "data/ceil_normal.bmp"
};

material_t level_materials [level_texture_max] = {
    {
        "tech_wall",
        {1.0, 1.0, 1.0, 1.0},
        {0.5, 0.5, 0.5, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        0.1, 0.0, 0
    },
    {
        "stone_wall",
        {1.0, 1.0, 1.0, 1.0},
        {0.5, 0.5, 0.5, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        0.1, 0.0, 0
    },
    {
        "tech_floor",
        {1.0, 1.0, 1.0, 1.0},
        {0.5, 0.5, 0.5, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        0.1, 0.0, 0
    },
    {
        "stone_floor",
        {0.2, 0.2, 0.2, 1.0},
        {0.5, 0.5, 0.5, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        0.001, 0.0, 0
    },
    {
        "ceil",
        {1.0, 1.0, 1.0, 1.0},
        {0.5, 0.5, 0.5, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        0.1, 0.0, 0
    }
};

level_vertex_t  level_texture_sizes [level_texture_max];

GLuint level_textures [level_texture_max] = {0};
GLuint level_normal_textures [level_texture_max] = {0};

material_t* level_material = NULL;

shader_t* level_shader = NULL;

int level_display_nodes;

int level_init_module (int display_nodes) {
    size_t i;
    
    bitmap_t* level_bitmaps [level_texture_max] = {NULL};
    bitmap_t* level_normal_bitmaps [level_texture_max] = {NULL};
    
    for (i = 0; i < level_texture_max; i++) {
        level_bitmaps [i] = bitmap_load (level_bitmap_names [i]);
        if (level_bitmaps [i] == NULL) {
            fprintf (stderr, "level_init_module error : Cannot load bitmap %s\n", level_bitmap_names [i]);
            return 1;
        }
        level_textures [i] = bitmap_to_texture (level_bitmaps [i]);
        level_texture_sizes [i].x = level_bitmaps [i]->w;
        level_texture_sizes [i].z = level_bitmaps [i]->h;
        bitmap_delete (level_bitmaps [i]);
        
        level_normal_bitmaps [i] = bitmap_load (level_normal_bitmap_names [i]);
        if (level_normal_bitmaps [i] == NULL) {
            fprintf (stderr, "level_init_module error : Cannot load normal bitmap %s\n", level_normal_bitmap_names [i]);
            return 1;
        }
        level_normal_textures [i] = bitmap_to_texture (level_normal_bitmaps [i]);
        bitmap_delete (level_normal_bitmaps [i]);
    }
    
    float mat_amb [4] = {0.2, 0.2, 0.2, 1.0},
          mat_spec [4] = {0.10, 0.10, 0.10, 1.0},
          mat_diff [4] = {0.8, 0.8, 0.8, 1.0};
    level_material = material_new ("level_material", mat_amb, mat_diff, mat_spec, 0.1, 0, 0);
    
    level_shader = NULL;
    if (shader_level == shader_level_high) {
        level_shader = shader_load ("data/shaders/level.vert", "data/shaders/level.frag");
    }
    else if (shader_level == shader_level_medium) {
        level_shader = shader_load ("data/shaders/level_medium.vert", "data/shaders/level_medium.frag");
    }
    else {
        level_shader = shader_load ("data/shaders/level_low.vert", "data/shaders/level_low.frag");
    }
    
    if (level_shader == NULL) {
        fprintf (stderr, "level_init_module error : Cannot load shader.\n");
        return 1;
    }
    
    level_display_nodes = display_nodes;
    
    return 0;
}

void level_deinit_module (void) {
    size_t i;

    for (i = 0; i < level_texture_max; i++) {
        glDeleteTextures (1, &(level_textures [i]));
        glDeleteTextures (1, &(level_normal_textures [i]));
    }
    
    material_delete (level_material);
    shader_delete (level_shader);
}

//--------------------------------------------------------------------------------------------------------------------//
// wall 
//--------------------------------------------------------------------------------------------------------------------//
void wall_eval_normal (wall_t* w) {
    float dx = w->b.x - w->a.x;
    float dz = w->b.z - w->a.z;
    
    float cos_a = cos (-3.14159265 * 0.5);
    float sin_a = sin (-3.14159265 * 0.5);
    
    w->normal.x = dx * cos_a - dz * sin_a;
    w->normal.z = dx * sin_a + dz * cos_a;
}

void wall_init (wall_t* w, int flags,
                float a_x, float a_z, float b_x, float b_z,
                int mtexture, int m_align_x, int m_align_y,
                int htexture, int h_align_x, int h_align_y
               ) {
    w->a.x = a_x;   w->a.z = a_z;
    w->b.x = b_x;   w->b.z = b_z; 

    wall_eval_normal (w);
    
    w->texture [wall_middle_texture].id = mtexture;
    w->texture [wall_middle_texture].align_x = m_align_x;
    w->texture [wall_middle_texture].align_y = m_align_y;
    
    w->texture [wall_up_texture].id = htexture;
    w->texture [wall_up_texture].align_x = h_align_x;
    w->texture [wall_up_texture].align_y = h_align_y;
    
    w->flags = flags;
}

int wall_get_intersection (wall_t* wall, float x1, float z1, float x2, float z2, float* xo, float* zo) {
    if (wall->flags & wall_blocking) {
        return line_get_intersection (x1, z1, x2, z2, wall->a.x, wall->a.z, wall->b.x, wall->b.z, xo, zo);
    }
    
    return 0;
}

void wall_middle_to_vbo (wall_t* wall,
                         GLuint data_vbo, GLuint index_vbo, 
                         GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                         GLintptr* it_index, GLuint* index_counter) {
    if (wall->texture [wall_middle_texture].id > -1) {
        float dx = wall->b.x - wall->a.x,
              dz = wall->b.z - wall->a.z;
        float line_length = sqrt (dx * dx + dz * dz);
                      
        //
        //----- Milieu -----
        //
        float v_m [18] = {
            wall->b.x, SECTOR_HEIGHT, wall->b.z,
            wall->b.x, 0, wall->b.z,
            wall->a.x, 0, wall->a.z,
            
            wall->a.x, 0, wall->a.z,
            wall->a.x, SECTOR_HEIGHT, wall->a.z,
            wall->b.x, SECTOR_HEIGHT, wall->b.z,
        };
        
        float vn_m [18] = {
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z,
            
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z
        };
        
        // On suppose que la résolution d'une texture de mur est de 512px/m.
        // On pose 1 mètre = 10.0 unité openGL.
        // Le facteur de répétition est :
        // taille en mètre de la ligne / taille en mètre de la texture
        float rep_x = line_length / 20;
        float rep_y = 1.0f;
        float vtc_m [12] = {
            rep_x, rep_y,
            rep_x, 0.0f,
            0.0f, 0.0f,
            
            0.0f, 0.0f,
            0.0f, rep_y,
            rep_x, rep_y
        };
        
        //float tex_scale_x = 
        
        // Premier triangle.
        glBindBuffer (GL_ARRAY_BUFFER, data_vbo);
        glBufferSubData (GL_ARRAY_BUFFER, (*it_v), 18 * sizeof (float), v_m);
        glBufferSubData (GL_ARRAY_BUFFER, (*it_vn), 18 * sizeof (float), vn_m);
        glBufferSubData (GL_ARRAY_BUFFER, (*it_vtc), 12 * sizeof (float), vtc_m);
        
        *it_v += 18 * sizeof (float);
        *it_vn += 18 * sizeof (float);
        *it_vtc += 12 * sizeof (float);
        
        GLuint index_m [6] = {(*index_counter), (*index_counter) + 1, (*index_counter) + 2,
                              (*index_counter) + 3, (*index_counter) + 4, (*index_counter) + 5};
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
        glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, (*it_index), 6 * sizeof (GLuint), index_m);
        
        wall->vbo_index [0] = *it_index;
        
        *it_index += 6 * sizeof (GLuint);
        *index_counter += 6;
    }
}

void wall_up_to_vbo (wall_t* wall, int height, 
                     GLuint data_vbo, GLuint index_vbo, 
                     GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                     GLintptr* it_index, GLuint* index_counter) {
    if (height > SECTOR_HEIGHT && wall->texture [wall_up_texture].id > -1) {
        float dx = wall->b.x - wall->a.x,
              dz = wall->b.z - wall->a.z;
        float line_length = sqrt (dx * dx + dz * dz);
                      
        //
        //----- Haut -----
        //
        float v_u [18] = {
            wall->b.x, height, wall->b.z,
            wall->b.x, SECTOR_HEIGHT, wall->b.z,
            wall->a.x, SECTOR_HEIGHT, wall->a.z,
            
            wall->a.x, SECTOR_HEIGHT, wall->a.z,
            wall->a.x, height, wall->a.z,
            wall->b.x, height, wall->b.z
        };
        
        float vn_u [18] = {
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z,
            
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z,
            wall->normal.x, 0, wall->normal.z
        };
        
        float rep_x = line_length / 20;
        float rep_y = (height - SECTOR_HEIGHT) / 20.0f;
        float vtc_u [12] = {
            rep_x, rep_y,
            rep_x, 0.0f,
            0.0f, 0.0f,
            
            0.0f, 0.0f,
            0.0f, rep_y,
            rep_x, rep_y
        };
    
    
        glBindBuffer (GL_ARRAY_BUFFER, data_vbo);
        glBufferSubData (GL_ARRAY_BUFFER, (*it_v), 18 * sizeof (float), v_u);
        glBufferSubData (GL_ARRAY_BUFFER, (*it_vn), 18 * sizeof (float), vn_u);
        glBufferSubData (GL_ARRAY_BUFFER, (*it_vtc), 12 * sizeof (float), vtc_u);
        
        *it_v += 18 * sizeof (float);
        *it_vn += 18 * sizeof (float);
        *it_vtc += 12 * sizeof (float);
        
        GLuint index_u [6] = {(*index_counter), (*index_counter) + 1, (*index_counter) + 2,
                              (*index_counter) + 3, (*index_counter) + 4, (*index_counter) + 5};
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
        glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, (*it_index), 6 * sizeof (GLuint), index_u);
        
        wall->vbo_index [1] = *it_index;
        
        *it_index += 6 * sizeof (GLuint);
        *index_counter += 6;
    }
}


void wall_render (wall_t* wall) {
    float tx = wall->b.x - wall->a.x;
    float tz = wall->b.z - wall->a.z;
    if (shader_level >= shader_level_medium) shader_update_tangent (level_shader, tx, 0, tz);

    if (wall->texture [wall_middle_texture].id > -1) {
        material_use (&level_materials [wall->texture [wall_middle_texture].id]);
        shader_update_color_map (level_shader, level_textures [wall->texture [wall_middle_texture].id]);
        if (shader_level >= shader_level_medium) shader_update_normal_map (level_shader, level_normal_textures [wall->texture [wall_middle_texture].id]);
        //shader_send_uniforms (level_shader, shader_send_color_map | shader_send_normal_map);
        
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*) NULL) + wall->vbo_index [0]);
    }
    if (wall->texture [wall_up_texture].id > -1) {
        material_use (&level_materials [wall->texture [wall_up_texture].id]);
        shader_update_color_map (level_shader, level_textures [wall->texture [wall_up_texture].id]);
        if (shader_level >= shader_level_medium) shader_update_normal_map (level_shader, level_normal_textures [wall->texture [wall_up_texture].id]);
        //shader_send_uniforms (level_shader, shader_send_color_map | shader_send_normal_map);
        
        glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, ((GLvoid*) NULL) + wall->vbo_index [1]);
    }
}

//--------------------------------------------------------------------------------------------------------------------//
// __sector_build_list_node
//--------------------------------------------------------------------------------------------------------------------//
typedef struct __sector_builder_list_node {
    float x, z;
    int   wall;
    struct __sector_builder_list_node* next;
} __sector_builder_list_node_t;

__sector_builder_list_node_t* __sector_builder_list_node_new (float x, float z, int wall) {
    __sector_builder_list_node_t* list = malloc (sizeof (__sector_builder_list_node_t));
    
    list->x = x;
    list->z = z;
    list->wall = wall;
    list->next = list;
    
    return list;
}

void __sector_builder_list_node_delete (__sector_builder_list_node_t* list) {
    free (list);
}

void __sector_builder_list_node_clear (__sector_builder_list_node_t* list) {
    __sector_builder_list_node_t* it, *tmp;
    
    it = list;
    
    do {
        tmp = it->next;
        free (it);
        it = tmp;
    } while (it != list);
}

void __sector_builder_list_node_add (__sector_builder_list_node_t* list, float x, float z, int wall) {
    __sector_builder_list_node_t* it;
    
    for (it = list; it->next != list; it = it->next) { }
    
    it->next = __sector_builder_list_node_new (x, z, wall);
    it->next->next = list;
}

#define __triangle_area(x1,y1,x2,y2,x3,y3) \
    0.5 * abs (x1 * y2 - x1 * y3 + x2 * y3 - x2 * y1 + x3 * y1 - x3 * y2);

int __sector_builder_list_node_check_point_in_triangle (__sector_builder_list_node_t* list, 
                                                        __sector_builder_list_node_t* a) {
    __sector_builder_list_node_t* it;

    float area_a, area_sub1, area_sub2, area_sub3;
    
    area_a = __triangle_area (a->x, a->z, a->next->x, a->next->z, a->next->next->x, a->next->next->z);

    it = list;
    do {
        if (it != a && it != a->next && it != a->next->next) {
            area_sub1 = __triangle_area (a->x, a->z, a->next->x, a->next->z, it->x, it->z);
            area_sub2 = __triangle_area (a->next->x, a->next->z, a->next->next->x, a->next->next->z, it->x, it->z);
            area_sub3 = __triangle_area (a->next->next->x, a->next->next->z, a->x, a->z, it->x, it->z);
            
            if (area_sub1 + area_sub2 + area_sub3 == area_a) {
                return 1;
            }
        }
        it = it->next;
    } while (it != list);
    
    return 0;
}



int __sector_builder_list_node_triangulize_once (__sector_builder_list_node_t* list, sub_sector_t* triangle) {
    float vx, vz, wx, wz;
    float angle;
    __sector_builder_list_node_t* it;
    __sector_builder_list_node_t* tmp;
    
    int i = 0;
    
    for (it = list; it->next != NULL && it->next->next != list->next; it = it->next) {
        //if (it->wall && it->next->wall) {
            // Vérifie que l'angle est convexe.
            vx = it->x - it->next->x;   
            vz = it->z - it->next->z;
            wx = it->next->next->x - it->next->x;
            wz = it->next->next->z - it->next->z;
            angle = atan2 (wz, wx) - atan2 (vz, vx);
            if (angle < 0) {
                angle += 2 * 3.14159265;
            }
            //printf ("%d : %f (%f %f; %f %f)\n", i, angle * (180 / 3.14), vx, vz, wx, wz);
            if (angle > 0 && angle < 3.14) {
                // Vérifie que le triangle formé par les 3 points ne contient aucun autre points du polygone.
                if (!__sector_builder_list_node_check_point_in_triangle (list, it)) {
                    triangle->v [0].x = it->x;              triangle->v [0].z = it->z;
                    triangle->v [1].x = it->next->x;        triangle->v [1].z = it->next->z;
                    triangle->v [2].x = it->next->next->x;  triangle->v [2].z = it->next->next->z;
                    
                    /*printf ("Triangle {(%f, %f), (%f, %f), (%f, %f)}\n", 
                            triangle->v [0].x, triangle->v [0].z, 
                            triangle->v [1].x, triangle->v [1].z, 
                            triangle->v [2].x, triangle->v [2].z);*/
                    
                    it->wall = 0;
                    tmp = it->next;
                    it->next = tmp->next;
                    __sector_builder_list_node_delete (tmp);
                    return 1;
                }
            }
        //}
        i++;
    }
    
    return 0;
}

size_t __sector_builder_list_node_triangulize (__sector_builder_list_node_t* list, sub_sector_t** triangles) {
    size_t       ntriangles = 0;
    sub_sector_t current;
    
    *triangles = NULL;
    while (__sector_builder_list_node_triangulize_once (list, &current)) {
        ntriangles++;
        *triangles = realloc (*triangles, ntriangles * sizeof (sub_sector_t));
        memcpy (&( (*triangles) [ntriangles - 1]), &current, sizeof (sub_sector_t));
    }
    
    return ntriangles;
}

//--------------------------------------------------------------------------------------------------------------------//
// node
//--------------------------------------------------------------------------------------------------------------------//
void node_add_link (node_t* a, node_t* b) {
    a->links [a->nlinks++] = b;
    b->links [b->nlinks++] = a;
}

int node_has_link (node_t* a, node_t* b) {
    size_t i;
    
    for (i = 0; i < a->nlinks; i++) {
        if (a->links [i] == b) {
            return 1;
        }
    }
    
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------//
// node_path
//--------------------------------------------------------------------------------------------------------------------//
node_path_t* node_path_new (node_t* node) {
    node_path_t* np = malloc (sizeof (node_path_t));
    
    np->node = node;
    np->next = NULL;
    
    return np;
}

void node_path_delete (node_path_t* path) {
    if (path) {
        node_path_delete (path->next);
        free (path);
    }
}

void node_path_pop_front (node_path_t** path) {
    node_path_t* tmp = (*path)->next;
    free (*path);
    *path = tmp;
}

void node_path_push_front (node_path_t** path, node_t* node) {
    node_path_t* np = node_path_new (node);
    np->next = *path;
    *path = np;
}

void node_path_pop_back (node_path_t** path) {
    if ((*path)->next == NULL) {
        free (*path);
        *path = NULL;
    }
    else {
        node_path_pop_back (&(*path)->next);
    }
}

void node_path_push_back (node_path_t** path, node_t* node) {
    if (*path == NULL) {
        *path = node_path_new (node);
    }
    else {
        node_path_push_back (&((*path)->next), node);
    }
}

int node_path_has_node (node_path_t* path, node_t* node) {
    if (path) {
        if (path->node == node) {
            return 1;
        }
        return node_path_has_node (path->next, node);
    }

    return 0;
}

static float __eval_node (node_t* node, node_t* last, node_t* end) {
    float dx, dz;
    float dist_ln = 0, 
          dist_ne = 0;
    
    // Distance avec le dernier noeud choisi.
    if (last != NULL) {
        dx = last->x - node->x;
        dz = last->z - node->z;
        dist_ln = dx * dx + dz * dz;
    }
    
    // Distance avec le point de destination.
    dx = end->x - node->x;
    dz = end->z - node->z;
    dist_ne = dx * dx + dz * dz;
    
    return dist_ln + dist_ne;
}

node_t* node_path_pick_best (node_path_t** path, node_t* last, node_t* end) {
    node_path_t** it, **fetched, *tmp;
    node_t* found = NULL;
    float ev, ev_min;
    fetched = NULL;
    
    for (it = path; *it != NULL; it = &(*it)->next) {
        if (fetched == NULL) {
            fetched = it;
            ev_min = __eval_node ((*fetched)->node, last, end);
        }
        else {
            ev = __eval_node ((*it)->node, last, end);
            if (ev < ev_min) {
                fetched = it;
                ev_min = ev;
            }
        }
    }
    
    if (*fetched != NULL) {
        found = (*fetched)->node;
        tmp = (*fetched)->next;
        free (*fetched);
        *fetched = tmp;
    }
    
    return found;
}

void node_path_print (node_path_t* np) {
    if (np) {
        printf ("(%f %f) ", np->node->x, np->node->z);
        node_path_print (np->next);
    }
    else {
        printf ("\n");
    }
}

node_path_t* node_path_find (node_t* start, node_t* end) {
    node_path_t* open_list = NULL;
    node_path_t* close_list = NULL;
    node_t* last = NULL, 
          * next;

    size_t i;
    
    
    
    last = start;
    
    if (start == end) {
        return NULL;
    }
    
    while (last != end) {
        //printf ("%f %f/%f %f\n", last->x, last->z, end->x, end->z);
        for (i = 0; i < last->nlinks; i++) {
                if (!node_path_has_node (close_list, last->links [i]) && !node_path_has_node (open_list, last->links [i])) {
                    node_path_push_front (&open_list, last->links [i]);
                }
        }
        
        next = node_path_pick_best (&open_list, last, end);
        if (next == NULL) {
            node_path_delete (open_list);
            node_path_delete (close_list);
            return NULL;
        }
        node_path_push_back (&close_list, next);
        //printf ("Adding %f %f\n", next->x, next->z);
        last = next;
        
        //fgetc (stdin);
    }    
    
    node_path_delete (open_list);
    
    node_path_pop_front (&close_list);
    
    return close_list;
}

//--------------------------------------------------------------------------------------------------------------------//
// sector
//--------------------------------------------------------------------------------------------------------------------//
void sector_init (sector_t* sector, int height, int floor_texture, int ceil_texture, 
                  float amb_r, float amb_g, float amb_b) {
    sector->height = height;
    light_init (&sector->light, GL_LIGHT7,
                0, 0, 0, 0,
                0, 0, 0,
                amb_r, amb_b, amb_g,
                0, 0, 0,
                0, 0);
    
    sector->size = 0;
    sector->n_sub_sectors = 0;
    
    sector->floor = floor_texture;
    sector->ceil = ceil_texture;
}

void sector_begin (sector_t* sector, level_vertex_t* vertex) {
    if (sector->size != 0) {
        fprintf (stderr, "sector_begin error : Sector has already been begun.\n");
        return;
    }
    
    memcpy (&(sector->walls [0].a), vertex, sizeof (level_vertex_t));
    sector->size++;
}

void sector_add_wall (sector_t* sector, level_vertex_t* vertex, int flags,
                      int mtexture, int m_align_x, int m_align_y,
                      int htexture, int h_align_x, int h_align_y) {
    if (sector->size == 0) {
        fprintf (stderr, "sector_add_wall error : Sector has not been begun.\n");
        return;
    }
    
    // TODO : regarder intersection avec un mur déjà posée.
    
    memcpy (&(sector->walls [sector->size - 1].b), vertex, sizeof (level_vertex_t));
    memcpy (&(sector->walls [sector->size].a), vertex, sizeof (level_vertex_t));
    
    wall_eval_normal (&sector->walls [sector->size - 1]);
    
    sector->walls [sector->size - 1].texture [0].id = mtexture;
    sector->walls [sector->size - 1].texture [0].align_x = m_align_x;
    sector->walls [sector->size - 1].texture [0].align_y = m_align_y;
    
    sector->walls [sector->size - 1].texture [1].id = htexture;
    sector->walls [sector->size - 1].texture [1].align_x = h_align_x;
    sector->walls [sector->size - 1].texture [1].align_y = h_align_y;
    
    sector->walls [sector->size - 1].flags = flags;
    
    sector->size++;
}

void sector_end (sector_t* sector, int flags,
                 int mtexture, int m_align_x, int m_align_y,
                 int htexture, int h_align_x, int h_align_y) {
    if (sector->size < 3) {
        fprintf (stderr, "sector_end error : Sector has not enough walls.\n");
        return;
    }
                 
    memcpy (&(sector->walls [sector->size - 1].b), &(sector->walls [0].a), sizeof (level_vertex_t));
    
    wall_eval_normal (&sector->walls [sector->size - 1]);
    
    sector->walls [sector->size - 1].texture [0].id = mtexture;
    sector->walls [sector->size - 1].texture [0].align_x = m_align_x;
    sector->walls [sector->size - 1].texture [0].align_y = m_align_y;
    
    sector->walls [sector->size - 1].texture [1].id = htexture;
    sector->walls [sector->size - 1].texture [1].align_x = h_align_x;
    sector->walls [sector->size - 1].texture [1].align_y = h_align_y;
    
    sector->walls [sector->size - 1].flags = flags;
    
    // Triangulisation.
    size_t i;
    __sector_builder_list_node_t* vlist = __sector_builder_list_node_new (sector->walls [0].a.x, sector->walls [0].a.z, 1);
    for (i = 1; i < sector->size; i++) {
        __sector_builder_list_node_add (vlist, sector->walls [i].a.x, sector->walls [i].a.z, 1);
    }
    
    sector->n_sub_sectors = __sector_builder_list_node_triangulize (vlist, &sector->sub_sectors);
    __sector_builder_list_node_clear (vlist);
}

wall_t* sector_has_intersection (sector_t* sector,
                              float x1, float z1, float x2, float z2,
                              float* xo, float* zo) {
    float txo, tzo;
    float last_dist;
    int nearest = -1;
    size_t i;
    
    for (i = 0; i < sector->size; i++) {
        if (wall_get_intersection (&sector->walls [i], x1, z1, x2, z2, &txo, &tzo)) {
            if (nearest == -1) {
                *xo = txo;
                *zo = tzo;
                last_dist = line_pow_length (x1, z1, *xo, *zo);
                nearest = (int) i;
            }
            else if (line_pow_length (x1, z1, txo, tzo) < last_dist) {
                *xo = txo;
                *zo = tzo;
                last_dist = line_pow_length (x1, z1, txo, tzo);
                nearest = (int) i;
            }
        }
    }
    
    if (nearest > -1) {
        return &sector->walls [nearest];
    }
    
    return NULL;
}

int sector_contains (sector_t* sector, float x, float z) {
    size_t i;
    sub_sector_t* s = sector->sub_sectors;
    
    for (i = 0; i < sector->n_sub_sectors; i++) {
        if (triangle_contains (s->v [0].x, s->v [0].z, s->v [1].x, s->v [1].z, s->v [2].x, s->v [2].z, x, z)) {
            return 1;
        }        
        s++;
    }
    
    return 0;
}

void sector_walls_to_vbo (sector_t* sector, GLuint data_vbo, GLuint index_vbo, 
                          GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                          GLintptr* it_index, GLuint* index_counter) {
    size_t i;     
    for (i = 0; i < sector->size; i++) {
        wall_middle_to_vbo (&sector->walls [i], data_vbo, index_vbo, it_v, it_vn, it_vtc, it_index, index_counter);
        wall_up_to_vbo (&sector->walls [i], sector->height, data_vbo, index_vbo, it_v, it_vn, it_vtc, it_index, index_counter);
    }
}

void sector_floor_to_vbo (sector_t* sector, GLuint data_vbo, GLuint index_vbo, 
                          GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                          GLintptr* it_index, GLuint* index_counter) {
    size_t i;
    level_vertex_t min, max;
    
    sector->vbo_floor_index = *it_index;
    
    // Recherche du plus petit rectangle contenant tous les points du secteur
    min.x = sector->walls [0].a.x;
    min.z = sector->walls [0].a.z;
    max.x = sector->walls [0].a.x;
    max.z = sector->walls [0].a.z;
    for (i = 0; i < sector->size; i++) {
        if (sector->walls [i].a.x < min.x) {
            min.x = sector->walls [i].a.x;
        }
        else if (sector->walls [i].a.x > max.x) {
            max.x = sector->walls [i].a.x;
        }
        
        if (sector->walls [i].a.z < min.z) {
            min.z = sector->walls [i].a.z;
        }
        else if (sector->walls [i].a.z > max.z) {
            max.z = sector->walls [i].a.z;
        }
    }
    
    
    // De même que pour les murs, une texture de sol a une résolution de 512px/m.
    // De même que pour les murs, un secteur a une taille en mètre de (max - min) / LEVEL_METER.
    float tex_res_x = level_texture_sizes [sector->floor].x / 512.0;
    float tex_res_z = level_texture_sizes [sector->floor].z / 512.0;
    
    // Ajout des sous-secteurs dans le vbo (plancher).
    float v [9];
    float vn [9] = {
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };
    float vtc [6];
    GLuint indexes [3];
    
    if (sector->floor > -1) {
        sub_sector_t* it = sector->sub_sectors;
        for (i = 0; i < sector->n_sub_sectors; i++) {
            v [0] = it->v [0].x;    v [1] = 0.0f;   v [2] = it->v [0].z;
            v [3] = it->v [1].x;    v [4] = 0.0f;   v [5] = it->v [1].z;
            v [6] = it->v [2].x;    v [7] = 0.0f;   v [8] = it->v [2].z;
            
            vtc [0] = ((it->v [0].x - min.x) / LEVEL_METER) / tex_res_x;
            vtc [1] = ((it->v [0].z - min.z) / LEVEL_METER) / tex_res_z;
            
            vtc [2] = ((it->v [1].x - min.x) / LEVEL_METER) / tex_res_x;
            vtc [3] = ((it->v [1].z - min.z) / LEVEL_METER) / tex_res_z;
            
            vtc [4] = ((it->v [2].x - min.x) / LEVEL_METER) / tex_res_x;
            vtc [5] = ((it->v [2].z - min.z) / LEVEL_METER) / tex_res_z;
            
            indexes [0] = (*index_counter)++;
            indexes [1] = (*index_counter)++;
            indexes [2] = (*index_counter)++;
            
            glBindBuffer (GL_ARRAY_BUFFER, data_vbo);
            glBufferSubData (GL_ARRAY_BUFFER, *it_v, 9 * sizeof (float), v);
            glBufferSubData (GL_ARRAY_BUFFER, *it_vn, 9 * sizeof (float), vn);
            glBufferSubData (GL_ARRAY_BUFFER, *it_vtc, 6 * sizeof (float), vtc);
            
            *it_v += 9 * sizeof (float);
            *it_vn += 9 * sizeof (float);
            *it_vtc += 6 * sizeof (float);
            
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
            glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, *it_index, 3 * sizeof (GLuint), indexes);
            
            *it_index += 3 * sizeof (GLuint);
            
            it++;
        }
    }
}

void sector_ceil_to_vbo (sector_t* sector, GLuint data_vbo, GLuint index_vbo, 
                          GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                          GLintptr* it_index, GLuint* index_counter) {
    size_t i;
    level_vertex_t min, max;
    
    sector->vbo_ceil_index = *it_index;
    
    // Recherche du plus petit rectangle contenant tous les points du secteur
    min.x = sector->walls [0].a.x;
    min.z = sector->walls [0].a.z;
    max.x = sector->walls [0].a.x;
    max.z = sector->walls [0].a.z;
    for (i = 0; i < sector->size; i++) {
        if (sector->walls [i].a.x < min.x) {
            min.x = sector->walls [i].a.x;
        }
        else if (sector->walls [i].a.x > max.x) {
            max.x = sector->walls [i].a.x;
        }
        
        if (sector->walls [i].a.z < min.z) {
            min.z = sector->walls [i].a.z;
        }
        else if (sector->walls [i].a.z > max.z) {
            max.z = sector->walls [i].a.z;
        }
    }
    
    float tex_res_x = level_texture_sizes [sector->floor].x / 512.0;
    float tex_res_z = level_texture_sizes [sector->floor].z / 512.0;
    
    // Ajout des sous-secteurs dans le vbo (plancher).
    float v [9];
    float vn [9] = {
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0
    };
    float vtc [6];
    GLuint indexes [3];
    
    if (sector->ceil > -1) {
        sub_sector_t* it = sector->sub_sectors;
        for (i = 0; i < sector->n_sub_sectors; i++) {
            v [0] = it->v [2].x;    v [1] = sector->height;   v [2] = it->v [2].z;
            v [3] = it->v [1].x;    v [4] = sector->height;   v [5] = it->v [1].z;
            v [6] = it->v [0].x;    v [7] = sector->height;   v [8] = it->v [0].z;
            
            vtc [4] = ((it->v [0].x - min.x) / LEVEL_METER) / tex_res_x;
            vtc [5] = ((it->v [0].z - min.z) / LEVEL_METER) / tex_res_z;
            
            vtc [2] = ((it->v [1].x - min.x) / LEVEL_METER) / tex_res_x;
            vtc [3] = ((it->v [1].z - min.z) / LEVEL_METER) / tex_res_z;
            
            vtc [0] = ((it->v [2].x - min.x) / LEVEL_METER) / tex_res_x;
            vtc [1] = ((it->v [2].z - min.z) / LEVEL_METER) / tex_res_z;
            
            indexes [0] = (*index_counter)++;
            indexes [1] = (*index_counter)++;
            indexes [2] = (*index_counter)++;
            
            glBindBuffer (GL_ARRAY_BUFFER, data_vbo);
            glBufferSubData (GL_ARRAY_BUFFER, *it_v, 9 * sizeof (float), v);
            glBufferSubData (GL_ARRAY_BUFFER, *it_vn, 9 * sizeof (float), vn);
            glBufferSubData (GL_ARRAY_BUFFER, *it_vtc, 6 * sizeof (float), vtc);
            
            *it_v += 9 * sizeof (float);
            *it_vn += 9 * sizeof (float);
            *it_vtc += 6 * sizeof (float);
            
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_vbo);
            glBufferSubData (GL_ELEMENT_ARRAY_BUFFER, *it_index, 3 * sizeof (GLuint), indexes);
            
            *it_index += 3 * sizeof (GLuint);
            
            it++;
        }
    }
}

void sector_render (sector_t* sector) {
    // Mise à jour du shader
    light_enable (&sector->light);
    shader_update_light_states (level_shader);
    
    size_t i;
    for (i = 0; i < sector->size; i++) {
        wall_render (&(sector->walls [i]));
    }
    
    shader_update_tangent (level_shader, 1, 0, 0);
    
    if (sector->floor > 0) {
        material_use (&level_materials [sector->floor]);
        shader_update_color_map (level_shader, level_textures [sector->floor]);
        shader_update_normal_map (level_shader, level_normal_textures [sector->floor]);
        
        glDrawElements (GL_TRIANGLES, sector->n_sub_sectors * 3, GL_UNSIGNED_INT, 
                        ((GLvoid*) NULL) + sector->vbo_floor_index);
    }
    
    if (sector->ceil > 0) {
        material_use (&level_materials [sector->ceil]);
        shader_update_color_map (level_shader, level_textures [sector->ceil]);
        shader_update_normal_map (level_shader, level_normal_textures [sector->ceil]);
        
        glDrawElements (GL_TRIANGLES, sector->n_sub_sectors * 3, GL_UNSIGNED_INT, 
                        ((GLvoid*) NULL) + sector->vbo_ceil_index);
    }
    
    light_disable (&sector->light);
}

//--------------------------------------------------------------------------------------------------------------------//
// level
//--------------------------------------------------------------------------------------------------------------------//
void level_init (level_t* level, size_t res_sectors, size_t res_nodes) {
    level->vbo_data = 0;
    level->vbo_index = 0;
    
    level->n_sectors = 0;
    level->res_sectors = res_sectors;
    level->sectors = malloc (res_sectors * sizeof (sector_t));
    
    level->n_nodes = 0;
    level->res_nodes = res_nodes;
    level->nodes = malloc (res_nodes * sizeof (node_t));
}

void level_deinit (level_t* level) {
    free (level->sectors);
    free (level->nodes);
}

sector_t* level_add_sector (level_t* level, int height, int floor_texture, int ceil_texture, 
                            float amb_r, float amb_g, float amb_b) {
    if (level->n_sectors == level->res_sectors) {
        fprintf (stderr, "level_add_sector error : Cannot add more sectors.\n");
        return NULL;
    }
    
    sector_t* s = &(level->sectors [level->n_sectors++]);
    
    sector_init (s, height, floor_texture, ceil_texture, amb_r, amb_g, amb_b);
    
    return s;             
}

wall_t* level_has_intersection (level_t* level,
                                float x1, float z1, float x2, float z2,
                                float* xo, float* zo) {
    float txo, tzo;
    wall_t* nearest = NULL, *wall;
    float last_dist;
    size_t i;
    
    // Looking with the sector which contains the (x1, z1) point first.
    
    for (i = 0; i < level->n_sectors; i++) {
        if ((wall = sector_has_intersection (&level->sectors [i], x1, z1, x2, z2, &txo, &tzo)) != NULL) {
            if (nearest == NULL) {
                nearest = wall;
                last_dist = line_pow_length (x1, z1, txo, tzo);
                *xo = txo;
                *zo = tzo;
            }
            else if (line_pow_length (x1, z1, txo, tzo) < last_dist) {
                nearest = wall;
                last_dist = line_pow_length (x1, z1, txo, tzo);
                *xo = txo;
                *zo = tzo;
            }
        }
    }
    
    return nearest;
}

int level_contains (level_t* level, float x, float z) {
    size_t i;
    
    for (i = 0; i < level->n_sectors; i++) {
        if (sector_contains (&level->sectors [i], x, z)) {
            return 1;
        }
    }
    
    return 0;
}

sector_t* level_sector_at (level_t* level, float x, float z) {
    size_t i;
    
    for (i = 0; i < level->n_sectors; i++) {
        if (sector_contains (&level->sectors [i], x, z)) {
            return &level->sectors [i];
        }
    }
    
    return NULL;
}

int level_has_node (level_t* level, float x, float z) {
    size_t i;
    
    for (i = 0; i < level->n_nodes; i++) {
        if (x == level->nodes [i].x && z == level->nodes [i].z) {
            return 1;
        }
    }
    
    return 0;
}

void level_generate_sub_nodes (level_t* level, node_t* node) {
    float ix, iz;
    
    node->sector = level_sector_at (level, node->x, node->z);
    
    if (level->n_nodes >= level->res_nodes) {
        fprintf (stderr, "level_genreate_sub_nodes error : Not enough reserved space for nodes.\n");
        return;
    }
    
    // Node en haut.
    if (    !level_has_intersection (level, node->x, node->z, node->x, node->z + LEVEL_METER * 1.5, &ix, &iz) 
        &&   level_contains (level, node->x, node->z + LEVEL_METER)) {
        if (!level_has_node (level, node->x, node->z + LEVEL_METER)) {
            level->nodes [level->n_nodes].x = node->x;
            level->nodes [level->n_nodes].z = node->z + LEVEL_METER;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node en haut à gauche.
    if (    !level_has_intersection (level, node->x, node->z, node->x + LEVEL_METER * 1.5, node->z + LEVEL_METER * 1.5, &ix, &iz) 
        &&   level_contains (level, node->x + LEVEL_METER, node->z + LEVEL_METER)) {
        if (!level_has_node (level, node->x + LEVEL_METER, node->z + LEVEL_METER)) {
            level->nodes [level->n_nodes].x = node->x + LEVEL_METER;
            level->nodes [level->n_nodes].z = node->z + LEVEL_METER;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node à gauche.
    if (    !level_has_intersection (level, node->x, node->z, node->x + LEVEL_METER * 1.5, node->z, &ix, &iz) 
        &&   level_contains (level, node->x + LEVEL_METER, node->z)) {
        if (!level_has_node (level, node->x + LEVEL_METER, node->z)) {
            level->nodes [level->n_nodes].x = node->x + LEVEL_METER;
            level->nodes [level->n_nodes].z = node->z;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node en bas à gauche.
    if (    !level_has_intersection (level, node->x, node->z, node->x + LEVEL_METER * 1.5, node->z - LEVEL_METER * 1.5, &ix, &iz) 
        &&   level_contains (level, node->x + LEVEL_METER, node->z - LEVEL_METER)) {
        if (!level_has_node (level, node->x + LEVEL_METER, node->z - LEVEL_METER)) {
            level->nodes [level->n_nodes].x = node->x + LEVEL_METER;
            level->nodes [level->n_nodes].z = node->z - LEVEL_METER;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node en bas.
    if (    !level_has_intersection (level, node->x, node->z, node->x, node->z - LEVEL_METER * 1.5, &ix, &iz) 
        &&   level_contains (level, node->x, node->z - LEVEL_METER)) {
        if (!level_has_node (level, node->x, node->z - LEVEL_METER)) {
            level->nodes [level->n_nodes].x = node->x;
            level->nodes [level->n_nodes].z = node->z - LEVEL_METER;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node en bas à droite.
    if (    !level_has_intersection (level, node->x, node->z, node->x - LEVEL_METER * 1.5, node->z - LEVEL_METER * 1.5, &ix, &iz) 
        &&   level_contains (level, node->x - LEVEL_METER, node->z - LEVEL_METER)) {
        if (!level_has_node (level, node->x - LEVEL_METER, node->z - LEVEL_METER)) {
            level->nodes [level->n_nodes].x = node->x - LEVEL_METER;
            level->nodes [level->n_nodes].z = node->z - LEVEL_METER;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node à droite.
    if (    !level_has_intersection (level, node->x, node->z, node->x - LEVEL_METER * 1.5, node->z, &ix, &iz) 
        &&   level_contains (level, node->x - LEVEL_METER, node->z)) {
        if (!level_has_node (level, node->x - LEVEL_METER, node->z)) {
            level->nodes [level->n_nodes].x = node->x - LEVEL_METER;
            level->nodes [level->n_nodes].z = node->z;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
    
    // Node à droite en haut.
    if (    !level_has_intersection (level, node->x, node->z, node->x - LEVEL_METER * 1.5, node->z + LEVEL_METER * 1.5, &ix, &iz) 
        &&   level_contains (level, node->x - LEVEL_METER, node->z + LEVEL_METER)) {
        if (!level_has_node (level, node->x - LEVEL_METER, node->z + LEVEL_METER)) {
            level->nodes [level->n_nodes].x = node->x - LEVEL_METER;
            level->nodes [level->n_nodes].z = node->z + LEVEL_METER;
            level->nodes [level->n_nodes].nlinks = 0;
            //node_add_link (&level->nodes [level->n_nodes], node);
            level->n_nodes++;
            level_generate_sub_nodes (level, &level->nodes [level->n_nodes - 1]);
        }
    }
}

void level_generate_nodes (level_t* level) {
    /*
    size_t n_index = 0;
    
    for (i = 0; i < level->n_sectors && n_index < level->res_nodes; i++) {
        sector_generate_nodes (&level->sectors [i], level->nodes, &n_index);
    }
    
    level->n_nodes = n_index;
    */
    
    // La technique est la suivante :
    level->nodes [0].x = 0;
    level->nodes [0].z = 0;
    level->nodes [0].nlinks = 0;
    level->nodes [0].sector = &level->sectors [0];
    level->n_nodes++;
    level_generate_sub_nodes (level, &level->nodes [0]);
    
    
    // Connexions.
    float dx, dz, dist;
    size_t i, j;
    for (i = 0; i < level->n_nodes; i++) {
        for (j = 0; j < level->n_nodes; j++) {
            dx = level->nodes [j].x - level->nodes [i].x;
            dz = level->nodes [j].z - level->nodes [i].z;
            dist = dx * dx + dz * dz;
            if (dist == LEVEL_METER * LEVEL_METER || dist == 2 * LEVEL_METER * LEVEL_METER) {
                if (!level_has_intersection (level, level->nodes [j].x, 
                                             level->nodes [j].z, level->nodes [i].x, level->nodes [i].z, &dx, &dz)) {
                    if (!node_has_link (&level->nodes [j], &level->nodes [i])) {
                        node_add_link (&level->nodes [j], &level->nodes [i]);
                    }
                }
            }
        }
    }
}

void level_render (level_t* level, float ex, float ey, float ez, float dx, float dy, float dz) {
    size_t i;
    
    // Setting up shader.
    shader_select (level_shader);
    shader_update_light_states (level_shader);
    if (shader_level == shader_level_high) shader_update_camera_world_position (level_shader, ex, ey, ez);
    
    for (i = 0; i < 3; i++) {
        if (robots [i] != NULL) {
            shader_update_light_world_position (level_shader, &robots [i]->light);
        }
    }
    shader_update_light_world_position (level_shader, &level->light);
    shader_send_uniforms (level_shader, shader_send_lights_wpos);
    
    material_use (level_material);
    //glColor3ub (255, 255, 255);
    
    
    // Setting up VBOs
    glBindBuffer (GL_ARRAY_BUFFER, level->vbo_data);
    glVertexPointer (3, GL_FLOAT, 0, NULL);
    glNormalPointer (GL_FLOAT, 0, ((GLvoid*) NULL) + level->offset_vn);
    glTexCoordPointer (2, GL_FLOAT, 0, ((GLvoid*) NULL) + level->offset_vtc);
    
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, level->vbo_index);
    
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    
    for (i = 0; i < level->n_sectors; i++) {
        if (level->sectors [i].ceil == -1) {
            light_enable (&level->light);
            //printf ("%f %f %f\n", level_shader->d_lights_wpos [6 * 3], level_shader->d_lights_wpos [6 * 3 + 1], level_shader->d_lights_wpos [6 * 3 + 2]);
        }
        sector_render (&level->sectors [i]);
        if (level->sectors [i].ceil == -1) {
            light_disable (&level->light);
        }
    }
    
    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState (GL_NORMAL_ARRAY);
    glDisableClientState (GL_TEXTURE_COORD_ARRAY);
    
    material_use (&default_material);
    
    shader_release ();

    if (level_display_nodes) {
        shader_select (base_shader);
        glColor3f (1, 1, 1);
        size_t j;
        for (i = 0; i < level->n_nodes; i++) {
            glPushMatrix ();
            glTranslatef (level->nodes [i].x, 5, level->nodes [i].z);
            glutSolidCube (1.0);
            glPopMatrix ();
            
            glBegin (GL_LINES);
            for (j = 0; j < level->nodes [i].nlinks; j++) {
                    glVertex3f (level->nodes [i].x, 5, level->nodes [i].z);
                    glVertex3f (level->nodes [i].links [j]->x, 5, level->nodes [i].links [j]->z);
            }
            glEnd ();
        }
    }
}

node_t* level_node_at (level_t* level, float x, float z) {
    node_t* found = NULL;
    node_t* current = level->nodes;
    size_t i;
    
    float dx, dz, dist_min, dist;
    
    for (i = 0; i < level->n_nodes; i++) {
        dx = x - current->x;
        dz = z - current->z;
        dist = dx * dx + dz * dz;
        
        if (found == NULL) {
            found = current;
            dist_min = dist;
        }
        else if (dist < dist_min) {
           found = current;
           dist_min = dist;
        }        
    
        current++;
    }
    
    return found;
}


void level_to_vbo (level_t* level) {
    
    // Counting number of vertex
    size_t i, j;
    size_t nvertex = 0;
    for (i = 0; i < level->n_sectors; i++) {
        for (j = 0; j < level->sectors [i].size; j++) {
            if (level->sectors [i].walls [j].texture [wall_middle_texture].id > -1) {
                nvertex += 6;
            }
            if (level->sectors [i].walls [j].texture [wall_up_texture].id > -1) {
                nvertex += 6;
            }
        }
        
        if (level->sectors [i].floor > -1) {
            nvertex += level->sectors [i].n_sub_sectors * 3;
        }
        
        if (level->sectors [i].ceil > -1) {
            nvertex += level->sectors [i].n_sub_sectors * 3;
        }
    }
    
    
    // VBOs generation.
    glGenBuffers (1, &level->vbo_data);
    glGenBuffers (1, &level->vbo_index);
    
    glBindBuffer (GL_ARRAY_BUFFER, level->vbo_data);
    glBufferData (GL_ARRAY_BUFFER, nvertex * 8 * sizeof (float), NULL, GL_STATIC_DRAW);
    
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, level->vbo_index);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, nvertex * sizeof (GLuint), NULL, GL_STATIC_DRAW);
    
    // VBOs filling
    GLintptr it_v = 0,
             it_vn = nvertex * 3 * sizeof (float),
             it_vtc = nvertex * 6 * sizeof (float),
             it_index = 0;
    GLuint   index_counter = 0;
    
    level->offset_v = 0;
    level->offset_vn = it_vn;
    level->offset_vtc = it_vtc;
    
    for (i = 0; i < level->n_sectors; i++) {
        sector_walls_to_vbo (&level->sectors [i], level->vbo_data, level->vbo_index, &it_v, &it_vn, &it_vtc, &it_index, &index_counter);
        sector_floor_to_vbo (&level->sectors [i], level->vbo_data, level->vbo_index, &it_v, &it_vn, &it_vtc, &it_index, &index_counter);
        sector_ceil_to_vbo (&level->sectors [i], level->vbo_data, level->vbo_index, &it_v, &it_vn, &it_vtc, &it_index, &index_counter);
    }
}


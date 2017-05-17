#ifndef _level_h_
#define _level_h_

#include <GL/glew.h>
#include <GL/gl.h>

#include "3d/light.h"

typedef size_t wall_id;
typedef size_t sector_id;
typedef size_t node_id;

enum {
    level_texture_tech_wall = 0,
    level_texture_stone_wall,
    level_texture_tech_floor,
    level_texture_stone_floor,
    level_texture_ceil,
    level_texture_max
};

#define LEVEL_METER 10.0f

int level_init_module (int display_nodes);

void level_deinit_module (void);

//--------------------------------------------------------------------------------------------------------------------//
// level_vertex
//--------------------------------------------------------------------------------------------------------------------//
typedef struct level_vertex {
    float x, z;
} level_vertex_t;

//--------------------------------------------------------------------------------------------------------------------//
// wall 
//--------------------------------------------------------------------------------------------------------------------//
enum {
    wall_middle_texture = 0,
    wall_up_texture
};

typedef struct wall_texture {
    int     id;
    int     align_x;
    int     align_y;
} wall_texture_t;

typedef enum {
    wall_blocking   = 0x1,
    wall_trigger    = 0x2,
} wall_flags_t;

typedef struct wall {
    level_vertex_t  a, b;
    level_vertex_t  normal;
    wall_texture_t  texture [2];
    int flags;
    
    GLintptr vbo_index [2];
} wall_t;

void wall_init (wall_t* w, int flags,
                float a_x, float a_z, float b_x, float b_z,
                int mtexture, int m_align_x, int m_align_y,
                int htexture, int h_align_x, int h_align_y
               );

// Return 1 if the line {(x1, z1), (x2, z2)} is intersecting the wall.
// In this case, xo and zo (if not NULL) will take the value of the intersection.
int wall_get_intersection (wall_t* wall, float x1, float z1, float x2, float z2, float* xo, float* zo);

// Add the middle part of a wall to a VBO.
void wall_middle_to_vbo (wall_t* wall, GLuint data_vbo, GLuint index_vbo, 
                         GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                         GLintptr* it_index, GLuint* index_counter);

// Add the up part of a wall to a VBO.
void wall_up_to_vbo (wall_t* wall, int height, GLuint data_vbo, GLuint index_vbo, 
                     GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                     GLintptr* it_index, GLuint* index_counter);

void wall_render (wall_t* wall);

//--------------------------------------------------------------------------------------------------------------------//
// node
//--------------------------------------------------------------------------------------------------------------------//
typedef struct sector sector_t;

typedef struct node {
    float x, z;
    size_t nlinks;
    struct node* links [16];
    sector_t*   sector;
} node_t;

void node_add_link (node_t* a, node_t* b);

//--------------------------------------------------------------------------------------------------------------------//
// node_path
//--------------------------------------------------------------------------------------------------------------------//
typedef struct node_path {
    node_t* node;
    struct node_path* next;
} node_path_t;

node_path_t* node_path_new (node_t* node);

void node_path_delete (node_path_t* path);

void node_path_pop_front (node_path_t** path);

node_path_t* node_path_find (node_t* start, node_t* end);


//--------------------------------------------------------------------------------------------------------------------//
// sector 
//--------------------------------------------------------------------------------------------------------------------//

#define SECTOR_MAX_WALLS    32
#define SECTOR_HEIGHT       20

// The sub sectors will be usefull to handle floor and ceil rendering.
typedef struct sub_sector {
    level_vertex_t  v [3];
} sub_sector_t;


struct sector {
    int height;
    light_t light;

    size_t          size;
    wall_t          walls [SECTOR_MAX_WALLS];
    
    size_t          n_sub_sectors;
    sub_sector_t*   sub_sectors;
    
    int floor, ceil;
    GLintptr    vbo_floor_index, vbo_ceil_index;
};

// initialize a new sector.
// Set floor_texture or ceil_tetxure to -1 to not display them.
void sector_init (sector_t* sector, int height, int floor_texture, int ceil_texture, 
                  float amb_r, float amb_g, float amb_b);

// Begin the construction of a sector at the given position.
// A sector must be formed in the clockwise.
void sector_begin (sector_t* sector, level_vertex_t* vertex);

// Add a wall from previous position to 'vertex' position. 
// Set mtexture or htexture to -1 to make them transparent.
// flags can be 'wall_blocking'. This will make the wall collisionnable.
void sector_add_wall (sector_t* sector, level_vertex_t* vertex, int flags,
                      int mtexture, int m_align_x, int m_align_y,
                      int htexture, int h_align_x, int h_align_y);

// Close the sector (make a wall from the last point to the first point).
void sector_end (sector_t* sector, int flags,
                 int mtexture, int m_align_x, int m_align_y,
                 int htexture, int h_align_x, int h_align_y);

// Looking if a point is in the sector.                 
int sector_contains (sector_t* sector, float x, float z);

// Check for an intersection of the [(x1, z1), (x2, z2)] line and a wall of the sector.
// Return the wall with the nearest intersection point, or NULL if no walls is in intersection.
wall_t* sector_has_intersection (sector_t* sector,
                                 float x1, float z1, float x2, float z2,
                                 float* xo, float* zo);

// Functions below permits to store wall and sector informations to VBO.
// - data_vbo and index_vbo are vbo identifiers for data and indexes.
// - it_v, it_vn, it_vtc are pointer to current position of vertex, normals and texture coordinates in the data VBO.
// - it_index is the current position of index in the index VBO.
// - index_counter is the number of indexes currently set.
void sector_walls_to_vbo (sector_t* sector, GLuint data_vbo, GLuint index_vbo, 
                          GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                          GLintptr* it_index, GLuint* index_counter);

void sector_floor_to_vbo (sector_t* sector, 
                          GLuint data_vbo, GLuint index_vbo, 
                          GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                          GLintptr* it_index, GLuint* index_counter);

void sector_ceil_to_vbo (sector_t* sector, 
                         GLuint data_vbo, GLuint index_vbo, 
                         GLintptr* it_v, GLintptr* it_vn, GLintptr* it_vtc, 
                         GLintptr* it_index, GLuint* index_counter);




//--------------------------------------------------------------------------------------------------------------------//
// level 
//--------------------------------------------------------------------------------------------------------------------//
typedef struct level {
    GLuint  vbo_data, vbo_index;
    GLintptr    offset_v, offset_vn, offset_vtc;
    
    size_t      n_sectors, res_sectors;
    sector_t*   sectors;
    
    size_t      n_nodes, res_nodes;
    node_t*     nodes;
    
    light_t     light;
} level_t;

// Initialize a new level.
// res_sectors and res_nodes are the maximum number of nodes and sectors the level can contains.
void level_init (level_t* level, size_t res_sectors, size_t res_nodes);

// Delete a level.
void level_deinit (level_t* level);

// check if a point is in the level.
int level_contains (level_t* level, float x, float z);

// Return the sector which contains given point. NULL if not found.
sector_t* level_sector_at (level_t* level, float x, float z);

// Add a sector to the level. Return the adress of the new created sector.
sector_t* level_add_sector (level_t* level, int height, int floor_texture, int ceil_texture, 
                            float amb_r, float amb_g, float amb_b);
                            
// Return the nearest wall of (x1, z1) in intersection with [(x1, z1), (x2, z2)].
wall_t* level_has_intersection (level_t* level,
                                float x1, float z1, float x2, float z2,
                                float* xo, float* zo);

// Generates node of the level.
// For now this function will fail if there is no sector at (0, 0) (first node is ever there).
void level_generate_nodes (level_t* level);

// Generate vbo from a level.
void level_to_vbo (level_t* level);

// Render a level.
void level_render (level_t* level, float cx, float cy, float cz, float dx, float dy, float dz);

// return nearest node of the level from (x, y).
node_t* level_node_at (level_t* level, float x, float y);

#endif


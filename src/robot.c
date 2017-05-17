#include "robot.h"
#include "3d/shader.h"
#include "math/matrix.h"
#include "3d/light.h"

#include "laser.h"
#include "scene/scene_data.h"

#include "smoke.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

vbo_mesh_t*  robot_vbo_mesh         = NULL;

GLuint       robot_textures [robot_color_max]   = {0};
GLuint       robot_normal_texture = 0;
char         robot_bitmap_names [robot_color_max] [64] = {
    "data/Shamgar.bmp",
    "data/ShamgarRed.bmp",
    "data/ShamgarGreen.bmp"
};

ALuint       robot_sounds [robot_sound_max] = {0};
char         robot_sound_names [robot_sound_max] [64] = {
    "data/laser_1.wav",
    "data/laser_2.wav",
    "data/laser_3.wav",
    "data/laser_4.wav",
    "data/laser_5.wav",
    "data/laser_6.wav",
    "data/mort_robot.wav",
    "data/apparition_robot.wav"
};

shader_t*   robot_shader = NULL;

//--------------------------------------------------------------------------------------------------------------------//
// Initialization
//--------------------------------------------------------------------------------------------------------------------//
int robot_init_module () {
    mesh_t* robot_mesh             = NULL;

    robot_mesh = mesh_load_obj ("data/Shamgar.obj");
	if (robot_mesh == NULL) {
		return 1;
	}
	
	robot_vbo_mesh = vbo_mesh_new (robot_mesh);
	if (robot_vbo_mesh == NULL) {
	    return 1;
	}
	
	mesh_delete (robot_mesh);
	
	bitmap_t* bmp;
	
	size_t i;
	for (i = 0; i < robot_color_max; i++) {
	    bmp = bitmap_load (robot_bitmap_names [i]);
	    if (bmp == NULL) {
	        fprintf (stderr, "robot_init_module error : Cannot load bitmap %s\n", robot_bitmap_names [i]);
	        return 1;
	    }
	    
	    robot_textures [i] = bitmap_to_texture (bmp);
	    bitmap_delete (bmp);
	}
	
	bmp = bitmap_load ("data/Shamgar_normal.bmp");
	if (bmp == NULL) {
	    fprintf (stderr, "robot_init_module error : Cannot load normal map %s\n", "data/Shamgar_normal.bmp");
	    return 1;
	}
	robot_normal_texture = bitmap_to_texture (bmp);
	bitmap_delete (bmp);
	
	
	// Loading sounds.
	for (i = 0; i < robot_sound_max; i++) {
	    robot_sounds [i] = audio_load_sound (robot_sound_names [i]);
	    if (robot_sounds [i] == 0) {
	        fprintf (stderr, "robot_init_module error : Cannot load sound %s\n", robot_sound_names [i]);
	        return 1;
	    }
	}
	
	robot_shader = shader_load ("data/shaders/mesh.vert", "data/shaders/mesh.frag");
	if (robot_shader == NULL) {
	    fprintf (stderr, "robot_init_module error : Cannot load shader\n");
	    return 1;
	}
	
	return 0;
}

void robot_close_module () {
    if (robot_vbo_mesh) {
        vbo_mesh_delete (robot_vbo_mesh);
        robot_vbo_mesh = NULL;
    }
    
    size_t i;
    for (i = 0; i < robot_color_max; i++) {
        glDeleteTextures (1, &(robot_textures [i]));
    }
    glDeleteTextures (1, &robot_normal_texture);
    
    for (i = 0; i < robot_sound_max; i++) {
        audio_delete_sound (robot_sounds [i]);
    }
}



//--------------------------------------------------------------------------------------------------------------------//
// General
//--------------------------------------------------------------------------------------------------------------------//
robot_t*    robot_new (float x, float y, float z, float ay, GLuint texture, light_t* light) {
    robot_t* robot = malloc (sizeof (robot_t));

    // Création du squelette.
    float p [4] = {0.0, 0.0, 0.0, 0.0};
	float s [4] = {0.0, 0.0, 0.0, 0.0};
	float a [4] = {0.0, 0.0, 0.0, 0.0};

    // Corps
    p [0] = x; p [1] = y; p [2] = z;
    a [1] = ay;
    robot->pelvis = vbo_rig_new (NULL,  p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.015_pelvis"), 3);
    a [1] = 0.0;

    // Ces points sont la position de l'articulation du torse et du bassin.
    // Chaque point de liaison est relatif au point (0, 0, 0) du modèle.
    p [0] = 0.0f;   p [1] = -1.1f;    p [2] = -0.4f;
    robot->torso = vbo_rig_new ( robot->pelvis, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.016_torso"), 3);
    vbo_rig_set_child (robot->pelvis, 0, robot->torso);
    
    p [0] = 0.0f;    p [1] = 4.0f;    p [2] = 0.0f;
    robot->head = vbo_rig_new ( robot->torso, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.002_head"), 1);
    vbo_rig_set_child (robot->torso, 0, robot->head);
    
    
    // Bras gauche.
    p [0] =  2.63f;    p [1] =  2.5f;    p [2] =  -0.65f;
    robot->left_shoulder = vbo_rig_new ( robot->torso, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.001_left_shoulder"), 1);
    vbo_rig_set_child (robot->torso, 1, robot->left_shoulder);
    
    p [0] =  3.1f;    p [1] =  0.25f;    p [2] =  -1.2f;
    robot->left_elbow = vbo_rig_new ( robot->left_shoulder, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.014_left_elbow"), 3);
    vbo_rig_set_child (robot->left_shoulder, 0, robot->left_elbow);
    
    p [0] = -3.0;    p [1] = -1.7;    p [2] =  0.0f;
    robot->left_pincher1 = vbo_rig_new ( robot->left_elbow, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.012_left_pincher_1"), 0);
    vbo_rig_set_child (robot->left_elbow, 0, robot->left_pincher1);
    
    p [0] = -3.6;    p [1] = -2.1;    p [2] =  -0.8f;
    robot->left_pincher2 = vbo_rig_new ( robot->left_elbow, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.011_left_pincger_2"), 0);
    vbo_rig_set_child (robot->left_elbow, 1, robot->left_pincher2);
    
    p [0] = 3.514;    p [1] = -2.6140;    p [2] = -0.1482;
    robot->blaster = vbo_rig_new (robot->left_elbow, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.003_Blaster"), 0);
    vbo_rig_set_child (robot->left_elbow, 2, robot->blaster);
    
    
    // Bras droit
    p [0] = -2.8256;    p [1] = 2.3262;    p [2] = -0.6489;
    robot->right_shoulder = vbo_rig_new (  robot->torso, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.005_right_shoulder"), 1);
    vbo_rig_set_child (robot->torso, 2, robot->right_shoulder);
    
    p [0] = -3.2;    p [1] = 0.2882;    p [2] = 1.2338;
    robot->right_elbow = vbo_rig_new (robot->right_shoulder, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.010_right_elbow"), 2);
    vbo_rig_set_child (robot->right_shoulder, 0, robot->right_elbow);
    
    p [0] = -3.5435;    p [1] = -2.0721;    p [2] = 0.6807;
    robot->pincher1 =  vbo_rig_new (robot->right_elbow, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.008_pincher1"), 0);
    vbo_rig_set_child (robot->right_elbow, 0, robot->pincher1);
    
    p [0] = -3.0275;    p [1] = -1.7972;    p [2] = -0.1644;
    robot->pincher2 =  vbo_rig_new (robot->right_elbow, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.007_pincher2"), 0);
    vbo_rig_set_child (robot->right_elbow, 1, robot->pincher2);
                      
    // Jambe gauche         
    p [0] = 1.0917;    p [1] = -3.0123;    p [2] = 0.2107;
    robot->left_upper_leg = vbo_rig_new (robot->pelvis, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.017_left_upper_leg"), 1);
    vbo_rig_set_child (robot->pelvis, 1, robot->left_upper_leg);
    
    p [0] = 1.1440;    p [1] = -2.9160;    p [2] = 0.3185;
    robot->left_knee = vbo_rig_new (robot->left_upper_leg, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.013_left_knee"), 1);
    vbo_rig_set_child (robot->left_upper_leg, 0, robot->left_knee);
    
    p [0] = 1.3163;    p [1] = -5.0874;    p [2] = 0.3185;
    robot->left_foot = vbo_rig_new (robot->left_knee, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh_left_foot"), 0);
    vbo_rig_set_child (robot->left_knee, 0, robot->left_foot);
    
    
    // Jambe droite
    p [0] = -1.1155;    p [1] = -2.8205;    p [2] = 0.3185;
    robot->right_upper_leg = vbo_rig_new (robot->pelvis, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.009_right_upper_leg"), 1);
    vbo_rig_set_child (robot->pelvis, 2, robot->right_upper_leg);
    
    p [0] = -1.0742;    p [1] = -3.1914;    p [2] = 0.3185;
    robot->right_knee = vbo_rig_new (robot->right_upper_leg, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.004_right_knee"), 1);
    vbo_rig_set_child (robot->right_upper_leg, 0, robot->right_knee);
    
    p [0] = -1.4040;    p [1] = -4.9225;    p [2] = 0.3585;
    robot->right_foot = vbo_rig_new (robot->right_knee, p, s, a, robot_vbo_mesh, vbo_mesh_get_face_group (robot_vbo_mesh, "Mesh.006_right_foot"), 0);
    vbo_rig_set_child (robot->right_knee, 0, robot->right_foot);
    
    // Other datas
    robot->life = 100;
    robot->damages = 0;
    robot->points = 0;
    
    robot->action = ROBOT_ACTION_WAIT;
    
    robot->actions [ROBOT_ACTION_WAIT]          = robot_action_wait;
    robot->actions [ROBOT_ACTION_WALK]          = robot_action_walk;
    robot->actions [ROBOT_ACTION_SHOOT]         = robot_action_shoot;
    robot->actions [ROBOT_ACTION_DIE]           = robot_action_die;
    
    robot->animations [ROBOT_ACTION_WAIT]          = robot_animation_wait;
    robot->animations [ROBOT_ACTION_WALK]          = robot_animation_walk;
    robot->animations [ROBOT_ACTION_SHOOT]         = robot_animation_shoot;
    robot->animations [ROBOT_ACTION_DIE]           = robot_animation_die;
    
    robot->ticker = 0;
    
    robot->repeat_shoot = 0;
    robot->repeat_walk = 0;
    robot->dx = 0; robot->dz = 1;
    robot->sx = 0; robot->sz = 0;
    robot->target = NULL;
    
    
    robot->texture = texture;
    memcpy (&robot->light, light, sizeof (light_t));
    
    robot->current_node = NULL;
    robot->goal = NULL;
    
    robot->audio_source = audio_create_source (4.0);
    
    return robot;
}

void robot_delete (robot_t* robot) {
    vbo_rig_delete (robot->pelvis);
    audio_delete_source (robot->audio_source);
    free (robot);
}


void robot_render (robot_t* robot) {
    shader_update_color_map (robot_shader, robot->texture);

    sector_t* s = robot->current_node->sector;
    if (s->ceil == -1) {
        light_enable (&scene_level.light);
    }
    light_enable (&s->light);
    
    glPushMatrix ();
    vbo_rig_get_model_matrix (robot->pelvis);
    shader_update_world_matrix (robot_shader);
    glPopMatrix ();
    
    //printf ("%f %f %f\n", s->light.ambient [0], s->light.ambient [1], s->light.ambient [2]);
    shader_update_light_states (robot_shader);

    vbo_rig_render (robot->pelvis);
    
    light_disable (&s->light);
    if (s->ceil == -1) {
        light_disable (&scene_level.light);
    }
}

void robot_update (robot_t* robot) {
    if (robot->damages < 100 && robot->damages > 0 && (rand () % 10) == 0) {
        robot->damages--;
    }

    // Looking for death
    if (robot->damages >= robot->life && robot->action != ROBOT_ACTION_DIE) {
        robot_request_action (robot, ROBOT_ACTION_DIE);
        audio_play_sound (robot_sounds [robot_sound_die], robot->audio_source, 2.5);
    }

    if (robot->actions [robot->action] (robot)) {
        robot_request_action (robot, ROBOT_ACTION_WAIT);
    }
    if (robot->animations [robot->action] (robot)) {
        robot->ticker = 0;
    }

    robot->ticker++;
    
    audio_source_update (robot->audio_source, 
                         robot->pelvis->position [0], robot->pelvis->position [1], robot->pelvis->position [2], 
                         0, 0, 0);
    
    robot_update_node (robot);
}


//--------------------------------------------------------------------------------------------------------------------//
// Robot manipulation
//--------------------------------------------------------------------------------------------------------------------//
static void __reset_rig_angles (vbo_rig_t* rig) {
    size_t i;
    rig->angles [0] = 0;
    rig->angles [1] = 0;
    rig->angles [2] = 0;
    
    for (i = 0; i < rig->nchilds; i++) {
        if (rig->childs [i] != NULL) __reset_rig_angles (rig->childs [i]);
    }
}

void robot_reset_rig (robot_t* robot) {
    robot->pelvis->angles [0] = 0.0f;
    robot->pelvis->angles [2] = 0.0f;
    robot->pelvis->position [1] = 10;
    __reset_rig_angles (robot->torso);
    __reset_rig_angles (robot->left_upper_leg);
    __reset_rig_angles (robot->right_upper_leg);
}

void robot_request_action (robot_t* robot, int action) {
    if (action != robot->action) {
        robot_reset_rig (robot);
        robot->action = action;
        robot->ticker = 0;
        light_disable (&robot->light);
    }
}

void robot_get_position (robot_t* robot, float* x, float* z) {
    *x = robot->pelvis->position [0];
    *z = robot->pelvis->position [2];
}


void robot_respawn (robot_t* robot) {
    robot_reset_rig (robot);
    robot->action = ROBOT_ACTION_WAIT;
    robot->ticker = 0;

    int n = rand () % (scene_level.n_nodes);
    robot->pelvis->position [0] = scene_level.nodes [n].x;
    robot->pelvis->position [2] = scene_level.nodes [n].z;
    robot->current_node = &scene_level.nodes [n];
    robot->goal = NULL;
    
    audio_source_update (robot->audio_source, 
                         robot->pelvis->position [0], robot->pelvis->position [1], robot->pelvis->position [2], 
                         0, 0, 0);
    audio_play_sound (robot_sounds [robot_sound_reborn], robot->audio_source, 2.0);
}

void robot_rotate_to (robot_t* robot, float x, float z) {
    float rx, rz;
    float dx, dz;
    float dist;
    float angle;
    
    robot_get_position (robot, &rx, &rz);
    
    dx = x - rx;
    dz = z - rz;
    dist = sqrt (dx * dx + dz * dz);
    robot->dx = dx / dist;
    robot->dz = dz / dist;
    
    
    
    angle = atan2 (-dz, dx);
    robot->pelvis->angles [1] = 90 + angle * 180 / 3.14159265;
}

void robot_rotate_of (robot_t* robot, float angle) {
    matrix_t R;
    vector_t axe = {0, 1, 0, 0};
    
    // Rotation du bassin.
    robot->pelvis->angles [1] += angle * 180 / 3.14159265;
    
    // Calcul du vecteur directeur.
    vector_t dir = {0, 0, 1, 0};
    matrix_init_rotation (&R, &axe, robot->pelvis->angles [1] * 3.14159265 / 180);
    matrix_apply_ex (&R, &dir);
    robot->dx = dir.x;
    robot->dz = dir.z;
}

int robot_has_intersection (robot_t* robot, float x1, float z1, float x2, float z2, float* xo, float* zo) {
    // Use the method at : http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect


    // Calcul du vecteur orthogonal de la ligne (imaginer un cylindre autour du robot).
    vector_t diff;
    diff.x = x2 - x1;
    diff.y = 0;
    diff.z = z2 - z1;
    
    matrix_t R;
    vector_t axe = {0.0f, 1.0f, 0.0f, 0.0f};
    matrix_init_rotation (&R, &axe, 3.14156265 * 0.5);
    vector_t diff_o;
    matrix_apply (&R, &diff, &diff_o);
    vector_normalize (&diff_o);
    vector_scalar_mul (&diff_o, 8.0f);
    
    float qx = robot->pelvis->position [0] - diff_o.x * 0.5;
    float qz = robot->pelvis->position [2] - diff_o.z * 0.5;
    float sx = diff_o.x;
    float sz = diff_o.z;
    
    float px = x1,
          pz = z1,
          rx = x2 - x1,
          rz = z2 - z1;
    
    float qpx = qx - px,
          qpz = qz - pz;
    float pqx = px - qx,
          pqz = pz - qz;
    float r_cross_s = rx * sz - rz * sx;
    float qp_cross_s = (qpx * sz - qpz * sx);
    float qp_cross_r = (qpx * rz - qpz * rx);
    
    float qp_dot_r = qpx * rx + qpz * rz,
          pq_dot_s = pqx * sx + pqz * sz;
    
    float t = 0, u = 0;
    
    if (r_cross_s == 0 && qp_cross_r == 0) {
        if (   (0 <= qp_dot_r && qp_dot_r <= (rx * rx + rz * rz))
            || (0 <= pq_dot_s && pq_dot_s <= (rx * rx + rz * rz))) {
            if (xo != NULL) *xo = -1;
            if (zo != NULL) *zo = -1;
            return 2;
        }
        else {
            return 0;
        }
    }
    else if (r_cross_s == 0) {
        return 0;
    }
    else {
        t = qp_cross_s / r_cross_s;
        u = qp_cross_r / r_cross_s;
        
        if (t >= 0 && t <= 1.0 && u >= 0 && u <= 1.0) {
            if (xo != NULL) *xo = px + t * rx;
            if (zo != NULL) *zo = pz + t * rz;
            
            return 1;
        }
    }
    
    return 0;
}

int robot_is_other_at_node (robot_t* robot, node_t* node) {
    size_t i;
    
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != robot && robots [i]->current_node == node) {
            return 1;
        }
    }
    
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------//
// Game functions
//--------------------------------------------------------------------------------------------------------------------//
void robot_update_node (robot_t* robot) {
    // Check only on connexions.
    node_t* nearest = robot->current_node;
    float dist_min, dist;
    float dx, dz;
    float px, pz;
    
    robot_get_position (robot, &px, &pz);
    dx = px - nearest->x;
    dz = pz - nearest->z;
    dist_min = dx * dx + dz * dz;
    
    size_t i;
    for (i = 0; i < robot->current_node->nlinks; i++) {
        dx = px - robot->current_node->links [i]->x;
        dz = pz - robot->current_node->links [i]->z;
        dist = dx * dx + dz * dz;
        if (dist < dist_min) {
            nearest = robot->current_node->links [i];
            dist_min = dist;
        }
    }
    
    robot->current_node = nearest;
}

static robot_t* __first_robot_on_line (robot_t* src, float x1, float z1, float x2, float z2, float* ix, float* iz, float* dist) {
    robot_t* found = NULL;
    float cur_dist;
    size_t i;
    float cix, ciz;
    
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != src && robots [i]->action != ROBOT_ACTION_DIE) {
            if (robot_has_intersection (robots [i], x1, z1, x2, z2, &cix, &ciz)) {
                cur_dist = (x1 - cix) * (x1 - cix) + (z1 - ciz) * (z1 - ciz);
                if (found == NULL || cur_dist < *dist) {
                    found = robots [i];
                    *dist = cur_dist;
                    *ix = cix;
                    *iz = ciz;
                } 
            }
        }
    }
    
    return found;
}

static wall_t* __first_wall_on_line (float x1, float z1, float x2, float z2, float* ix, float* iz, float* dist) {
    wall_t* wall = level_has_intersection (&scene_level, x1, z1, x2, z2, ix, iz);
    if (wall != NULL) {
        *dist = (*ix - x1) * (*ix - x1) + (*iz - z1) * (*iz - z1);
    }
    
    return wall;
}

void robot_play_laser_sound (robot_t* robot) {
    int which = rand () % 6;
    audio_play_sound (robot_sounds [which], robot->audio_source, 1.0);
}

void robot_shoot (robot_t* robot) {
    vector_t start, stop;
    vector_t axe_y = {0, 1, 0, 0};
    matrix_t R;
    float rand_angle;

    start.x = 2;
    start.y = 2.0;
    start.z = 9;
    start.w = 0;
    
    matrix_init_rotation (&R, &axe_y, robot->pelvis->angles [1] * 3.14159265 / 180);
    matrix_apply_ex (&R, &start);
    
    vector_add (&start, (vector_t*) robot->pelvis->position);

    // Allume la lumière
    vector_copy (&(robot->light.position), &start);
    light_enable (&robot->light);
    
    // 
    
    stop.x = robot->dx * 1000.0f;
    stop.y = 0.0f;
    stop.z = robot->dz * 1000.0f;
    stop.w = 0.0f;
    rand_angle = 0;//(-50 + rand () % 100) * 0.1 * 3.14159265 / 180;
    matrix_init_rotation (&R, &axe_y, rand_angle);
    matrix_apply_ex (&R, &stop);
    vector_add (&stop, &start);
    
    float dist_wall, dist_robot = 0;
    float wix, wiz, rix = 0, riz = 0;
    wall_t* col_wall;
    robot_t* col_robot;
    
    col_wall = __first_wall_on_line (start.x, start.z, stop.x, stop.z, &wix, &wiz, &dist_wall);
    col_robot = __first_robot_on_line (robot, start.x, start.z, stop.x, stop.z, &rix, &riz, &dist_robot);
    if ((col_wall && !col_robot) || (col_wall && col_robot && dist_wall < dist_robot)) {
        stop.x = wix;
        stop.z = wiz;
        
        // Ajout de l'impact sur le mur.
        laser_impact_add (stop.x, stop.y, stop.z, col_wall->b.x - col_wall->a.x, col_wall->b.z - col_wall->a.z,
                    robot->light.diffuse [0] * 255, robot->light.diffuse [1] * 255, robot->light.diffuse [2] * 255);
        laser_sparks_add (stop.x, stop.y, stop.z, col_wall->b.x - col_wall->a.x, col_wall->b.z - col_wall->a.z,
                    robot->light.diffuse [0] * 255, robot->light.diffuse [1] * 255, robot->light.diffuse [2] * 255);
    }
    else if ((!col_wall && col_robot) || (col_wall && col_robot && dist_robot < dist_wall)) {
        stop.x = rix;
        stop.z = riz;
        
        laser_sparks_add (stop.x, stop.y, stop.z, robot->dz, -robot->dx,
                    robot->light.diffuse [0] * 255, robot->light.diffuse [1] * 255, robot->light.diffuse [2] * 255);
        
        float damages = 5 + rand () % 10;
        col_robot->damages += damages;
        if (col_robot->damages >= 100 && col_robot->damages - damages < 100) {
            robot->points++;
        }
    }
    
    laser_add (start.x, start.y, start.z, stop.x, stop.y, stop.z, 
               robot->light.diffuse [0] * 255, robot->light.diffuse [1] * 255, robot->light.diffuse [2] * 255);
    
    if (rand () % 2) smoke_add (start.x, start.y, start.z);
    if (rand () % 2) smoke_add (stop.x - (stop.x - start.x) * 0.1, stop.y, stop.z - (stop.z - start.z) * 0.1);
    
    robot_play_laser_sound (robot);
}

robot_t* robot_find_target (robot_t* robot) {
    size_t i;
    
    float x1, z1, x2, z2, ix, iz;
    
    robot_get_position (robot, &x1, &z1);
    
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != NULL && robots [i] != robot && robots [i]->action != ROBOT_ACTION_DIE && rand () % 2) {
            robot_get_position (robots [i], &x2, &z2);
            if (!level_has_intersection (&scene_level, x1, z1, x2, z2, &ix, &iz)) {
                return robots [i];
            }
        }
    }
    
    return NULL;
}

void robot_choose_destination (robot_t* robot) {

    node_t* dest;
    
    do {
        dest = &scene_level.nodes [rand () % scene_level.n_nodes];
    } while (dest == robot->current_node);
    
    robot->goal = node_path_find (robot->current_node, dest);
}

void robot_ai (robot_t* robot) {
    robot_t* target;

    if (robot->action == ROBOT_ACTION_DIE) {
    
    }
    else if (robot->action == ROBOT_ACTION_WAIT) {
        // Fetching target.
        target = robot_find_target (robot);
        if (target != NULL) {
            robot->target = target;
            robot_request_action (robot, ROBOT_ACTION_SHOOT);
            robot->repeat_shoot = 1;
        }
        // Moving.
        else if (robot->action != ROBOT_ACTION_WALK) {
            robot_choose_destination (robot);
            if (robot->goal != NULL && robot->goal->node != robot->current_node) {
                robot_rotate_to (robot, robot->goal->node->x, robot->goal->node->z);
                robot_request_action (robot, ROBOT_ACTION_WALK);
                robot->sx = robot->dx;
                robot->sz = robot->dz;
                robot->repeat_walk = 1;
            }
        }
    }
    else if (robot->action == ROBOT_ACTION_WALK) {
        if (robot->current_node == robot->goal->node) {
            node_path_pop_front (&(robot->goal));
            if (robot->goal != NULL) {
                robot_rotate_to (robot, robot->goal->node->x, robot->goal->node->z);
                robot->sx = robot->dx;
                robot->sz = robot->dz;
            }
            else {
                robot->repeat_walk = 0;
            }
        }
    
        target = robot_find_target (robot);
        if (rand () % 40 == 0 && target != NULL) {
            robot->target = target;
            robot_request_action (robot, ROBOT_ACTION_SHOOT);
        }
        else if (robot->goal != NULL) {
            if (robot_is_other_at_node (robot, robot->goal->node)) {
                node_path_delete (robot->goal);
                robot->goal = NULL;
                robot_request_action (robot, ROBOT_ACTION_WAIT);
            }
        }
    }
    else if (robot->action == ROBOT_ACTION_SHOOT) {
        float tx, tz;
        float rx, rz;
        float ix, iz;
        robot_get_position (robot, &rx, &rz);
        robot_get_position (robot->target, &tx, &tz);
        
        float dist = (rx - tx) * (rx - tx) + (rz - tz) * (rz - tz);
        
        if (level_has_intersection (&scene_level, rx, rz, tx, tz, &ix, &iz) || dist < 100 || robot->target->damages >= robot->target->life) {
            robot_request_action (robot, ROBOT_ACTION_WAIT);
        }
        else {
            if (rand () % 100 == 0) {
                robot->repeat_shoot = 0;
            }
            robot_rotate_to (robot, tx -4 + rand () % 8, tz -4 + rand () % 8);
        }
    }
}




//--------------------------------------------------------------------------------------------------------------------//
// Actions functions
//--------------------------------------------------------------------------------------------------------------------//
//
// Wait animation.
//
int robot_action_wait (robot_t* robot) {
    return 1;
}

int robot_animation_wait (robot_t* robot) {
    if (robot->ticker < 15) {
        robot->head->angles [1] -= 2.0f;
    }
    else if (robot->ticker < 45) {
        robot->head->angles [1] += 2.0f;
    }
    else {
        robot->head->angles [1] -= 2.0f;
    }
    
    return robot->ticker == 60;
}


//
// Walk Animation
//
void robot_stop_walk (robot_t* robot) {
    if (robot->action == ROBOT_ACTION_WALK) {
        robot->action = ROBOT_ACTION_WAIT;
        
        robot_reset_rig (robot);
        robot->ticker = 0;
    }
}

static int __robots_are_adjacents (robot_t* a, robot_t* b) {
    size_t i;
    
    if (a->current_node == b->current_node) {
        return 1;
    }
    
    for (i = 0; i < a->current_node->nlinks; i++) {
        if (a->current_node->links [i] == b->current_node) {
            return 1;
        }
    }
    
    return 0;
}

int robot_action_walk (robot_t* robot) {
    // Looking for collision with a wall.
    float rx, rz, ix, iz;
    
    robot_get_position (robot, &rx, &rz);
    if (level_has_intersection (&scene_level, rx, rz, rx + robot->sx * 2.0, rz + robot->sz * 2.0, &ix, &iz)) {
        node_path_delete (robot->goal);
        robot->goal = NULL;
        return 1;
    }
    
    // Looking for collision with an other robot.
    size_t i;
    for (i = 0; i < number_of_robot; i++) {
        if (robots [i] != robot && __robots_are_adjacents (robot, robots [i])) {
            if (robot_has_intersection (robots [i], rx, rz, rx + robot->sx * 5, rz + robot->sz * 5, &ix, &iz)) {
                node_path_delete (robot->goal);
                robot->goal = NULL;
                return 1;
            }
        }
    }
    
    robot->pelvis->position [0] += robot->sx;
    robot->pelvis->position [2] += robot->sz;
    
    return (robot->repeat_walk == 0);
}

int robot_animation_walk (robot_t* robot) {
    /*
    __right_leg_anim_walk (robot, robot->ticker);
    __left_leg_anim_walk (robot, robot->ticker);
    __right_arm_anim_walk (robot, robot->ticker);
    __left_arm_anim_walk (robot, robot->ticker);
    */
    
    // Pas gauche 2
    if (robot->ticker <= 15) {
        robot->right_knee->angles [0] -= 3.0f;
        robot->right_foot->angles [0] += 3.0f;
        
        robot->left_knee->angles [0] += 1.5f;
        robot->left_foot->angles [0] += 1.5f;
        
        robot->pelvis->position [1] = 10.2 - sin (robot->ticker * 3.14159265 / 60.0);
        
        robot->right_shoulder->angles [0] -= 3.0f;
        robot->left_shoulder->angles [0] += 3.0f;
    }
    // Pas droit 1
    else if (robot->ticker <= 30) {
        robot->right_knee->angles [0] += 3.0f;
        robot->right_foot->angles [0] -= 3.0f;
        
        robot->left_knee->angles [0] -= 1.5f;
        robot->left_foot->angles [0] -= 1.5f;
        
        robot->pelvis->position [1] = 10.2 - sin (robot->ticker * 3.14159265 / 60.0);
        
        robot->right_shoulder->angles [0] += 3.0f;
        robot->left_shoulder->angles [0] -= 3.0f;
    }
    // Pas droit 2
    else if (robot->ticker <= 45) {
        robot->right_knee->angles [0] += 1.5f;
        robot->right_foot->angles [0] += 1.5f;
        
        robot->left_knee->angles [0] -= 3.0f;
        robot->left_foot->angles [0] += 3.0f;
        
        robot->pelvis->position [1] = 10.2 - sin (robot->ticker * 3.14159265 / 60.0);
        
        robot->right_shoulder->angles [0] += 3.0f;
        robot->left_shoulder->angles [0] -= 3.0f;
    }
    // Pas gauche 1
    else if (robot->ticker <= 60) {
        robot->right_knee->angles [0] -= 1.5f;
        robot->right_foot->angles [0] -= 1.5f;
        
        robot->left_knee->angles [0] += 3.0f;
        robot->left_foot->angles [0] -= 3.0f;
        
        robot->pelvis->position [1] = 10.2 - sin (robot->ticker * 3.14159265 / 60.0);
        
        robot->right_shoulder->angles [0] -= 3.0f;
        robot->left_shoulder->angles [0] += 3.0f;
    }
    
    return robot->ticker == 60;
}


//
// Shoot Animation
//  Making the ticker repeat step two until mouse released.
//
int robot_action_shoot (robot_t* robot) {    
    if (robot->ticker == 12) {
        robot_shoot (robot);
    }
    else if (robot->ticker == 17) {
        light_disable (&robot->light);
    }
    
    if (robot->ticker == 20 && robot->repeat_shoot) {
        robot->ticker = 10;
    }

    return robot->ticker == 29;
}

int robot_animation_shoot (robot_t* robot) {
    // Step one : ccw rotation (from face) and arm positioning.
    if (robot->ticker > 0 && robot->ticker < 10) {
        robot->left_shoulder->angles [0] -= 6.5;
        robot->left_shoulder->angles [1] -= 4.0;
        
        robot->right_shoulder->angles [0] -= 6.5;
        robot->right_shoulder->angles [1] += 2.5;
        
        robot->torso->angles [1] += 4.0;
        robot->head->angles [1] -= 4.0;
    }
    
    // Step two : weapon up and down
    else if (robot->ticker <= 20) {
        if (robot->ticker == 11) {
            robot->torso->angles [0] -= 5.0f;
            robot->left_shoulder->angles [0] -= 3.0f;
            robot->right_shoulder->angles [0] -= 3.0f;
        }
        else if (robot->ticker > 11 && robot->ticker < 20) {
            robot->torso->angles [0] += 5.0f / 8;
            robot->left_shoulder->angles [0] += 3.0f / 8;
            robot->right_shoulder->angles [0] += 3.0f / 8;
        }
    }
    
    // Step three : cw rotation and arm repositioning
    else {
        robot->left_shoulder->angles [0] += 7.5;
        robot->left_shoulder->angles [1] += 4.0;
        
        robot->right_shoulder->angles [0] += 7.5;
        robot->right_shoulder->angles [1] -= 2.5;
        
        robot->torso->angles [1] -= 4.0;
        robot->head->angles [1] += 4.0;
    }
    
    return robot->ticker == 29;
}

//
// Die Animation
//
int robot_action_die (robot_t* robot) {
/*
    if (robot->ticker == 200) {
        robot->damages = 0;
        robot->requested_action = -1;
        robot_reset_rig (robot);
        robot->goal_node = NULL;
        
                    
        node_t* node = &(robot_level->nodes [rand () % robot_level->nnodes]);
        robot->pelvis->position [0] = node->x;
        robot->pelvis->position [2] = node->z;
        robot->current_node = node;
        
        audio_play_sound (robot_reborn_sound, robot->audio_source);
    }
*/
    if (robot->ticker == 200) {
        robot->damages = 0;
        robot_respawn (robot);
    }

    return robot->ticker == 200;
}

int robot_animation_die (robot_t* robot) {
    matrix_t R;

    // Le robot plie les genoux et le haut des jambes.
    if (robot->ticker < 15) {        
        robot->left_knee->angles [0] -= 3.33;
        robot->right_knee->angles [0] -= 3.33;
        
        robot->left_foot->angles [0] += 10;
        robot->right_foot->angles [0] += 10;
        
        robot->left_shoulder->angles [0] -= 1.0;
        
        robot->pelvis->position [1] -= 0.30;
    }
    // Puis il tombe face contre sol
    else if (robot->ticker > 30 && robot->ticker < 45) {   
        robot->left_knee->angles [0] += 3.33;
        robot->right_knee->angles [0] += 3.33;
        
        robot->left_foot->angles [0] -= 10;
        robot->right_foot->angles [0] -= 10;
    
        robot->pelvis->angles [0] += 6;
        robot->pelvis->position [1] -= 0.30;
        
        robot->left_shoulder->angles [0] -= 9.5;
        
        vector_t axe = {0, 1, 0, 0};
        matrix_init_rotation (&R, &axe, robot->pelvis->angles [1] * 3.14159265 / 180);
        
        vector_t vi = {0, 0,  0.30, 0}, vo;
        matrix_apply (&R, &vi, &vo);
        
        vector_add ((vector_t*) robot->pelvis->position, &vo);
    }
    
    return robot->ticker == 201;
}




// TODO line_has_intersection (...)


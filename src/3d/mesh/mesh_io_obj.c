#include "3d/mesh/mesh_io.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#if defined(WIN64)
#define SIZE_T_FORMAT		"%lu"
#elif defined(WIN32)
#define SIZE_T_FORMAT		"%u"
#else
#define SIZE_T_FORMAT		"%zu"
#endif

static char* __strip_spaces (char* s) {
    while (isspace (*s)) {
        s++;
    }
    
    return s;
}

int obj_char_is_space (char c) {
    return (c == ' ' || c == '\r' || c == '\n' || c == '\0' || c == '\t');
}

int obj_line_is_comment (char* s) {
    return (__strip_spaces (s) [0] == '#');
}

int obj_line_is_vertex (char* s) {
    s = __strip_spaces (s);
    return (s [0] == 'v' && obj_char_is_space (s [1]));
}

int obj_line_is_vertex_normal (char* s) {
    s = __strip_spaces (s);
    return (s [0] == 'v' && s [1] == 'n' && obj_char_is_space (s [2]));
}

int obj_line_is_vertex_tex_coord (char* s) {
    s = __strip_spaces (s);
    return (s [0] == 'v' && s [1] == 't' && obj_char_is_space (s [2]));
}

int obj_line_is_face (char* s) {
    s = __strip_spaces (s);
    return (s [0] == 'f' && obj_char_is_space (s [1]));
}

int obj_line_is_group (char* s) {
    s = __strip_spaces (s);
    return (s [0] == 'g' && obj_char_is_space (s [1]));
}

int obj_line_is_smooth (char* s) {
    s = __strip_spaces (s);
    return (s [0] == 's' && obj_char_is_space (s [1]));
}

int obj_line_is_matlib (char* s) {
    s = __strip_spaces (s);
    return (!strncmp (s, "mtllib", 6) && obj_char_is_space (s [6]));
}

int obj_line_is_usemtl (char* s) {
    s = __strip_spaces (s);
    return (!strncmp (s, "usemtl", 6) && obj_char_is_space (s [6]));
}


int obj_line_parse_vertex (char* s, vertex_t* v) {
    int n = sscanf (s, "v %f %f %f", &v->x, &v->y, &v->z);
    if (n < 3) {
        fprintf (stderr, "obj_line_parse_vertex error : '%s' doesn't seem to be a valid vertex line.\n", s);
        return 1;
    }
    
    return 0;
}

int obj_line_parse_vertex_normal (char* s, vertex_normal_t* vn) {
    int n = sscanf (s, "vn %f %f %f", &vn->x, &vn->y, &vn->z);
    if (n < 3) {
        fprintf (stderr, "obj_line_parse_vertex_normal error : '%s' doesn't seem to be a valid vertex normal line.\n", 
                         s);
        return 1;
    }
    
    return 0;
}

int obj_line_parse_vertex_tex_coord (char* s, vertex_tex_coord_t* vtc) {
    int n = sscanf (s, "vt %f %f", &vtc->u, &vtc->v);
    if (n < 2) {
        fprintf (stderr, "obj_line_parse_vertex_tex_coord error : '%s' doesn't seem to be a valid vertex tex_coord "
                         "line.\n", s);
        return 1;
    }
    
    return 0;
}

int obj_line_parse_face (char* s, face_t* face) {
    int n;
    
    if ((n = sscanf (s, "f "SIZE_T_FORMAT"/"SIZE_T_FORMAT"/"SIZE_T_FORMAT" "SIZE_T_FORMAT"/"SIZE_T_FORMAT"/"SIZE_T_FORMAT" "SIZE_T_FORMAT"/"SIZE_T_FORMAT"/"SIZE_T_FORMAT"",
                        &face->v_id [0], &face->vtc_id [0], &face->vn_id [0],
                        &face->v_id [1], &face->vtc_id [1], &face->vn_id [1],
                        &face->v_id [2], &face->vtc_id [2], &face->vn_id [2])) == 9) {
        face->flags = face_use_vertex_normal | face_use_vertex_tex_coord;
    }
    else if ((n = sscanf (s, "f "SIZE_T_FORMAT"//"SIZE_T_FORMAT" "SIZE_T_FORMAT"//"SIZE_T_FORMAT" "SIZE_T_FORMAT"//"SIZE_T_FORMAT"",
                        &face->v_id [0], &face->vn_id [0],
                        &face->v_id [1], &face->vn_id [1],
                        &face->v_id [2], &face->vn_id [2]) == 6)) {
        face->flags = face_use_vertex_normal;
    }
    else if ((n = sscanf (s, "f "SIZE_T_FORMAT"/"SIZE_T_FORMAT" "SIZE_T_FORMAT"/"SIZE_T_FORMAT" "SIZE_T_FORMAT"/"SIZE_T_FORMAT"",
                        &face->v_id [0], &face->vtc_id [0],
                        &face->v_id [1], &face->vtc_id [1],
                        &face->v_id [2], &face->vtc_id [2])) == 6) {
        face->flags = face_use_vertex_tex_coord;
    }
    else if ((n = sscanf (s, "f "SIZE_T_FORMAT" "SIZE_T_FORMAT" "SIZE_T_FORMAT"", &face->v_id [0], &face->v_id [1], &face->v_id [2])) == 6) {
        face->flags = 0;
    }
    else {
        fprintf (stderr, "obj_line_parse_face error : '%s' doesn't seem to be a valid face line.\n", s);
        return 1;
    }
    
    face->v_id [0]--; face->v_id [1]--; face->v_id [2]--;
    face->vn_id [0]--; face->vn_id [1]--; face->vn_id [2]--;
    face->vtc_id [0]--; face->vtc_id [1]--; face->vtc_id [2]--;
    
    return 0;
}

int obj_line_parse_group (char* s, char* name) {
    int n = sscanf (s, "g %255[^\r\n]", name);
    if (n == 0) {
        fprintf (stderr, "obj_line_parse_group error : '%s' doesn't seem to be a valid group line.\n", s);
        return 1;
    }
    
    return 0;
}

int obj_line_parse_matlib (char* s, char* name) {
    int n = sscanf (s, "mtllib %255[^\r\n]", name);
    if (n == 0) {
        fprintf (stderr, "obj_line_parse_matlib error : '%s' doesn't seem to be a valid matlib line.\n", s);
        return 1;
    }
    
    return 0;
}

int obj_line_parse_usemtl (char* s, char* name) {
    int n = sscanf (s, "usemtl %255[^\r\n]", name);
    if (n == 0) {
        fprintf (stderr, "obj_line_parse_usemtl error : '%s' doesn't seem to be a valid usemtl line.\n", s);
        return 1;
    }
    
    return 0;
}


void obj_count_elements (FILE* f, size_t* n_v, size_t* n_vn, size_t* n_vtc, size_t* n_f, size_t* n_fg) {
    fseek (f, SEEK_SET, 0);
    
    char buffer [512];
    
    while (!feof (f)) {
        fgets (buffer, 511, f);
        if (obj_line_is_vertex (buffer))                (*n_v)++;
        else if (obj_line_is_vertex_normal (buffer))    (*n_vn)++;
        else if (obj_line_is_vertex_tex_coord (buffer)) (*n_vtc)++;
        else if (obj_line_is_face (buffer))             (*n_f)++;
        else if (obj_line_is_group (buffer))            (*n_fg)++;
    }
}

static void __get_base_path (const char* path, char* basepath) {
    int limit;
    
    limit = strlen (path) - 1;
    while (limit > 0 && path [limit - 1] != '/') {
        limit--;
    }
    
    strncpy (basepath, path, limit);
    basepath [limit] = '\0';
}

mesh_t* mesh_load_obj (const char* path) {
    FILE* file = fopen (path, "r");
    if (file == NULL) {
        fprintf (stderr, "mesh_load_obj error : Cannot open file %s.\n", path);
        return NULL;
    }

    size_t line = 1;
    size_t n_v, n_vn, n_vtc, n_f, n_fg;
    char buffer [512];
	unsigned int n;
    
    vertex_t            v;
    vertex_normal_t     vn;
    vertex_tex_coord_t  vtc;
    face_t              f;
    char                name [32];
    
    char basepath [32];
    __get_base_path (path, basepath);
    char fullpath [256];
    
    n_v = n_vn = n_vtc = n_f = n_fg = 0;
    obj_count_elements (file, &n_v, &n_vn, &n_vtc, &n_f, &n_fg);
    mesh_t* mesh = mesh_new (n_v, n_vn, n_vtc, n_f, n_fg);
    
    n_v = n_vn = n_vtc = n_f = n_fg = 0;
    
    fseek (file, SEEK_SET, 0);
    while (!feof (file)) {
        fgets (buffer, 511, file);
		n = strlen (buffer);
		while (buffer [n - 1] == '\r' || buffer [n - 1] == '\n') {
			buffer [n - 1] = '\0';
			n--;
		}
        // Parsing vertex.
        if (obj_line_is_vertex (buffer)) {
            if (obj_line_parse_vertex (buffer, &v)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            mesh_set_vertex (mesh, n_v++, &v);
        }
        // Parsing vertex normal.
        else if (obj_line_is_vertex_normal (buffer)) {
            if (obj_line_parse_vertex_normal (buffer, &vn)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            mesh_set_vertex_normal (mesh, n_vn++, &vn);
        }
        // Parsing vertex texture coord.
        else if (obj_line_is_vertex_tex_coord (buffer)) {
            if (obj_line_parse_vertex_tex_coord (buffer, &vtc)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            mesh_set_vertex_tex_coord (mesh, n_vtc++, &vtc);
        }
        // Parsing face.
        else if (obj_line_is_face (buffer)) {
            if (obj_line_parse_face (buffer, &f)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            mesh_add_face (mesh, n_f++, &f);
        }
        // Parsing group.
        else if (obj_line_is_group (buffer)) {
            if (obj_line_parse_group (buffer, name)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            mesh_begin_face_group (mesh, name);
        }
        // Parsing usemtl.
        else if (obj_line_is_usemtl (buffer)) {
            if (obj_line_parse_usemtl (buffer, name)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            mesh_set_current_material (mesh, name);
        }
        // Parsing matlib.
        else if (obj_line_is_matlib (buffer)) {
            if (obj_line_parse_matlib (buffer, name)) {
                fprintf (stderr, "mesh_load_obj error : Invalid line "SIZE_T_FORMAT".\n", line);
                free (mesh);
                fclose (file);
                return NULL;
            }
            
            sprintf (fullpath, "%s%s", basepath, name);
            
            mesh->matlib = material_lib_load (fullpath);
            if (mesh->matlib == NULL) {
                fprintf (stderr, "mesh_load_obj error : Cannot load material library %s : %s\n", name, 
                                 material_parse_error);
                return NULL;
            }
        }
        
        line++;
    }
    
    fclose (file);
    
    return mesh;
}


#ifdef TEST_MESH_IO_OBJ

int main (int argc, char** argv) {
    if (argc < 1) {
        printf ("usage : test_mesh_io_obj <obj file>\n");
    }
    
    mesh_t* mesh = mesh_load_obj (argv [1]);
    if (mesh) {
        printf ("Seems to be good !\n");
        mesh_delete (mesh);
    }
    
    return 0;
}

#endif



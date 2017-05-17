#include "3d/material.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>

char material_parse_error [512];

material_t default_material = {
    "default_material",
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    0.0f,
    0.0f,
    0
};

material_t* material_current_used = NULL;

material_t* material_new (const char* name, float ambient [4], float diffuse [4], float specular [4], float speccoef, 
						  float transluency, int illumination) {
	material_t* m = malloc (sizeof (material_t));
	
	strcpy (m->name, name);
	memcpy (m->ambient, ambient, 4 * sizeof (float));
	memcpy (m->diffuse, diffuse, 4 * sizeof (float));
	memcpy (m->specular, specular, 4 * sizeof (float));
	m->specular_coef = speccoef;
	m->transluency = transluency;
	m->illumination_model = illumination;
	
	return m;					
}

material_t* material_copy (material_t* src) {
    material_t* m = malloc (sizeof (material_t));
    memcpy (m, src, sizeof (material_t));
    return m;
}

void material_delete (material_t* m) {
	free (m);
}

void material_use (material_t* mat) {
	//...
	//float shininess [4] = {50.0, 50.0, 50.0, 50.0};
	
	if (mat != material_current_used) {
	    glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	    glColorMaterial (GL_FRONT, GL_SHININESS);
	    glMaterialfv (GL_FRONT, GL_SPECULAR, mat->specular);
        glMaterialfv (GL_FRONT, GL_AMBIENT, mat->ambient);
       	glMaterialfv (GL_FRONT, GL_DIFFUSE, mat->diffuse);
        glMaterialf (GL_FRONT, GL_SHININESS, mat->specular_coef);
	    //glColor3f (mat->diffuse [0], mat->diffuse [1], mat->diffuse [2]);
	
	    material_current_used = mat;
	}
}






material_lib_t* material_lib_new (int size) {
	material_lib_t* ml = malloc (sizeof (material_lib_t));
	
	ml->size = size;
	ml->materials = malloc (size * sizeof (material_t*));
	memset (ml->materials, 0, size * sizeof (material_t*));
	
	return ml;
}

material_lib_t* material_lib_copy (material_lib_t* src) {
    material_lib_t* copy = malloc (sizeof (material_lib_t));
    
    copy->size = src->size;
    copy->materials = malloc (copy->size * sizeof (material_t*));
    
    int i;
    for (i = 0; i < src->size; i++) {
        copy->materials [i] = material_copy (src->materials [i]);
    }
    
    return copy;
}

void material_lib_delete (material_lib_t* matlib) {
	int i;
	
	for (i = 0; i < matlib->size; i++) {
		if (matlib->materials [i] != NULL) {
			material_delete (matlib->materials [i]);
		}
	}
	free (matlib->materials);
	free (matlib);
}

material_lib_t* material_lib_load (const char* path) {
	char buffer [512];
	char name [256];
	float ambient [4];
	float diffuse [4];
	float specular [4];
	float speccoef;
	float transluency;
	int illumination;
	
	int current_mat;
	int mat_count;
	int line;

	FILE* f;
	material_lib_t* matlib;
	
	f = fopen (path, "r");
	if (f == NULL) {
		sprintf (material_parse_error, "Cannot open file %s.", path);
		return NULL;
	}
	
	// Counting number of materials
	mat_count = 0;
	do {
		fgets (buffer, 511, f);
		if (!feof (f)) {
			if (!strncmp (buffer, "newmtl ", 7)) {
				mat_count++;
			}
		}
	} while (!feof (f));
	
	if (mat_count <= 0) {
		sprintf (material_parse_error, "No material in file %s.", path);
		return NULL;
	}
	
	matlib = material_lib_new (mat_count);
	current_mat = -1;
	fseek (f, 0, SEEK_SET);
	line = 1;
	do {
		fgets (buffer, 511, f);
		if (!feof (f)) {
			// Getting on a new material definition
			if (!strncmp (buffer, "newmtl ", 7)) {
				// If there is already a parsed material...
				if (current_mat >= 0) {
					matlib->materials [current_mat] = material_new (name, ambient, diffuse, specular, speccoef,
																	transluency, illumination);
				}
			
				current_mat++;
				if (sscanf (buffer, "newmtl %255[^\r\n]", name) < 1) {
					sprintf (material_parse_error, "Line %d : A material must have a name %s.", line, name);
					material_lib_delete (matlib);
					return NULL;
				}
				
				memset (ambient, 0, 4 * sizeof (float));
				memset (diffuse, 0, 4 * sizeof (float));
				memset (specular, 0, 4 * sizeof (float));
				speccoef = 0.0f;
				transluency = 0.0f;
				illumination = 0;
			}
			// Ambient
			else if (!strncmp (buffer, "Ka ", 3)) {
				if (sscanf (buffer, "Ka %f %f %f", &ambient [0], &ambient [1], &ambient [2]) < 3) {
					sprintf (material_parse_error, "Line %d : A material ambient color must have 3 components.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			// Diffuse
			else if (!strncmp (buffer, "Kd ", 3)) {
				if (sscanf (buffer, "Kd %f %f %f", &diffuse [0], &diffuse [1], &diffuse [2]) < 3) {
					sprintf (material_parse_error, "Line %d : A material diffuse color must have 3 components.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			// Specular
			else if (!strncmp (buffer, "Ks ", 3)) {
				if (sscanf (buffer, "Ks %f %f %f", &specular [0], &specular [1], &specular [2]) < 3) {
					sprintf (material_parse_error, "Line %d : A material specular color must have 3 components.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			// Specular coeficient
			else if (!strncmp (buffer, "Ns ", 3)) {
				if (sscanf (buffer, "Ns %f", &speccoef) < 1) {
					sprintf (material_parse_error, "Line %d : A material specular coefficient must be a real.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			// Transluency
			else if (!strncmp (buffer, "d ", 2)) {
				if (sscanf (buffer, "d %f", &transluency) < 1) {
					sprintf (material_parse_error, "Line %d : Transluency error.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			else if (!strncmp (buffer, "Tr ", 3)) {
				if (sscanf (buffer, "Tr %f", &transluency) < 1) {
					sprintf (material_parse_error, "Line %d : Transluency error.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			// Illumination
			else if (!strncmp (buffer, "illum ", 6)) {
				if (sscanf (buffer, "illum %d", &illumination) < 1) {
					sprintf (material_parse_error, "Line %d : Illumination error.", line);
					material_lib_delete (matlib);
					return NULL;
				}
			}
			
			line++;
		}
		else {
			matlib->materials [current_mat] = material_new (name, ambient, diffuse, specular, speccoef,
															transluency, illumination);
		}
	} while (!feof (f));
	
	fclose (f);
	
	return matlib;
}

material_t* material_lib_get (material_lib_t* matlib, const char* mat_name) {
	int i;
	
	for (i = 0; i < matlib->size; i++) {
		if (matlib->materials [i] != NULL && !strcmp (matlib->materials [i]->name, mat_name)) {
			return matlib->materials [i];
		}
	}
	
	return NULL;
}


/* material.h
 *  Gestion du matériel openGL et des bibliothèques de matériels.
 */

#ifndef _material_h_
#define _material_h_

// Check this if an error occured using functions below to know what happened.
extern char material_parse_error [512];

typedef struct material {
	char name [256];
	
	float ambient [4];
	float diffuse [4];
	float specular [4];
	float specular_coef;
	
	float transluency;
	
	int illumination_model;
} material_t;

// Default material.
extern material_t default_material;

// Allocate a new material.
material_t* material_new (const char* name, float ambient [4], float diffuse [4], float specular [4], float speccoef, 
						  float transluency, int illumination);

// Create a copy of another material.					
material_t* material_copy (material_t* src);

// Delete a material.
void material_delete (material_t* mat);

// Set this material to current openGL material.
void material_use (material_t* mat);


typedef struct material_lib {
	int size;
	material_t** materials;
} material_lib_t;

// Create a new material library with 'size' materials in.
material_lib_t* material_lib_new (int size);

// Create a new copy of a material library.
material_lib_t* material_lib_copy (material_lib_t* src);

// Delete a material library and all materials it contains.
void material_lib_delete (material_lib_t* matlib);

// Load a material library from a file. Return NULL on error.
material_lib_t* material_lib_load (const char* path);

// Return a material by its name from a material library.
material_t* material_lib_get (material_lib_t* matlib, const char* mat_name);

#endif


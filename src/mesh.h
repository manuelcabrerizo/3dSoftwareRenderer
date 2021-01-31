#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "upng.h"

//Define a struct for dynamic size meshes
typedef struct
{
    vec3_t* vertices; // dynamic array of vertices
    face_t* faces;    // dynamic array of faces
    upng_t* texture;  // mesh png texture pointer
    vec3_t rotation;  
    vec3_t scale;
    vec3_t translation;
} mesh_t;


void LoadMesh(const char* obj_filename, const char* png_filename,
              vec3_t scale, vec3_t traslation, vec3_t rotation);
void LoadObjFile(mesh_t* mesh, const char* filename);
void LoadPngFile(mesh_t* mesh, const char* filePhat);

int get_num_meshes(void);
mesh_t* GetMeshByIndex(int mesh_index);
void FreeMeshes(void);


#endif

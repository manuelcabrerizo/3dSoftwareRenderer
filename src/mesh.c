#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mesh.h"
#include "array.h"
#include "texture.h"

#define MAX_NUM_MESHES 10
static mesh_t meshes[MAX_NUM_MESHES];
static int mesh_count = 0;


void LoadMesh(const char* obj_filename, const char* png_filename,
              vec3_t scale, vec3_t traslation, vec3_t rotation)
{
    LoadObjFile(&meshes[mesh_count], obj_filename);
    LoadPngFile(&meshes[mesh_count], png_filename);

    meshes[mesh_count].scale = scale; 
    meshes[mesh_count].translation = traslation;
    meshes[mesh_count].rotation = rotation; 

    mesh_count++;
 
}

void LoadObjFile(mesh_t* mesh, const char* filename)
{
    FILE* file;
    file = fopen(filename, "r");

    if(file == NULL)
    {
        printf("error loading file\n");
    }
    char line[1024];

    tex2_t* texCoords = NULL;

    while(fgets(line, 1024, file) != NULL)
    {
         if(strncmp(line, "v ", 2) == 0)
         {
            vec3_t vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            array_push(mesh->vertices, vertex);
         }

         if(strncmp(line, "vt ", 3) == 0)
         {
            tex2_t texCoord;
            sscanf(line, "vt %f %f", &texCoord.u, &texCoord.v);
            array_push(texCoords, texCoord);
         }

         if(strncmp(line, "f ", 2) == 0)
         {
            //indices, texture, normals
            int indices[3];
            int textures[3];
            int normals[3];

            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                &indices[0], &textures[0], &normals[0], 
                &indices[1], &textures[1], &normals[1],
                &indices[2], &textures[2], &normals[2]
            );

            face_t face;
            face.a = indices[0] - 1;
            face.b = indices[1] - 1;
            face.c = indices[2] - 1;
            face.a_uv = texCoords[textures[0] - 1];
            face.b_uv = texCoords[textures[1] - 1];
            face.c_uv = texCoords[textures[2] - 1];
            face.color = 0xFFFFFFFF;

            array_push(mesh->faces, face);
         }
    } 
    fclose(file);
    free(file);
    array_free(texCoords);
}

void LoadPngFile(mesh_t* mesh, const char* filePhat)
{
    upng_t* png_image = upng_new_from_file(filePhat);
    if(png_image != NULL)
    {
        upng_decode(png_image);
        if(upng_get_error(png_image) == UPNG_EOK)
        {
            mesh->texture = png_image;
        }
        
    }
}

int get_num_meshes(void)
{
    return mesh_count;
}

mesh_t* GetMeshByIndex(int mesh_index)
{
    return &meshes[mesh_index];
}

void FreeMeshes(void)
{
    for(int i = 0; i < mesh_count; i++)
    {
        upng_free(meshes[i].texture);
        array_free(meshes[i].faces);
        array_free(meshes[i].vertices);
    }
}
























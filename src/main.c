#include <stdlib.h>
#include <stdio.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "upng.h" 
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "camera.h"
#include "clipping.h"
#include "math.h"

enum RenderMode 
{
    wireFrameVertex = 0,
    wireFrame = 1,
    filled = 2,
    filledWireFrame = 3,
    renderTexture = 4,
    renderTextureWire = 5
} renderMode;

bool backFaceCulling = true;
#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangelsToRender[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

mat4_t projectionMatrix;
mat4_t viewMatrix;
mat4_t worldMatrix;

light_t globalLight;

bool isRunning = false;
int previousFrameTime = 0;
float deltaTime = 0;

mesh_t* player = NULL;
vec3_t player_direction = {0, 0, 5};

void SetUp(void)
{
    globalLight.direction.x = 1.0;
    globalLight.direction.y = -1.0;
    globalLight.direction.z = 1.0;
    globalLight.direction = NormalizeVec3(globalLight.direction);

    //Initialize prespective projection Matrix
    float aspect_x = (float)GetWindowWidth() / (float)GetWindowHeight();
    float aspect_y = (float)GetWindowHeight() / (float)GetWindowWidth();
    float fov_y = 3.141592 / 3.0;
    float fov_x = atan(tan(fov_y / 2) * aspect_x) * 2;
    float znear = 1.0;
    float zfar = 20.0;
    projectionMatrix = Mat4MakePerspective(fov_y, aspect_y, znear, zfar);
    //Initialize Frustum Planes
    InitializeFrustumPlanes(fov_x, fov_y, znear, zfar);

    //TODO: Load multiple Meshes
    LoadMesh("./assets/runway.obj", "./assets/runway.png", Vec3New(1, 1, 1), Vec3New(0, -1.5, +23), Vec3New(0, 0, 0));
    LoadMesh("./assets/f22.obj", "./assets/f22.png", Vec3New(1, 1, 1), Vec3New(0, -1.3, +5), Vec3New(0, -M_PI/2, 0));
    LoadMesh("./assets/efa.obj", "./assets/efa.png", Vec3New(1, 1, 1), Vec3New(-2, -1.3, +9), Vec3New(0, -M_PI/2, 0));
    LoadMesh("./assets/f117.obj", "./assets/f117.png", Vec3New(1, 1, 1), Vec3New(+2, -1.3, +9), Vec3New(0, -M_PI/2, 0));
    
    renderMode = wireFrame;
    player = GetMeshByIndex(2);
}

void ProcessInput(void)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
    switch(event.type)
    {
        case SDL_QUIT:
        {
            isRunning = false;
        }break;

        case SDL_KEYDOWN:
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
                break;
            }
            if(event.key.keysym.sym == SDLK_1)
            {
                 renderMode = wireFrameVertex;
                 break;
            }
            if(event.key.keysym.sym == SDLK_2)
            {
                renderMode = wireFrame;
                break;
            }
            if(event.key.keysym.sym == SDLK_3)
            {
                renderMode = filled;
                break;
            }
            if(event.key.keysym.sym == SDLK_4)
            {
                renderMode = filledWireFrame;
                break;
            }
            if(event.key.keysym.sym == SDLK_5)
            {
                renderMode = renderTexture;
                break;
            }
            if(event.key.keysym.sym == SDLK_6)
            {
                renderMode = renderTextureWire;
                break;
            }
            if(event.key.keysym.sym == SDLK_c)
            {
                backFaceCulling = true;
                break; 
            }
            if(event.key.keysym.sym == SDLK_f)
            {
                backFaceCulling = false; 
                break;
            }
            if(event.key.keysym.sym == SDLK_w)
            {
                //camera.forwardVelocity = Vec3Mul(camera.direction, 5.0 * deltaTime);
                //camera.position =  Vector3Add(camera.position, camera.forwardVelocity);
                
                player->translation = Vector3Add(player->translation, Vec3Mul(player_direction, deltaTime));
                break;
            }
            if(event.key.keysym.sym == SDLK_s)
            {
                //camera.forwardVelocity = Vec3Mul(camera.direction, 5.0 * deltaTime);
                //camera.position =  Vector3Sub(camera.position, camera.forwardVelocity);
                //player->translation.z -= 2.0 * deltaTime;
                player->translation = Vector3Sub(player->translation, Vec3Mul(player_direction, deltaTime));
                break;
            }
            if(event.key.keysym.sym == SDLK_a)
            {
                //camera.yaw -= 1.0 * deltaTime;
                player_direction = vec3RotateY(player_direction, -2.0 * deltaTime);
                player->rotation.y -= 2.0 * deltaTime;
                break; 
            }
            if(event.key.keysym.sym == SDLK_d)
            {
                //camera.yaw += 1.0 * deltaTime;
                player_direction = vec3RotateY(player_direction, 2.0 * deltaTime);
                player->rotation.y += 2.0 * deltaTime;
                break; 
            }
            if(event.key.keysym.sym == SDLK_PAGEDOWN)
            {
                player->rotation.x -= 2.0 * deltaTime; 
                break;
            }
            if(event.key.keysym.sym == SDLK_PAGEUP)
            {
                player->rotation.x += 2.0 * deltaTime;
                break;
            }
            if(event.key.keysym.sym == SDLK_UP)
            {
                //camera.position.y += 3.0 * deltaTime;
                player->translation.y += 2.0 * deltaTime;
                break;
            }
            if(event.key.keysym.sym == SDLK_DOWN)
            {
                //camera.position.y -= 3.0 * deltaTime;
                player->translation.y -= 2.0 * deltaTime;
                break;
            }

        }break;
    }
    }
}




void Update(void)
{
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);

    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(timeToWait);
    }
    
    //Get a delta time factor converted to seconds to be use to update our game objects
    deltaTime = (SDL_GetTicks() - previousFrameTime) / 1000.0;
    printf("dt %f\n", deltaTime);
    previousFrameTime = SDL_GetTicks();
    
    num_triangles_to_render = 0;


    for(int mesh_index = 0; mesh_index < get_num_meshes(); mesh_index++)
    {
        mesh_t* mesh = GetMeshByIndex(mesh_index);

        //mesh->scale.x = 1.0;
        //mesh->scale.y = 1.0;
        //mesh->scale.z = 1.0;
        //mesh->rotation.x = 90 * (M_PI / 180);
        //mesh->rotation.x += 1.0 * deltaTime;
        //mesh->rotation.y += 1.0 * deltaTime;
        //mesh->rotation.z += 0.00 * deltaTime;
        //mesh->translation.x += 0.01 * deltaTime;
        //mesh->translation.z += 0.01 * deltaTime; 
        //mesh->translation.y = 0.0;
        //mesh->translation.z = 5.0;
        //mesh->translation.x = 0.0;
        //mesh->translation.y = 0.0;


        //create scale matrix
        mat4_t scaleMatrix = Mat4MakeScale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
        mat4_t translationMatrix = Mat4MakeTranslation(mesh->translation.x, mesh->translation.y, mesh->translation.z);
        mat4_t rotationMatrix_X = Mat4MakeRotationX(mesh->rotation.x);
        mat4_t rotationMatrix_Y = Mat4MakeRotationY(mesh->rotation.y);
        mat4_t rotationMatrix_Z = Mat4MakeRotationZ(mesh->rotation.z);

        //TODO: compute the new camera rotation and translation for FPS camera movement
        vec3_t target = get_camera_target();
        vec3_t up_direction = { 0, 1, 0 };
        viewMatrix = Mat4LookAt(camera.position, target, up_direction);

        int num_faces = array_length(mesh->faces);
        for(int i = 0; i < num_faces; i++)
        {
            face_t meshFace = mesh->faces[i];
        
            vec3_t faceVertices[3];
            faceVertices[0] = mesh->vertices[meshFace.a];
            faceVertices[1] = mesh->vertices[meshFace.b];
            faceVertices[2] = mesh->vertices[meshFace.c];    

            vec4_t transformedVertices[3];
            // tarnform evrery Vertex
            for(int j = 0; j < 3; j++)
            {
                vec4_t transformedVertex = Vec4FromVec3(faceVertices[j]);

                //TODO: use Matrix to transform 
                //Mutiply scale Matrix by vertex
           
                worldMatrix = Mat4Identity(); //Creo el world Matrix
                // Order Matters: frist scale, then rotate, then translate.
                worldMatrix = Mat4MultMat4(scaleMatrix, worldMatrix);   //Lo mutiplico por todas
                worldMatrix = Mat4MultMat4(rotationMatrix_Z, worldMatrix);
                worldMatrix = Mat4MultMat4(rotationMatrix_Y, worldMatrix);
                worldMatrix = Mat4MultMat4(rotationMatrix_X, worldMatrix);  //las Matrices de tranformaciones lineales 
                worldMatrix = Mat4MultMat4(translationMatrix, worldMatrix);

                transformedVertex = Mat4MultVect4(worldMatrix, transformedVertex);
                //Multiplay the viewMatrix by the original vector
                transformedVertex = Mat4MultVect4(viewMatrix, transformedVertex);

                transformedVertices[j] = transformedVertex;
            }
            //BackFace Culling
            //TODO: backFace Culling Algoritmo
            vec3_t vecA = Vec3FromVec4(transformedVertices[0]);
            vec3_t vecB = Vec3FromVec4(transformedVertices[1]);
            vec3_t vecC = Vec3FromVec4(transformedVertices[2]);

            vec3_t vecAB = NormalizeVec3(Vector3Sub(vecB, vecA));
            vec3_t vecAC = NormalizeVec3(Vector3Sub(vecC, vecA));
            vec3_t normal = NormalizeVec3(CrossProduct(vecAB, vecAC));

            vec3_t origin = { 0, 0, 0 };
            vec3_t cameraRay = Vector3Sub(origin, vecA);
            float normalDirection = Vec3DotProduct(normal, cameraRay);
  
            if(backFaceCulling == true)
            { 
                if(normalDirection < 0)
                {
                    continue;
                }
            }

            // TODO: Clipping!!!
            polygon_t polygon = CreatePolygonFromTriangle(
                    vecA,
                    vecB,
                    vecC,
                    meshFace.a_uv,
                    meshFace.b_uv,
                    meshFace.c_uv
            );

            ClipPolygon(&polygon);

            // TODO: Break the polygon Back in to triangles
            triangle_t triangles_after_clipping[MAX_NUM_POLY_TRIANGLE];
            int num_triangles_after_clipping = 0;

            TrianglesFromPolygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);

            // Loop all the assembled triangles after clipping
            for(int t = 0; t < num_triangles_after_clipping; t++)
            {
                triangle_t triangle_afterClipping = triangles_after_clipping[t];
                vec4_t projectedPoints[3];

                //Project ervery Vertex
                for(int j = 0; j < 3; j++)
                {
                    projectedPoints[j] = Mat4MultVec4Project(projectionMatrix, triangle_afterClipping.points[j]);

                    projectedPoints[j].y *= -1;

                    projectedPoints[j].x *= (GetWindowWidth() / 2.0);
                    projectedPoints[j].y *= (GetWindowHeight() / 2.0);

                    projectedPoints[j].x += (GetWindowWidth() / 2.0);
                    projectedPoints[j].y += (GetWindowHeight() / 2.0);

                } 

                float lightBounce = -Vec3DotProduct(normal, globalLight.direction);
                meshFace.color = LightApplyIntesity(meshFace.color, lightBounce);

                triangle_t projectedTriangle = {
                    .points = {
                        {projectedPoints[0].x, projectedPoints[0].y, projectedPoints[0].z, projectedPoints[0].w},
                        {projectedPoints[1].x, projectedPoints[1].y, projectedPoints[1].z, projectedPoints[1].w},
                        {projectedPoints[2].x, projectedPoints[2].y, projectedPoints[2].z, projectedPoints[2].w}
                    },
                    .texCoords = {
                        {triangle_afterClipping.texCoords[0].u, triangle_afterClipping.texCoords[0].v},
                        {triangle_afterClipping.texCoords[1].u, triangle_afterClipping.texCoords[1].v},
                        {triangle_afterClipping.texCoords[2].u, triangle_afterClipping.texCoords[2].v}
                    },
                    .color = meshFace.color,
                    .texture = mesh->texture
                };

                //triangelsToRender[i] = projectedTriangle;
                if(num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
                {
                    triangelsToRender[num_triangles_to_render] = projectedTriangle;
                    num_triangles_to_render++;
                }
        
            }      
        }
    }
}


void Render(void)
{

    ClearColorBuffer(0xFF000000);
    ClearZBuffer();

    DrawColorGrid();
 
    for(int i = 0; i < num_triangles_to_render; i++)
    {
        triangle_t triangle = triangelsToRender[i];
        
        if(renderMode == wireFrameVertex)
        {
            DrawRect(triangle.points[0].x - 5, triangle.points[0].y - 5, 10, 10, 0xFF0000FF);
            DrawRect(triangle.points[1].x - 5, triangle.points[1].y - 5, 10, 10, 0xFF0000FF);
            DrawRect(triangle.points[2].x - 5, triangle.points[2].y - 5, 10, 10, 0xFF0000FF);
        }            

        if(renderMode == filled ||renderMode == filledWireFrame)
        {
            DrawFilledTriangle
            (
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
                triangle.color
            );
        }

        if(renderMode == renderTexture || renderMode == renderTextureWire)
        {
            
            DrawTextureTriangle(
                triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texCoords[0].u, triangle.texCoords[0].v,
                triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texCoords[1].u, triangle.texCoords[1].v,
                triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texCoords[2].u, triangle.texCoords[2].v,
                triangle.texture 
            );
            
        }

        if(renderMode == filledWireFrame || renderMode == wireFrameVertex || renderMode == wireFrame|| renderMode == renderTextureWire)
        {
            DrawTriangle
            (
                triangle.points[0].x, triangle.points[0].y,
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[2].x, triangle.points[2].y,
                0xFFF00FFF
            );
        }
    } 
    
    RenderColorBuffer();

    
}


void FreeResources(void)
{ 
    FreeMeshes();
    DestroyWindow();
}

int main(int argc, char* argv[])
{
    isRunning = InitializeWindow();
    
    SetUp();
     
    while(isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }


    FreeResources();
    return 0;
}





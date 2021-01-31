#include "clipping.h"
#include "math.h"
#include "texture.h"


#define NUM_PLANES 6
plane_t frustumPlanes[NUM_PLANES];

void InitializeFrustumPlanes(float fov_x, float fov_y, float zNear, float zFar)
{
    float cos_half_fov_x = cos(fov_x / 2);
    float sin_half_fov_x = sin(fov_x / 2);
    float cos_half_fov_y = cos(fov_y / 2);
    float sin_half_fov_y = sin(fov_y / 2);


    frustumPlanes[LEFT_FRUSTUM_PLANE].point = Vec3New(0, 0, 0);
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov_x;
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov_x; 

    frustumPlanes[RIGHT_FRUSTUM_PLANE].point = Vec3New(0, 0, 0);
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov_x;
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0; 
    frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov_x;

    frustumPlanes[TOP_FRUSTUM_PLANE].point = Vec3New(0, 0, 0);
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov_y;
    frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

    frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = Vec3New(0, 0, 0);
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov_y;
    frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov_y;

    frustumPlanes[NEAR_FRUSTUM_PLANE].point = Vec3New(0, 0, zNear);
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[NEAR_FRUSTUM_PLANE].normal.z = 1;

    frustumPlanes[FAR_FRUSTUM_PLANE].point = Vec3New(0, 0, zFar);
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.x = 0;
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.y = 0;
    frustumPlanes[FAR_FRUSTUM_PLANE].normal.z = -1;

}

polygon_t CreatePolygonFromTriangle(vec3_t v0, vec3_t v1, vec3_t v2,
                                    tex2_t t0, tex2_t t1, tex2_t t2)
{
    polygon_t result;
    result.vertices[0] = v0;
    result.vertices[1] = v1;
    result.vertices[2] = v2;
    result.texcoords[0] = t0;
    result.texcoords[1] = t1;
    result.texcoords[2] = t2;
    result.numVertices = 3;
    return result;
}

void TrianglesFromPolygon(polygon_t* polygon, triangle_t triangles[], int* num_triangles)
{
    for(int i = 0; i < polygon->numVertices - 2; i++)
    {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;

        //TODO: create triangle
        triangles[i].points[0] = Vec4FromVec3(polygon->vertices[index0]); 
        triangles[i].points[1] = Vec4FromVec3(polygon->vertices[index1]);
        triangles[i].points[2] = Vec4FromVec3(polygon->vertices[index2]);
        triangles[i].texCoords[0] = polygon->texcoords[index0];
        triangles[i].texCoords[1] = polygon->texcoords[index1];
        triangles[i].texCoords[2] = polygon->texcoords[index2];
    }
    *num_triangles = polygon->numVertices - 2;
}

float floatLerp(float a, float b, float t)
{
    return a + t * (b - a);
}

void ClipPolygonAgainstPlane(polygon_t* polygon, int plane)
{
    vec3_t plane_point = frustumPlanes[plane].point;
    vec3_t plane_normal = frustumPlanes[plane].normal; 

    // the array of inside vertices that will be part of the final polygon returned via parameter
    vec3_t inside_vectices[MAX_NUM_POLY_VERTICES];
    int num_inside_vertices = 0;
    tex2_t inside_texcoords[MAX_NUM_POLY_VERTICES];
    //int num_inside_texcoords = 0;

    // Start current whit the first vertex and pervious whit the last vertex 
    vec3_t* current_vertex = &polygon->vertices[0];
    tex2_t* current_texcoord = &polygon->texcoords[0];

    vec3_t* previous_vertex = &polygon->vertices[polygon->numVertices - 1];
    tex2_t* previous_texcoord = &polygon->texcoords[polygon->numVertices - 1];



    // Start the current and previous dot Product
    float current_dot = 0; 
    float previous_dot = Vec3DotProduct(Vector3Sub(*previous_vertex, plane_point), plane_normal);

    while(current_vertex != &polygon->vertices[polygon->numVertices])
    {
        current_dot = Vec3DotProduct(Vector3Sub(*current_vertex, plane_point), plane_normal); 
        // if we change from inside to outside or otherwise 
        if(current_dot * previous_dot < 0)
        {
            // TODO: calculate the interpolation factor t
            float t = previous_dot / ( previous_dot - current_dot);

            // TODO: calculate the intersection point I = Q1 + t(Q2- Q1) LERP (linear interpolation)
            vec3_t intersection_point = {
                .x = floatLerp(previous_vertex->x, current_vertex->x, t),
                .y = floatLerp(previous_vertex->y, current_vertex->y, t),
                .z = floatLerp(previous_vertex->z, current_vertex->z, t)
            }; 

            // Use the lerp (linear interpolation) to get interpolated U and V texCoods
            tex2_t intersection_texcoord = {
                .u = floatLerp(previous_texcoord->u, current_texcoord->u, t),
                .v = floatLerp(previous_texcoord->v, current_texcoord->v, t)
            };

            // TODO: Insert the new intersection in inside List
            inside_texcoords[num_inside_vertices] = Tex2Clone(&intersection_texcoord);
            inside_vectices[num_inside_vertices] = Vec3Clone(&intersection_point);
            num_inside_vertices++;
        }

        // If current point is inside the plane
        if(current_dot > 0)
        {
            inside_vectices[num_inside_vertices] = Vec3Clone(current_vertex);
            inside_texcoords[num_inside_vertices] = Tex2Clone(current_texcoord);
            num_inside_vertices++;
        }
        // Move to the next vertex
        previous_dot = current_dot;
        previous_texcoord = current_texcoord;
        previous_vertex = current_vertex;
        current_texcoord++;
        current_vertex++;
    }

    // TODO: Set all the vertices of the inside vertices Array to the polygon
    for(int i = 0; i < num_inside_vertices; i++)
    {
        polygon->vertices[i] = Vec3Clone(&inside_vectices[i]);
        polygon->texcoords[i] = Tex2Clone(&inside_texcoords[i]);
    }
    polygon->numVertices = num_inside_vertices;

}

void ClipPolygon(polygon_t* polygon)
{
    ClipPolygonAgainstPlane(polygon, LEFT_FRUSTUM_PLANE);
    ClipPolygonAgainstPlane(polygon, RIGHT_FRUSTUM_PLANE);
    ClipPolygonAgainstPlane(polygon, TOP_FRUSTUM_PLANE);
    ClipPolygonAgainstPlane(polygon, BOTTOM_FRUSTUM_PLANE);
    ClipPolygonAgainstPlane(polygon, NEAR_FRUSTUM_PLANE);
    ClipPolygonAgainstPlane(polygon, FAR_FRUSTUM_PLANE);
}

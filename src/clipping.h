#ifndef CLIPPING_H
#define CLIPPING_H

#include "vector.h"
#include "triangle.h"

#define MAX_NUM_POLY_VERTICES 10
#define MAX_NUM_POLY_TRIANGLE 10

enum 
{
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE
};

typedef struct 
{
    vec3_t point;
    vec3_t normal;
} plane_t;

typedef struct
{
    vec3_t vertices[MAX_NUM_POLY_VERTICES];
    tex2_t texcoords[MAX_NUM_POLY_VERTICES];
    int numVertices;
} polygon_t;

void InitializeFrustumPlanes(float fov_x, float fov_y, float zNear, float zFar);
polygon_t CreatePolygonFromTriangle(vec3_t v0, vec3_t v1, vec3_t v2, tex2_t t0, tex2_t t1, tex2_t t2);
void ClipPolygonAgainstPlane(polygon_t* polygon, int plane);
void ClipPolygon(polygon_t* polygon);
void TrianglesFromPolygon(polygon_t* polygon, triangle_t triangles[], int* num_triangles);

#endif

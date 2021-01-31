#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"
typedef struct
{
    float m[4][4];
} mat4_t;

mat4_t Mat4Identity(void);
mat4_t Mat4MakeScale(float sx, float sy, float sz);
mat4_t Mat4MakeTranslation(float tx, float ty, float tz);
mat4_t Mat4MakeRotationX(float angle);
mat4_t Mat4MakeRotationY(float angle);
mat4_t Mat4MakeRotationZ(float angle);
mat4_t Mat4MakePerspective(float fov, float aspect, float znear, float zfar);
vec4_t Mat4MultVect4(mat4_t m, vec4_t v);
mat4_t Mat4MultMat4(mat4_t a, mat4_t b);
vec4_t Mat4MultVec4Project(mat4_t matProj, vec4_t v);
mat4_t Mat4LookAt(vec3_t eye, vec3_t target, vec3_t up);

#endif

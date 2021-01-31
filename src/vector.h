#ifndef VECTOR_H
#define VECTOR_H

typedef struct
{
    float x;
    float y;
} vec2_t;

typedef struct
{
    float x; 
    float y;
    float z;
} vec3_t;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
}vec4_t;




vec3_t vec3RotateX(vec3_t v, float angel);
vec3_t vec3RotateY(vec3_t v, float angel);
vec3_t vec3RotateZ(vec3_t v, float angel);

vec3_t Vec3New(float x, float y, float z);
float Vector3Lenght(vec3_t vec3);
vec3_t NormalizeVec3(vec3_t v);
vec3_t Vector3Add(vec3_t v0, vec3_t v1);
vec3_t Vector3Sub(vec3_t v0, vec3_t v1);
vec3_t Vec3Mul(vec3_t v, float scalar);
vec3_t Vec3Div(vec3_t v, float scalar);

vec2_t Vec2New(float x, float y);
float Vector2Lenght(vec2_t vec2);
vec2_t NormalizeVec2(vec2_t v);
vec2_t Vector2Add(vec2_t v0, vec2_t v1);
vec2_t Vector2Sub(vec2_t v0, vec2_t v1);
vec2_t Vec2Mul(vec2_t v, float scalar);
vec2_t Vec2Div(vec2_t v, float scalar);

vec3_t CrossProduct(vec3_t a, vec3_t b);
float Vec2DotProduct(vec2_t a, vec2_t b);
float Vec3DotProduct(vec3_t a, vec3_t b);

//vec4 Functions
vec4_t Vec4FromVec3(vec3_t v);
vec3_t Vec3FromVec4(vec4_t v);
vec2_t vec2FromVec4(vec4_t v);

vec3_t Vec3Clone(vec3_t* v);



#endif

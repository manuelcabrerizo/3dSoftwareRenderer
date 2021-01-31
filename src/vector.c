#include <math.h>
#include "vector.h"

vec3_t vec3RotateX(vec3_t v, float angel)
{
    vec3_t rotatedVector = {
        .x = v.x,
        .y = v.y * cos(angel) - v.z * sin(angel),
        .z = v.y * sin(angel) + v.z * cos(angel)
    };
    return rotatedVector;
}

vec3_t vec3RotateY(vec3_t v, float angel)
{
    vec3_t rotatedVector = {
        .x = v.x * cos(angel) + v.z * sin(angel),
        .y = v.y,
        .z = -v.x * sin(angel) + v.z * cos(angel)
    };
    return rotatedVector;

}

vec3_t vec3RotateZ(vec3_t v, float angel)
{
    vec3_t rotatedVector = {
        .x = v.x * cos(angel) - v.y * sin(angel),
        .y = v.x * sin(angel) + v.y * cos(angel),    
        .z = v.z
    };
    return rotatedVector;

}

float Vector2Lenght(vec2_t vec2)
{
    return sqrt(pow(vec2.x, 2) + pow(vec2.y, 2));
}

float Vector3Lenght(vec3_t vec3)
{
    return sqrt(pow(vec3.x, 2) + pow(vec3.y, 2) + pow(vec3.z, 2));
}


vec3_t NormalizeVec3(vec3_t v)
{
    vec3_t normalizedVec;
    float vLenght = Vector3Lenght(v); 
    normalizedVec.x = v.x / vLenght;
    normalizedVec.y = v.y / vLenght;
    normalizedVec.z = v.z / vLenght;
    return normalizedVec;
}


vec2_t NormalizeVec2(vec2_t v)
{
    vec2_t normalizedVec;
    float vLenght = Vector2Lenght(v);
    normalizedVec.x = v.x / vLenght;
    normalizedVec.y = v.y / vLenght;
    return normalizedVec;
}


vec2_t Vector2Add(vec2_t v0, vec2_t v1)
{
    vec2_t result;
    result.x = v0.x + v1.x;
    result.y = v0.y + v1.y;
    return result;
}

vec2_t Vector2Sub(vec2_t v0, vec2_t v1)
{
    vec2_t result;
    result.x = v0.x - v1.x;
    result.y = v0.y - v1.y;
    return result;
}


vec3_t Vector3Add(vec3_t v0, vec3_t v1)
{
    vec3_t result;
    result.x = v0.x + v1.x;
    result.y = v0.y + v1.y;
    result.z = v0.z + v1.z;
    return result;
}

vec3_t Vector3Sub(vec3_t v0, vec3_t v1)
{
    vec3_t result;
    result.x = v0.x - v1.x;
    result.y = v0.y - v1.y;
    result.z = v0.z - v1.z;
    return result;
}

vec2_t Vec2Mul(vec2_t v, float scalar)
{
    vec2_t result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

vec3_t Vec3Mul(vec3_t v, float scalar)
{
    vec3_t result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}

vec2_t Vec2Div(vec2_t v, float scalar)
{
    vec2_t result;
    result.x = v.x / scalar;
    result.y = v.y / scalar;
    return result;
}

vec3_t Vec3Div(vec3_t v, float scalar)
{
    vec3_t result;
    result.x = v.x / scalar;
    result.y = v.y / scalar;
    result.z = v.z / scalar;
    return result;
}

vec3_t CrossProduct(vec3_t a, vec3_t b)
{
    vec3_t result = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x
    };
    return result;

}


float Vec2DotProduct(vec2_t a, vec2_t b)
{
    return (a.x * b.x) + (a.y * b.y);
}

float Vec3DotProduct(vec3_t a, vec3_t b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vec4_t Vec4FromVec3(vec3_t v)
{
    vec4_t result = {v.x, v.y, v.z, 1.0};
    return result;
}

vec3_t Vec3FromVec4(vec4_t v)
{
    vec3_t result = {v.x, v.y, v.z};
    return result;
}

vec2_t vec2FromVec4(vec4_t v)
{
    vec2_t result = {v.x, v.y};
    return result;
}

vec2_t Vec2New(float x, float y)
{
    vec2_t result = {x, y};
    return result;
}

vec3_t Vec3New(float x, float y, float z)
{
    vec3_t result = {x, y, z};
    return result;
}

vec3_t Vec3Clone(vec3_t* v)
{
    vec3_t result = { v->x, v->y, v->z };
    return result;
}

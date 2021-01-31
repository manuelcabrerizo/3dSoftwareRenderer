#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct
{
    vec3_t position;
    vec3_t direction;
    vec3_t forwardVelocity;
    float yaw;
    float pitch;
} camera_t;

extern camera_t camera;
vec3_t get_camera_target(void);

#endif

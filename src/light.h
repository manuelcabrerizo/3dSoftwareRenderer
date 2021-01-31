#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include <stdint.h>

typedef struct
{
    vec3_t direction;
}light_t;

uint32_t LightApplyIntesity(uint32_t originalColor, float porcentage);


#endif

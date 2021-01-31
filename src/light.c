#include "light.h"

uint32_t LightApplyIntesity(uint32_t originalColor, float porcentage)
{

    if(porcentage < 0){ porcentage = 0; }
    if(porcentage > 1){ porcentage = 1; }

    uint32_t a = (originalColor & 0xFF000000);
    uint32_t r = (originalColor & 0x00FF0000) * porcentage;
    uint32_t g = (originalColor & 0x0000FF00) * porcentage;
    uint32_t b = (originalColor & 0x000000FF) * porcentage;

    uint32_t newColor = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    return newColor;
}


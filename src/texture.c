#include <stdio.h>
#include "texture.h"

tex2_t Tex2Clone(tex2_t* t)
{
    tex2_t result = { t->u, t->v };
    return result;
}





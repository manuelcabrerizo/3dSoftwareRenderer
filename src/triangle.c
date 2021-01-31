#include "triangle.h"
#include <stdlib.h>
#include "display.h"
//TODO: crate implementation of trangle.h


void IntSwap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void FloatSwap(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}


void FillFlatBottom(int x0, int y0, int x1, int y1, int mx, int my, uint32_t color)
{
    //TODO: make the function
    float invSlope1 = (float)(x1 - x0) / (y1 - y0);
    float invSlope2 = (float)(mx - x0) / (my - y0);

    float xStart = x0;
    float xEnd = x0;

    for(int y = y0; y <= my; y++)
    {
        DrawLine(xStart, y, xEnd, y, color);
        xStart += invSlope1;
        xEnd += invSlope2;
    }
}

void FillFlatTop(int x1, int y1, int mx, int my, int x2, int y2, uint32_t color)
{
    //TODO: Make the Function
    float invSlope1 = (float)(x2 - x1) / (y2 - y1);
    float invSlope2 = (float)(x2 - mx) / (y2 - my);

    float xStart = x2;
    float xEnd = x2;

    for(int y = y2; y >= my; y--)
    {
        DrawLine(xStart, y, xEnd, y, color);
        xStart -= invSlope1;
        xEnd -= invSlope2;
    }
}

void DrawTrianglePixel(
        int x, int y, uint32_t color,
        vec4_t point_a,
        vec4_t point_b,
        vec4_t point_c)
{
    vec2_t p = {x, y};
    vec2_t a = vec2FromVec4(point_a);
    vec2_t b = vec2FromVec4(point_b);
    vec2_t c = vec2FromVec4(point_c);

    vec3_t weights = BarycentricWeights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    const float EPSILON = 0.0000001;
    if(alpha < -EPSILON || beta < -EPSILON || gamma < -EPSILON)
    {
        return;
    }

    float interpolated_rw;

    interpolated_rw = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    // adjust 1/w so the pixel that are closer to the camear have smaller values
    interpolated_rw = 1.0 - interpolated_rw;

    if(interpolated_rw < get_zbuffer_at(x, y))
    {
        DrawPixel(x, y, color);
        //Update the z_buffer value whit the 1/w of this current pixel
        update_zbuffer_at(x, y, interpolated_rw);
    }
}

void DrawFilledTriangle(
        int x0, int y0, float z0, float w0,
        int x1, int y1, float z1, float w1,
        int x2, int y2, float z2, float w2,
        uint32_t color)
{
    if(y0 > y1)
    {
        IntSwap(&y0, &y1);
        IntSwap(&x0, &x1);
        FloatSwap(&z0, &z1);
        FloatSwap(&w0, &w1);
    }

    if(y1 > y2)
    {
        IntSwap(&y1, &y2);
        IntSwap(&x1, &x2);
        FloatSwap(&z1, &z2);
        FloatSwap(&w1, &w2);
    }

    if(y0 > y1)
    {
        IntSwap(&y0, &y1);
        IntSwap(&x0, &x1);
        FloatSwap(&z0, &z1);
        FloatSwap(&w0, &w1);
    }
    //Create Vector Points
    vec4_t point_a = {x0, y0, z0, w0};
    vec4_t point_b = {x1, y1, z1, w1};
    vec4_t point_c = {x2, y2, z2, w2};

    // Render the upper part of the triangle (float-botto)
    //////////////////////////////////////////////////////
    float invSlope1 = 0;
    float invSlope2 = 0;
   
    if(y1 - y0 != 0) invSlope1 = (float)(x1 - x0) / abs((y1 - y0));
    if(y2 - y0 != 0) invSlope2 = (float)(x2 - x0) / abs((y2 - y0));
    
    if(y1 - y0 != 0)
    {
        for(int y = y0; y < y1; y++)
        {
            int xStart = x1 + (y - y1) * invSlope1;
            int xEnd = x0 + (y - y0) * invSlope2;

            if(xEnd < xStart)
            {
                IntSwap(&xStart, &xEnd);
            }
            for(int x  = xStart; x <= xEnd; x++)
            {
                DrawTrianglePixel(x, y, color, point_a, point_b, point_c);
            }
        }
    }
    //Render the bottom part of the triangle (flat-top)
    ////////////////////////////////////////////////////
    invSlope1 = 0;
    invSlope2 = 0;

    if(y2 - y1 != 0) invSlope1 = (float)(x2 - x1) / abs((y2 - y1));
    if(y2 - y0 != 0) invSlope2 = (float)(x2 - x0) / abs((y2 - y0));
    
    if(y2 - y1 != 0)
    {
        for(int y = y1; y < y2; y++)
        {
            int xStart = x1 + (y - y1) * invSlope1;
            int xEnd = x0 + (y - y0) * invSlope2;

            if(xEnd < xStart)
            {
                IntSwap(&xStart, &xEnd);
            }
            for(int x  = xStart; x <= xEnd; x++)
            {
                DrawTrianglePixel(x, y, color, point_a, point_b, point_c);
            }
        }
    }

}

//Calculate BarycentricWeights
vec3_t BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
    // Find the vectors between the vertices ABC and point p
    vec2_t ab = Vector2Sub(b, a);
    vec2_t bc = Vector2Sub(c, b);
    vec2_t ac = Vector2Sub(c, a);
    vec2_t ap = Vector2Sub(p, a);
    vec2_t bp = Vector2Sub(p, b);

    // Calcualte the area of the full triangle ABC using cross product (area of parallelogram)
    float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);

    // Weight alpha is the area of subtriangle BCP divided by the area of the full triangle ABC
    float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;

    // Weight beta is the area of subtriangle ACP divided by the area of the full triangle ABC
    float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;

    // Weight gamma is easily found since barycentric cooordinates always add up to 1
    float gamma = 1 - alpha - beta;

    vec3_t weights = { alpha, beta, gamma };
    return weights;
}

//DrawTexel(x, y, vec_t point_a, vec2_t point_b, vec2_t point_c,
//          u0, v0, u1, v1, u2, v2, texture);
void DrawTexel(
    int x, int y, upng_t* texture,
    vec4_t point_a, vec4_t point_b, vec4_t point_c,
    float u0, float v0, 
    float u1, float v1, 
    float u2, float v2 
){
    //TODO: implement this function
    vec2_t p = {x, y};
    vec2_t a = vec2FromVec4(point_a);
    vec2_t b = vec2FromVec4(point_b);
    vec2_t c = vec2FromVec4(point_c);

    vec3_t weights = BarycentricWeights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    const float EPSILON = 0.0000001;
    if(alpha < -EPSILON || beta < -EPSILON || gamma < -EPSILON)
    {
        return;
    }

    float interpolated_u;
    float interpolated_v;
    float interpolated_rw;

    interpolated_u = (u0 / point_a.w) * alpha + (u1 / point_b.w) * beta + (u2 / point_c.w) * gamma;
    interpolated_v = (v0 / point_a.w) * alpha + (v1 / point_b.w) * beta + (v2 / point_c.w) * gamma;

    interpolated_rw = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    interpolated_u /= interpolated_rw;
    interpolated_v /= interpolated_rw;

    int textureWidth = upng_get_width(texture);
    int textureHeight = upng_get_height(texture);

    int textureX = abs((int)(interpolated_u * textureWidth)) % textureWidth; 
    int textureY = abs((int)(interpolated_v * textureHeight)) % textureHeight;
    
    // adjust 1/w so the pixel that are closer to the camear have smaller values
    interpolated_rw = 1.0 - interpolated_rw;

    if(interpolated_rw < get_zbuffer_at(x, y))
    {
        //Get the buffer of colors from the texture
        uint32_t* textureBuffer = (uint32_t*)upng_get_buffer(texture);

        DrawPixel(x, y, textureBuffer[(textureWidth * textureY) + textureX]);
        //Update the z_buffer value whit the 1/w of this current pixel
        update_zbuffer_at(x, y, interpolated_rw);
    }

}


void DrawTextureTriangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    upng_t* texture
){
    if(y0 > y1)
    {
        IntSwap(&y0, &y1);
        IntSwap(&x0, &x1);
        FloatSwap(&z0, &z1);
        FloatSwap(&w0, &w1);
        FloatSwap(&u0, &u1);
        FloatSwap(&v0, &v1);
    }

    if(y1 > y2)
    {
        IntSwap(&y1, &y2);
        IntSwap(&x1, &x2);
        FloatSwap(&z1, &z2);
        FloatSwap(&w1, &w2);
        FloatSwap(&u1, &u2);
        FloatSwap(&v1, &v2);
    }

    if(y0 > y1)
    {
        IntSwap(&y0, &y1);
        IntSwap(&x0, &x1);
        FloatSwap(&z0, &z1);
        FloatSwap(&w0, &w1);
        FloatSwap(&u0, &u1);
        FloatSwap(&v0, &v1);
    }

    //Flip the v component to acount for inverted uv coodinates
    v0 = 1 - v0;
    v1 = 1 - v1;
    v2 = 1 - v2;
    //Create Vector Points
    vec4_t point_a = {x0, y0, z0, w0};
    vec4_t point_b = {x1, y1, z1, w1};
    vec4_t point_c = {x2, y2, z2, w2};

    // Render the upper part of the triangle (float-botto)
    //////////////////////////////////////////////////////
    float invSlope1 = 0;
    float invSlope2 = 0;
   
    if(y1 - y0 != 0) invSlope1 = (float)(x1 - x0) / abs(y1 - y0);
    if(y2 - y0 != 0) invSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    
    if(y1 - y0 != 0)
    {
        for(int y = y0; y < y1; y++)
        {
        int xStart = x1 + (y - y1) * invSlope1;
        int xEnd = x0 + (y - y0) * invSlope2;

            if(xEnd < xStart)
            {
                IntSwap(&xStart, &xEnd);
            }
            for(int x  = xStart; x <= xEnd; x++)
            {
                //TODO: render texture
                //DrawTexel(x, y, vec_t point_a, vec2_t point_b, vec2_t point_c,
                //          u0, v0, u1, v1, u2, v2, texture);
                DrawTexel(
                    x, y, texture,
                    point_a, point_b, point_c,
                    u0, v0, 
                    u1, v1, 
                    u2, v2 
                );
            }
        }
    }
    //Render the bottom part of the triangle (flat-top)
    ////////////////////////////////////////////////////
    invSlope1 = 0;
    invSlope2 = 0;

    if(y2 - y1 != 0) invSlope1 = (float)(x2 - x1) / abs(y2 - y1);
    if(y2 - y0 != 0) invSlope2 = (float)(x2 - x0) / abs(y2 - y0);
    
    if(y2 - y1 != 0)
    {
        for(int y = y1; y < y2; y++)
        {
            int xStart = x1 + (y - y1) * invSlope1;
            int xEnd = x0 + (y - y0) * invSlope2;

            if(xEnd < xStart)
            {
                IntSwap(&xStart, &xEnd);
            }
            for(int x  = xStart; x <= xEnd; x++)
            {
                //TODO: render texture
                DrawTexel(
                    x, y, texture,
                    point_a, point_b, point_c,
                    u0, v0, 
                    u1, v1, 
                    u2, v2 
                );
            }
        }
    }


}



/*
    //TODO: use z_buffer
    //we need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if(y0 > y1)
    {
        IntSwap(&y0, &y1);
        IntSwap(&x0, &x1);
    }

    if(y1 > y2)
    {
        IntSwap(&y1, &y2);
        IntSwap(&x1, &x2);
    }

    if(y0 > y1)
    {
        IntSwap(&y0, &y1);
        IntSwap(&x0, &x1);
    }

    if(y1 == y2)
    {   
        FillFlatBottom(x0, y0, x1, y1, x2, y2, color);
    }
    else if(y0 == y1)
    {
        FillFlatTop(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        //TODO: calculate midle point
        int my = y1;
        int mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
        //TODO: 
        //Draw Flat-Bottom triangle
        //Draw Flat-Top triangle
        FillFlatBottom(x0, y0, x1, y1, mx, my, color);
        FillFlatTop(x1, y1, mx, my, x2, y2, color);
    }

*/


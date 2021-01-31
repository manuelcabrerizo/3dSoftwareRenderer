#include <stdlib.h>
#include <stdio.h>
#include "display.h"
#include "math.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* colorBuffer = NULL;
static float* z_buffer = NULL;

static SDL_Texture* colorBufferTexture = NULL;
static int windowWidth = 1920;
static int windowHeight = 1080;

int GetWindowWidth(void)
{
    return windowWidth;
}

int GetWindowHeight(void)
{
    return windowHeight;
}

bool InitializeWindow(void)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    int fullScreenWidth = displayMode.w;
    int fullScreenHeight = displayMode.h;

    
    window = SDL_CreateWindow
    (
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        fullScreenWidth,
        fullScreenHeight,
        SDL_WINDOW_BORDERLESS  
    );

    if(!window)
    {
        fprintf(stderr, "Error creating the window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(!renderer)
    {
        fprintf(stderr, "Error creating the renderer.\n");
        return false;
    }

    colorBuffer = (uint32_t*) malloc(sizeof(uint32_t) * windowWidth * windowHeight);
    z_buffer = (float*) malloc(sizeof(float) * windowWidth * windowHeight);
    colorBufferTexture = SDL_CreateTexture
    (
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth,
        windowHeight
    );

    return true;
}


void DrawPixel(int xPos, int yPos, uint32_t color)
{
    if(xPos >= 0 && xPos < windowWidth && yPos >= 0 && yPos < windowHeight)
        colorBuffer[(windowWidth * yPos) + xPos] = color;
}


void DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    int deltaX = (x1 - x0);
    int deltaY = (y1 - y0);

    int sideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xInc = deltaX / (float)sideLength;
    float yInc = deltaY / (float)sideLength;

    float currentX = x0;
    float currentY = y0;

    for(int i = 0; i <= sideLength; i++)
    {
        DrawPixel(round(currentX), round(currentY), color);
        currentX += xInc;
        currentY += yInc;
    }
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    DrawLine(x0, y0, x1, y1, color);
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x0, y0, color);
}

void DrawRect(int xPos, int yPos, int width, int height, uint32_t color)
{
    for(int y = yPos; y < yPos + height; y++)
    {
        for(int x = xPos; x < xPos + width; x++)
        { 
            DrawPixel(x, y, color);
        }
    }
}

void DrawColorGrid(void)
{
    for(int y = 0; y < windowHeight; y += 10)
    {
        for(int x = 0; x < windowWidth; x += 10)
        {
            colorBuffer[(windowWidth * y) + x] = 0xFF333333;    
        }
    }
}

void RenderColorBuffer(void)
{
    SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)windowWidth * sizeof(uint32_t));
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void ClearColorBuffer(uint32_t color)
{
    for(int y = 0; y < windowHeight; y++)
    {
        for(int x = 0; x < windowWidth; x++)
        {
            colorBuffer[(windowWidth * y) + x] = color;
        }
    }
}

void DestroyWindow(void)
{
    free(z_buffer);
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void ClearZBuffer(void)
{
    for(int y = 0; y < windowHeight; y++)
    {
        for(int x = 0; x < windowWidth; x++)
        {
            z_buffer[(windowWidth * y) + x] = 1.0;
        }
    }
}

float get_zbuffer_at(int x, int y)
{
    if(x < 0 || x >= windowWidth || y < 0 || y >= windowHeight)
    {
        return 1.0;
    }
    return z_buffer[(y * windowWidth) + x];
}
void update_zbuffer_at(int x, int y, float value)
{
    if(x < 0 || x >= windowWidth || y < 0 || y >= windowHeight)
    {
        return;
    }
    z_buffer[(y * windowWidth) + x] = value;
}


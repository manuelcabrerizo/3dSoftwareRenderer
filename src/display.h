#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 144
#define FRAME_TARGET_TIME (1000 / FPS)



bool InitializeWindow(void);
void DrawColorGrid(void);
void DrawRect(int xPos, int yPos, int width, int height, uint32_t color);
void DrawPixel(int xPox, int yPos, uint32_t color);
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void RenderColorBuffer(void);
void ClearColorBuffer(uint32_t color);
void DestroyWindow(void);
void ClearZBuffer(void);

int GetWindowWidth(void);
int GetWindowHeight(void);

float get_zbuffer_at(int x, int y);
void update_zbuffer_at(int x, int y, float value);


#endif

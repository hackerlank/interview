#ifndef __DRAW_H_
#define __DRAW_H_

#include "font.h"

void DrawText(const char* text, UINT length, const Font* font, float x, float y);
void DrawPixel(UINT Xcoord, UINT Ycoord, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void Echo(const char* c);




#endif // __DRAW_H_
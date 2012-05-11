#ifndef __DRAW_H_
#include "font.h"

void PutPixel(UINT Xcoord, UINT Ycoord, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void PrintText(const char* text, const Font* font, UINT x, UINT y);
void PrintTextSafe(const char* text, UINT length, const Font* font, UINT x, UINT y);
void Circle(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius, float width, bool antialias);
//void CircleAnim(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius, float width, bool antialias);
void DrawRect(RECT rect);
void RectBox(LONG left, LONG top, LONG right, LONG bottom);
void Box(UINT x, UINT y, UINT width, UINT height);
void Box2(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
		  UCHAR red2, UCHAR green2, UCHAR blue2, UCHAR alpha2);
void FilledBox(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha);
void FilledBox2(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
				UCHAR red2, UCHAR green2, UCHAR blue2, UCHAR alpha2);
//void WUCircle(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius);
void PrintTextD(string text, Font* font, HDC* hDC, UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
			  float speed);

void DarkBox(int x1, int y1, int x2, int y2);
#endif

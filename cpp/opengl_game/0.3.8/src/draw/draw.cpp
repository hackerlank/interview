#include "stdafx.h"

#include "font.h"
#include "texture.h"
#include "../misc.h"
#include "draw.h"
#include "../config.h"
#include <math.h>


void PutPixel(UINT Xcoord, UINT Ycoord, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor4ub(red, green, blue, alpha);
	glBegin(GL_POINTS);
		glVertex2i(Xcoord, Ycoord + 1);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}


void PrintText(const char* text, const Font* font, UINT x, UINT y)
{
	if(text && font)
	{
		if(!font->tex_name)
		{
			glPushAttrib(GL_LIST_BIT);

#ifdef USE_FONT_BITMAPS
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
#endif

				glListBase(font->base/*-32*/);

#ifdef USE_FONT_BITMAPS
				glRasterPos2i(x, y + font->height-4);
#endif

#ifdef USE_FONT_OUTLINES
				glPushMatrix();
				float length = 0;
				for(unsigned int loop=0;
					loop < (strlen(text));
					loop++)	// Loop To Find Text Length
				{
					length += font->gmf[text[loop]].gmfCellIncX;		// Increase Length By Each Characters Width
				}

				glTranslatef((float)x-length, (float)y, 0);
#endif				
				glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
#ifdef USE_FONT_BITMAPS
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
#endif
#ifdef USE_FONT_OUTLINES
				glPopMatrix();
#endif
			glPopAttrib();
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);		

				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);

				glBindTexture(GL_TEXTURE_2D,
					TextureByName(font->tex_name)->tex);

				glPushMatrix();

					glTranslatef((float)x, (float)y, 0);
					glListBase(font->base);
					glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
				glPopMatrix();

			glPopAttrib();
		}
	}
}

void PrintTextSafe(const char* text, UINT length, const Font* font, UINT x, UINT y)
{
	if(text && font)
	{
		if(!font->tex_name)
		{
			glPushAttrib(GL_LIST_BIT);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
				glListBase(font->base/*-32*/);
				glRasterPos2i(x, y + font->height - 4);
					
				glCallLists(length, GL_UNSIGNED_BYTE, text);
				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
			glPopAttrib();
		}
		else
		{
			glPushAttrib(GL_LIST_BIT);		

				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);

				glBindTexture(GL_TEXTURE_2D,
					TextureByName(font->tex_name)->tex);

				glPushMatrix();

					glTranslatef((float)x, (float)y, 0);
					glListBase(font->base);
					glCallLists(length, GL_UNSIGNED_BYTE, text);
				glPopMatrix();

				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
			glPopAttrib();
		}
	}
}





void Circle(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius, float width, bool antialias)
{
	glColor4ub(red, green, blue, alpha);
	glDisable(GL_TEXTURE_2D);



	glBegin(GL_POINTS);
		for(int i = 0; i < 360; i++)
		{
			glVertex2d(x + cos(DEGTORAD(i))*radius, y + sin(DEGTORAD(i))*radius);
		}
	glEnd();
		
   glEnable(GL_TEXTURE_2D);
}

void RectBox(LONG left, LONG top, LONG right, LONG bottom)
{
	LONG w = right - left;
	LONG h = bottom - top ;
	LONG x = left;
	LONG y = top;
	Box(x, y, w, h);
}

void DrawRect(RECT rect)
{
	RectBox(rect.left, rect.top, rect.right, rect.bottom);
}


void Box(UINT x, UINT y, UINT width, UINT height)
{
	glDisable(GL_TEXTURE_2D);

	// 1 - - 2
	// |     |
	// |     |
	// 4 - - 3

	UINT w = width;
	UINT h = height;
	UINT _x = x;
	UINT _y = y;


	glBegin(GL_LINES);
		// 1 - - 2
		glVertex2i(_x, y);
		glVertex2i(x + w, y);

		// 2 - - 3
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);

		// 3 - - 4
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);

		// 4 - - 1
		glVertex2i(_x, y + h);
		glVertex2i(x, y);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void Box2(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
		  UCHAR red2, UCHAR green2, UCHAR blue2, UCHAR alpha2)
{
	glDisable(GL_TEXTURE_2D);

	float dx = float(width) / 2;
	float dy = float(height) / 2;
	float w = float(width);
	float h = float(height);

	float px = float(x);
	float py = float(y);

	// 1 - - 2
	// |     |
	// |     |
	// 4 - - 3

	glBegin(GL_LINES);
		// 1 - - 2
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px, py);
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px + w, py);

		// 2 - - 3
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px + w, py);
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px + w, py + h);

		// 3 - - 4
		glColor4ub(red, green2, blue, alpha);
		glVertex2f(px + w, py + h);
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px, py + h);

		// 4 - - 1
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px, py + h);
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px, py);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void FilledBox(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
{
	glColor4ub(red, green, blue, alpha);
	glDisable(GL_TEXTURE_2D);

	float dx = float(width) / 2;
	float dy = float(height) / 2;
	float w = float(width);
	float h = float(height);

	float px = float(x);
	float py = float(y);

	// 1 - - 2
	// |     |
	// |     |
	// 4 - - 3

	glBegin(GL_QUADS);
		// 1 - - 2
		glVertex2f(px, py);
		glVertex2f(px + w, py);

		// 2 - - 3
		glVertex2f(px + w, py);
		glVertex2f(px + w, py + h);

		// 3 - - 4
		glVertex2f(px + w, py + h);
		glVertex2f(px, py + h);

		// 4 - - 1
		glVertex2f(px, py + h);
		glVertex2f(px, py);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void FilledBox2(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
				UCHAR red2, UCHAR green2, UCHAR blue2, UCHAR alpha2)
{
	glDisable(GL_TEXTURE_2D);

	float dx = float(width) / 2;
	float dy = float(height) / 2;
	float w = float(width);
	float h = float(height);

	float px = float(x);
	float py = float(y);

	// 1 - - 2
	// |     |
	// |     |
	// 4 - - 3

	glBegin(GL_QUADS);
		// 1 - - 2
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px, py);
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px + w, py);

		// 2 - - 3
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px + w, py);
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px + w, py + h);

		// 3 - - 4
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px + w, py + h);
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px, py + h);

		// 4 - - 1
		glColor4ub(red2, green2, blue2, alpha2);
		glVertex2f(px, py + h);
		glColor4ub(red, green, blue, alpha);
		glVertex2f(px, py);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void DarkBox(int x1, int y1, int x2, int y2)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

//void CircleAnim(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float width, float radius, bool antialias)
//{
//
//
//	glColor4ub(red, green, blue, alpha);
//	glDisable(GL_TEXTURE_2D);
//
//	glBegin(GL_LINES);
//		for(int i = 0; i < 360; i += (10+ cos(DEGTORAD(current_time))) )
//		{
//			glVertex2f(x + cos(DEGTORAD(i))*radius, y + sin(DEGTORAD(i))*radius);
//		}
//	glEnd();
//		
//   glEnable(GL_TEXTURE_2D);
//}













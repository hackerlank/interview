#include "stdafx.h"
#include "font.h"
#include "texture.h"
#include "misc.h"
#include "draw.h"
#include "config.h"
#include <math.h>

extern int current_time;
extern config cfg;

//TODO: ANTIALIASING TO ALL, LINE WIDTHES

void PutPixel(UINT Xcoord, UINT Ycoord, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor4ub(red, green, blue, alpha);
	glBegin(GL_POINTS);
		glVertex2i(Xcoord, Ycoord);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

// very slow call
void TextOut(string text, font_t* font, HDC* hDC, UINT x, UINT y)
{
	if(!text.empty() && font != NULL && hDC != NULL)
	{
		glPushAttrib(GL_LIST_BIT);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glListBase(font->base);
			glRasterPos2i(x, y + font->height);
				
			glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
		glPopAttrib();
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

void Box(UINT x, UINT y, UINT width, UINT height, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
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

	glBegin(GL_LINES);
		// 1 - - 2
		glVertex2f(px, py);
		glVertex2f(px + w + 1, py);

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
		glVertex2f(px + w + 1, py);

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
		glVertex2f(px + w + 1, py);

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
		glVertex2f(px + w + 1, py);

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











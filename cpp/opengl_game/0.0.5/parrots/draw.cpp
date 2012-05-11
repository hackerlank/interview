#include "stdafx.h"
#include "font.h"
#include "texture.h"
#include "misc.h"
#include "draw.h"
#include "config.h"
#include <math.h>

#define pi 3.14159265
#define DEGTORAD(a) a * pi/180

extern int current_time;
extern config cfg;

//TODO: ANTIALIASING TO ALL, LINE WIDTHES

void PutPixel(UINT Xcoord, UINT Ycoord, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glColor4ub(red, green, blue, alpha);
	glBegin(GL_POINTS);
		glVertex2f(Xcoord, Ycoord);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void TextOut(string text, font_t* font, HDC* hDC, UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
{
	SelectObject(*hDC, font->font);
	wglUseFontBitmaps(*hDC, 0, 255, font->data);

	if(!text.empty() && font->font != NULL && hDC != NULL)
	{
		glPushAttrib(GL_LIST_BIT);
			glDisable(GL_TEXTURE_2D);
			//glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(red, green, blue, alpha);
			glListBase(font->data);
			glRasterPos2f(x, y + font->height);
			glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
			//glDisable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
		glPopAttrib();
	}
}

void TextOutD(string text, font_t* font, HDC* hDC, UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha,
			  float speed)
{
	static float ny;
	static float nr;
	static float ng;
	static float nb;

	ny += speed;
	if(nr + speed <= red) nr += speed*5;
	if(ng + speed <= green) ng += speed*5;
	if(nb + speed <= blue) nb += speed*5;


	TextOut(text, font, hDC, x, y - int(ny), red - int(nr), green - int(ng), blue - int(nb), alpha);
}
	



void Circle(UINT x, UINT y, UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha, float radius, float width, bool antialias)
{
	glColor4ub(red, green, blue, alpha);
	glDisable(GL_TEXTURE_2D);


	//glPushAttrib(GL_LINE_BIT);
	//if(antialias)
		//glEnable(GL_LINE_SMOOTH);


	glBegin(GL_POINTS);
		for(int i = 0; i < 360; i++)
		{
			glVertex2f(x + cos(DEGTORAD(i))*radius, y + sin(DEGTORAD(i))*radius);
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











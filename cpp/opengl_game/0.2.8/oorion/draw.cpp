#include "stdafx.h"

#include "font.h"
#include "texture.h"
#include "misc.h"
#include "draw.h"
#include "config.h"
#include <math.h>



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


void PrintText(const char* text, font_t* font, UINT x, UINT y)
{
	if(text && font)
	{
			if(!font->tex_name)
			{
				glPushAttrib(GL_LIST_BIT);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					glListBase(font->base-32);
					glRasterPos2i(x, y + font->height);
						
					glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
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
						glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
					glPopMatrix();

					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
				glPopAttrib();
			}
	}
}

void PrintTextSafe(const char* text, UINT length, font_t* font, UINT x, UINT y)
{
	if(text && font)
	{
			if(!font->tex_name)
			{
				glPushAttrib(GL_LIST_BIT);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_BLEND);
					glListBase(font->base-32);
					glRasterPos2i(x, y + font->height);
						
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
		glVertex2f(px + w, py);

		// 2 - - 3
		glVertex2f(px + w, py);
		glVertex2f(px + w, py + h);

		// 3 - - 4
		glVertex2f(px + w, py + h);
		glVertex2f(px-1, py + h);

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











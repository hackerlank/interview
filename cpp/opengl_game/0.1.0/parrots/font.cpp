#include "stdafx.h"
#include "font.h"
#include "misc.h"

extern int font_count;
extern HDC hDC;
extern char path_fonts[MAX_PATH];


bool LoadFont(char in_name[], font_t* font, BYTE height, LONG weight)
{
	static int count;
	char buf[100];
	string name;

	count++;



	font->font = CreateFont(-height, 0, 0, 0 , weight, 0, 0, 0, DEFAULT_CHARSET,
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
				FF_DONTCARE || DEFAULT_PITCH, in_name);

	font->height = height;
	font->weight = weight;

	//font->name = strcat("FONT", itoa(count, buf, 10));
	name = "Font" + string(itoa(count, buf, 10));
	font->name = (char*)name.c_str();
	if(!font)
	{
		LogToFile("debug.log", "Failed to load font: " + name);
		MessageBox(NULL, "Failed to load font", "CreateFont() Error", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	SelectObject(hDC, font->font);
	font->base = glGenLists(256);
	wglUseFontBitmaps(hDC, 0, 255, font->base);
	//wglUseFontOutlines(hDC, 0, 255, font->base, 0.0f, 0.5f, WGL_FONT_POLYGONS, font->lpgmf);
#ifdef _DEBUG_
	LogToFile("debug.log", "Font loaded: " + name);
#endif
	font_count++;
	return true;
}


//bool LoadFontFromFile(char in_name[], font_t* dest, int symbol_width, int symbol_height)
//{
//		// loading font from file (from app_path/fonts)
//		AUX_RGBImageRec* image = NULL;
//		char from[MAX_PATH];
//
//		strcpy(from, path_fonts);
//		strcat(from, "\\");
//		strcat(from, in_name);
//
//		image = auxDIBImageLoad(from);
//
//		if(!image)
//		{
//			image = NULL;
//			LogToFile("Failed to load font from file: " + string(in_name));
//			return false;
//		}
//		else
//		{
//			SetTransparent(image, true, 0, 0, 0, 100);
//			dest->base = glGenLists(256);
//			
//			glGenTextures(1, &dest->tex);
//			glBindTexture(GL_TEXTURE_2D, dest->tex);
//			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//			glTexImage2D(GL_TEXTURE_2D, 0, 4, image->sizeX, image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
//
//			float cx;
//			float cy;
//			int i;
//
//			for(i = 0; i < 256; i++)
//			{
//				cx = float(i % symbol_width) / float(symbol_width);
//				cy = float(i / symbol_height) / float(symbol_height);
//
//				glNewList(dest->base + i, GL_COMPILE);
//				glBegin(GL_QUADS);
//					glTexCoord2f(cx, 1-cy-1/symbol_height);
//					glVertex2i(0, 0);
//
//					glTexCoord2f(cx + 1/symbol_width, 1-cy-1/symbol_height);
//					glVertex2i(symbol_width, 0);
//
//                    glTexCoord2f(cx+1/symbol_width, 1-symbol_height);
//                    glVertex2i(symbol_width,symbol_height); 
//
//                    glTexCoord2f(cx,1-cy);
//                    glVertex2i(0,symbol_height);
//				glEnd();
//
//				glTranslated(10, 0, 0);
//				glEndList();
//			}
//		}
//
//}
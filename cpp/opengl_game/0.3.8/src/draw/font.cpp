#include "stdafx.h"

#include "font.h"
#include "../misc.h"

#include "texture.h"


vector<Font> fonts;
unsigned int font_count = 0;

extern HDC hDC;


bool LoadFont(const char* in_name, BYTE height, LONG weight, const char* out_name)
{
	static int count;
	string name;

	count++;

	Font font;



	font.font = CreateFont(-height, 0, 0, 0 , weight, 0, 0, 0, DEFAULT_CHARSET,
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
				FF_DONTCARE || DEFAULT_PITCH, in_name);


	font.height = height;
	font.weight = weight;

	font.tex_name = NULL;

	if(out_name == NULL)
	{
		font.name = new char[strlen("Font\0" + (count % 10) + 1)];
		sprintf(font.name, "Font%d", count);
	}
	else
	{
		font.name = new char[strlen(out_name) + 1];
		sprintf(font.name, out_name);
	}

	if(!font.font)
	{
		return false;
	}
	SelectObject(hDC, font.font);
	font.base = glGenLists(256);
#ifdef USE_FONT_BITMAPS
	wglUseFontBitmaps(hDC, 0, 256, font.base);
#else
#ifdef USE_FONT_OUTLINES
	wglUseFontOutlines(hDC, 0, 256, font.base, 0.0f, 0.2f, WGL_FONT_POLYGONS, font.gmf);
#endif
#endif


	fonts.push_back(font);
	font_count++;
	return true;
	
}

bool LoadFontFromFile(const char* file_name, UINT d_size, int d_margin)
{
	if(d_size <= 0)
		return false;

	if(!LoadTexture(file_name))
		return false;
	else
	{

		float cx;
		float cy;

		Font font;

		font.name = 0;
		font.font = 0;
		font.height = 0;
		font.weight = 0;
		font.tex_name = new char[strlen(file_name)+1];
		sprintf(font.tex_name, "%s\0", NameFromPath(string(file_name)).data());

		font.base = glGenLists(256);
		glBindTexture(GL_TEXTURE_2D, TextureByName(font.tex_name)->tex);



		for(UINT i = 0; i < 256; i++)
		{
			cx = float(i % d_size) / float(d_size);
			cy = float(i / d_size) / float(d_size);
			float dd = 0.0625f;

			glNewList(font.base + i, GL_COMPILE);

				glBegin(GL_QUADS);
					glTexCoord2f(cx, 1 - cy);
					glVertex2i(0, 0);

					glTexCoord2f(cx + dd,1 - cy);
					glVertex2i(d_size, 0);

					glTexCoord2f(cx + dd, 1 - cy - dd);
					glVertex2i(d_size, d_size);

					glTexCoord2f(cx, 1 - cy - dd);
					glVertex2i(0, d_size);
				glEnd();

				glTranslated(d_margin, 0, 0);

			glEndList();
		}


		char output[50];

		sprintf(output, "Font loaded: %s", font.name);
		LogToFile(output);

		fonts.push_back(font);
		font_count++;


		return true;
	}

}

Font* FontByName(char* name)
{
	for(vector<Font>::iterator it = fonts.begin();
		it != fonts.end();
		it++)
	{
		Font* f = &(*it);

		if(f->name)
		{
			if(strcmp(f->name, name) == 0)
				return f;
		}
		else if(f->tex_name)
		{
			if(strcmp(f->tex_name, name) == 0)
				return f;
		}
		
	}
	return NULL;
}
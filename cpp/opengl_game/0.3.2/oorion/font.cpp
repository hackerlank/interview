#include "stdafx.h"

#include "font.h"
#include "misc.h"

#include "texture.h"


vector<font_t> fonts;
unsigned int font_count = 0;

extern HDC hDC;


bool LoadFont(char* in_name, BYTE height, LONG weight, char* out_name)
{
	static int count;
	string name;
	char output[50];

	count++;

	font_t font;



	font.font = CreateFont(-height, 0, 0, 0 , weight, 0, 0, 0, DEFAULT_CHARSET,
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
				FF_DONTCARE || DEFAULT_PITCH, in_name);


	font.height = height;
	font.weight = weight;

	font.tex_name = NULL;

	if(out_name == 0)
	{
		font.name = new char[strlen("Font\0" + (count % 10) + 1)];
		sprintf(font.name, "Font%d", count);
	}
	else
	{
		font.name = new char[strlen(out_name)];
		sprintf(font.name, out_name);
	}

	if(!font.font)
	{
		sprintf(output, "Failed to load font: %s", font.name);
		LogToFile(output);
		return false;
	}
	SelectObject(hDC, font.font);
	font.base = glGenLists(96);
	wglUseFontBitmaps(hDC, 32, 96, font.base);


	sprintf(output, "Font loaded: %s", font.name);
	LogToFile(output);

	fonts.push_back(font);
	font_count++;
	return true;
	
}

bool LoadFontFromFile(char* file_name, UINT d_size, int d_margin)
{
	if(d_size <= 0)
		return false;

	if(!LoadTexture(file_name))
		return false;
	else
	{

		float cx;
		float cy;

		font_t font;

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


	


font_t* FontByName(char* name)
{
	for(vector<font_t>::iterator it = fonts.begin();
		it != fonts.end();
		it++)
	{
		font_t* f = &(*it);

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
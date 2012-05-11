#include "stdafx.h"

#include "font.h"
#include "misc.h"


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

font_t* FontByName(char* name)
{
	for(vector<font_t>::iterator it = fonts.begin();
		it != fonts.end();
		it++)
	{
		font_t* f = &(*it);

		if(strcmp(f->name, name) == 0)
			return f;
	}
	return NULL;
}
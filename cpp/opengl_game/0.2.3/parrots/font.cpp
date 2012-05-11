#include "stdafx.h"
#include "font.h"
#include "misc.h"

extern int font_count;
extern HDC hDC;
extern char path_fonts[MAX_PATH];


bool LoadFont(char* in_name, font_t* font, BYTE height, LONG weight, char* out_name)
{
	static int count;
	string name;
	char output[50];

	count++;



	font->font = CreateFont(-height, 0, 0, 0 , weight, 0, 0, 0, DEFAULT_CHARSET,
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
				FF_DONTCARE || DEFAULT_PITCH, in_name);

	font->height = height;
	font->weight = weight;

	if(out_name == 0)
		sprintf(font->name, "Font%d", count);
	else
		sprintf(font->name, out_name);

	if(!font->font)
	{
		sprintf(output, "Failed to load font: %s", font->name);
		LogToFile(output);
		return false;
	}
	SelectObject(hDC, font->font);
	font->base = glGenLists(256);
	wglUseFontBitmaps(hDC, 0, 255, font->base);


	sprintf(output, "Font loaded: %s", font->name);
	LogToFile(output);

	font_count++;
	return true;
}

font_t* FontByName(font_t* fonts, char* name)
{
	for(int i = 0; i < font_count; i++)
	{
		try
		{
			if(strcmp(fonts[i].name, name) == 0)
			{
				return &fonts[i];
			}
		}
		catch(...)
		{
			MessageBox(NULL, "Can't find font name", "Error", NULL);
			
		}

	}

	return NULL;
}
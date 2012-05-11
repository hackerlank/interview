#include "stdafx.h"
#include "font.h"
#include "misc.h"

extern int font_count;


bool LoadFont(char in_name[], font_t* font, BYTE height, LONG weight)
{
	static int count;
	char buf[100];
	string name;

	count++;

	font->data = glGenLists(256);

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

#ifdef _DEBUG
	LogToFile("debug.log", "Font loaded: " + name);
#endif
	font_count++;
	return true;
}
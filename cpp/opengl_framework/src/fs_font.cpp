//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
// Font stuff
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "fs_font.h"
#include "m_tools.h"

#include <gl/gl.h>


// this is our pointer to all loaded fonts

font_t* fonts = NULL;

// font counter

uint font_count = 0;



// load windows font
// in_name - name of the windows font (e.g. 'Courier New')
// height
// weight (400 - NORMAL, 700 - BOLD, etc..)
// out_name - font will be enabled in game under this name

extern HDC dc;

// characters count

#define BASE_COUNT 256

bool FS_LoadWindowsFont(const char* in_name, byte height, long weight,
					   const char* out_name)
{

	if(!in_name || !out_name)
	{
		I_ERROR("Incorrect in_name or out_name for loading windows font");
		return false;
	}

	font_t* font = NULL;
	font = new font_t;
	memset(font, '\0', sizeof(font_t));

	font->font = CreateFont(-height, 0,	0, 0, weight, 0, 0, 0,
		DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		FF_DONTCARE || DEFAULT_PITCH, in_name);

	if(!font->font)
	{
		I_ERROR("Failed to create font");
		return false;
	}

	font->height = height;
	font->weight = weight;
	font->name = new char[strlen(out_name) + 1];
	memset(font->name, '\0', strlen(out_name) + 1);
	sprintf(font->name, out_name);

	SelectObject(dc, font->font);
	font->base = glGenLists(BASE_COUNT);
	wglUseFontBitmaps(dc, 0, BASE_COUNT, font->base);


	font_count++;

	if(fonts == NULL)
		fonts = (font_t*)malloc(1 * sizeof(font_t));
	else
		fonts = (font_t*)realloc(fonts, font_count * sizeof(font_t));

	memcpy(&fonts[font_count - 1], font, sizeof(font_t));
	delete font;

	return true;
}



// free specified font

void FS_FreeFont(font_t* font)
{
	if(!font)
		return;

	font_t* old_memory = fonts;

	font_t* new_memory = (font_t*)malloc((font_count - 1) * sizeof(font_t));
	memset(new_memory, '\0', (font_count - 1) * sizeof(font_t));

	uint j = 0;
	for(uint i = 0; i < font_count; i++)
	{
		if(&old_memory[i] != font)
		{
			memcpy(&new_memory[j], &old_memory[i], sizeof(font_t));
			j++;
		}
	}

	font_count--;

	// free font
	if(font->name)
		delete[] font->name;

	// free object
	DeleteObject(font->font);

	// free gl-list
	glDeleteLists(font->base, BASE_COUNT);

	// now free old memory
	free(old_memory);

	fonts = new_memory;
}



void FS_FreeFont(const char* font_name)
{
	// just find font by name and free it, using FS_FreeFont(font_t*)

	FS_FreeFont(FS_FontByName(font_name));
}



// free all fonts

void FS_FreeFonts(void)
{
	for(;;)
	{
		if(font_count > 0)
			FS_FreeFont(&fonts[font_count-1]);
		else
		{
			free(fonts);
			break;
		}
	}
}



// get font by name

font_t* FS_FontByName(const char* font_name)
{
	for(uint i = 0; i < font_count; i++)
	{
		if(strcmp(fonts[i].name, font_name) == 0)
		{
			return &fonts[i];
		}
	}

	return NULL;
}
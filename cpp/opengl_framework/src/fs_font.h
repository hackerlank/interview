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

#ifndef __FS_FONT_H_
#define __FS_FONT_H_

#include "fs_def.h"

// for HFONT

#include <windows.h>

typedef struct {
	uint base;
	char* name;
	byte height;
	long weight;
	HFONT font;
	//texture_t* texture;
} font_t;


// load windows font
// in_name - name of the windows font (e.g. 'Courier New')
// height
// weight (400 - NORMAL, 700 - BOLD, etc..)
// out_name - font will be enabled in game under this name

bool FS_LoadWindowsFont(const char* in_name, byte height, long weight,
					   const char* out_name);


// free specified font

void FS_FreeFont(font_t* font);
void FS_FreeFont(const char* font_name);



// free all fonts

void FS_FreeFonts(void);



// get font by name

font_t* FS_FontByName(const char* font_name);

#endif // __FS_FONT_H_
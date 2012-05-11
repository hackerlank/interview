//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine-related
//
// DESCRIPTION:
//	Configuration file routines
//
//-----------------------------------------------------------------------------

#ifndef __FS_CONFIG_H_
#define __FS_CONFIG_H_

#include "fs_def.h"


typedef struct {

	// screen dimensions

	word scr_width;
	word scr_height;

	// window dimensions

	word wnd_width;
	word wnd_height;

	// bits per pixel
	// should be 16 or 24
	
	byte bpp;
	
	// 

	bool fullscreen;

	// show fps?

	bool show_fps;

} config_t;


//?

bool FS_LoadConfig(config_t* cfg, char* file_name);

//?

void FS_ApplyConfig(config_t* cfg);


#endif // __FS_CONFIG_H_
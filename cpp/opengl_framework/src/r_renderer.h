//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Render
//
// DESCRIPTION:
//	Renderer initializing and related routines
//
//-----------------------------------------------------------------------------

#ifndef __R_RENDERER_H_
#define __R_RENDERER_H_


// we need cfg-data to properly init renderer
// so

#include "fs_config.h"

// for HWND

#include <windows.h>


// this will initialize renderer
// true/false on return

bool R_InitRenderer(config_t* cfg, HWND window);


// resize viewport
void R_ResizeRenderer(uint width, uint height, HWND window);


// free renderer

void R_FreeRenderer(HWND window);


#endif // __R_RENDERER_H_
//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Windowing
//
// DESCRIPTION:
//	Window initializing and handling routines
//
//-----------------------------------------------------------------------------

#ifndef __W_WINDOW_H_
#define __W_WINDOW_H_

#include "fs_def.h"

// we need config to properly create window

#include "fs_config.h"

#include <windows.h>


// this will register window class

bool W_Init(void);



// this will unregister window class

bool W_Free(void);



//?

HWND W_CreateWindow(config_t* cfg);



//?

void W_DestroyWindow(HWND window);


#endif __W_WINDOW_H_ // __W_WINDOW_H_
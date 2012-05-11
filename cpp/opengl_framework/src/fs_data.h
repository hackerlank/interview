//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
// 
//
//-----------------------------------------------------------------------------

#ifndef __FS_DATA_H_
#define __FS_DATA_H_

#include "fs_def.h"



// arg count, got it when application started

extern int myargc;

// arg vector

extern char* myargv;

// current game state

extern int current_state;

// application time

extern int appl_time;

// startup time

extern int startup_time;

// screen

extern uint screen_width;
extern uint screen_height;



#endif // __FS_DATA_H_
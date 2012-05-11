//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Miscellaneous
//
// DESCRIPTION:
//	Misc. stuff, tools, logging etc
//
//-----------------------------------------------------------------------------

#ifndef __M_TOOLS_
#define __M_TOOLS_

//

#include "fs_def.h"


//?

void M_Echo(int stream, const char* message, ...);

#define I_ERROR(x) M_Echo(S_LOG, x)


// gets extension
// e.g. from 'textures/sky.bmp' will return 'bmp'

char* M_GetExtFromPath(const char* path);


// gets file name without extension
// e.g. from 'textures/sky.bmp' will return 'sky'
// NOTE: dont forget to free memory after usage

char* M_GetNameFromPath(const char* path);










#endif // __M_TOOLS_
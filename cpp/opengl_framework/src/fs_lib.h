//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
// Engine addition stuff.
// 
//
//-----------------------------------------------------------------------------

#ifndef __FS_LIB_H_
#define __FS_LIB_H_

#include "fs_def.h"
#include "fs_font.h"






// creates cube in specified position
//    |
//    |_ _ _ _ size
// (x,y,z)
//  /
//
//

int FS_CreateCube(int ix, int iy, int iz, int isize);


void FS_CreateSky(void);

void FS_UpdateSky(void);

void FS_SetObjectLabel(int object_id, const font_t* font, const char* text);








#endif // __FS_LIB_H_
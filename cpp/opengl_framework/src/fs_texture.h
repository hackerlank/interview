//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
// Texture
// 
//
//-----------------------------------------------------------------------------

#ifndef __FS_TEXTURE_H_
#define __FS_TEXTURE_H_

#include "fs_def.h"

typedef struct {
		uint tex;
		char* name;
		uint width;
		uint height;

} texture_t;





void FS_SetTransparent(byte** image, uint width, uint height, byte red, byte green, byte blue, byte percent);



// load texture
// path - in file, texture
// out_name - name of the texture, will be used in game

bool FS_LoadTexture(const char* path, const char* out_name);


// free specified texture

void FS_FreeTexture(texture_t* texture);
void FS_FreeTexture(const char* texture_name);



// free all textures

void FS_FreeTextures(void);



// get texture by name

texture_t* FS_TextureByName(const char* texture_name);







#endif // __FS_TEXTURE_H_
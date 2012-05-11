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

#include "stdafx.h"

#include <gl/gl.h>
#include <gl/glaux.h>

#include "fs_texture.h"
#include "m_tools.h"




// this is our pointer to all loaded textures

texture_t* textures = NULL;

// texture counter

uint texture_count = 0;


void FS_SetTransparent(byte** image, uint width, uint height, byte red, byte green, byte blue, byte percent)
{
	byte rrr;
	byte ggg;
	byte bbb;
	byte aaa;

	uint l = width * height;

	byte* newdata = (byte*)malloc(l * 4);

	byte* data = *image;

	for(uint i = 0; i < l; i++)
	{
		rrr = data[i*3];
		ggg = data[i*3 + 1];
		bbb = data[i*3 + 2];

		newdata[i*4] = rrr;
		newdata[i*4 + 1] = ggg;
		newdata[i*4 + 2] = bbb;

		if(percent < 100)
			if(rrr == red && ggg == green && bbb == blue)
				aaa = 0;
			else
				aaa = 255 / 100 * percent;
		else
			aaa = 255;

		newdata[i*4 + 3] = aaa;
	}
	
	free(*image);
	*image = newdata;
}

	



// load texture
// path - in file, texture
// out_name - name of the texture, will be used in game

bool FS_LoadTexture(const char* path, const char* out_name)
{

	if(!path)
	{
		I_ERROR("No path to load texture");
		return false;
	}

	char* ext = M_GetExtFromPath(path);

	uint width = 0;
	uint height = 0;
	byte* image = NULL;

	if(strcmp(ext, "bmp") == 0)
	{
		// BMP texture

		AUX_RGBImageRec* rec = auxDIBImageLoad(path);

		if(!rec)
		{
			I_ERROR("Failed to load texture");
			return false;
		}
		
		width = rec->sizeX;
		height = rec->sizeY;
		image = rec->data;
	}
	else
	{
		I_ERROR("This texture format is unsupported");
		return false;
	}

	FS_SetTransparent(&image, width, height, 0, 0, 0, 100);

	texture_t* texture = new texture_t;

	ZeroMemory(texture, sizeof(texture_t));

	texture->width = width;
	texture->height = height;

	// size should be 2^n, due to OpenGL
	// show warning

	if((width & (width - 1)) != 0 || (height & (height - 1)) != 0)
	{
		I_ERROR("Inappopriate texture size");
	}

	if(!out_name)
	{
		// take texture name from path
		// e.g. from 'textures/sky.bmp' will be 'sky'

		char* part = M_GetNameFromPath(path);
		
		if(!part)
		{
			I_ERROR("Failed to get name from texture path");
			return false;
		}

		texture->name = part;
	}
	else
	{
		texture->name = new char[strlen(out_name) + 1];
		memset(texture->name, '\0', strlen(out_name) + 1);
		strcpy(texture->name, out_name);
	}

	glGenTextures(1, &texture->tex);
	glBindTexture(GL_TEXTURE_2D, texture->tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texture->width, texture->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);

	free(image);

	texture_count++;

	if(textures == NULL)
		textures = (texture_t*)malloc(1 * sizeof(texture_t));
	else
		textures = (texture_t*)realloc(textures, texture_count * sizeof(texture_t));

	memcpy(&textures[texture_count - 1], texture, sizeof(texture_t));
	delete texture;

	return true;
}



// free specified texture

void FS_FreeTexture(texture_t* texture)
{
	if(!texture)
		return;

	texture_t* old_memory = textures;

	texture_t* new_memory = (texture_t*)malloc((texture_count-1) * sizeof(texture_t));
	memset(new_memory, '\0', (texture_count-1) * sizeof(texture_t));

	uint j = 0;
	for(uint i = 0; i < texture_count; i++)
	{
		if(&old_memory[i] != texture)
		{
			memcpy(&new_memory[j], &old_memory[i], sizeof(texture_t));
			j++;
		}
	}

	texture_count--;

	// free texture
	if(texture->name)
		delete[] texture->name;

	glDeleteTextures(1, &texture->tex);

	// now free old memory
	free(old_memory);

	textures = new_memory;
}


void FS_FreeTexture(const char* texture_name)
{
	// just find font by name and free it, using FS_FreeFont(font_t*)

	FS_FreeTexture(FS_TextureByName(texture_name));
}



// free all textures

void FS_FreeTextures(void)
{
	for(;;)
	{
		if(texture_count > 0)
			FS_FreeTexture(&textures[texture_count-1]);
		else
			break;
	}
}



// get texture by name

texture_t* FS_TextureByName(const char* texture_name)
{
	for(uint i = 0; i < texture_count; i++)
	{
		if(strcmp(textures[i].name, texture_name) == 0)
		{
			return &textures[i];
		}
	}

	return NULL;
}
#include "stdafx.h"
#include "ootypes.h"
#include "sprite.h"
#include "misc.h"

AUX_RGBImageRec* GetTexture(char *file_name)
{
	if(!file_name) return NULL;
	return auxDIBImageLoad(file_name);
};

int LoadTexture(texture* dest, char* path)
{

		AUX_RGBImageRec *temp_texture;
		
		if(!FileExists(path)) return -1;
		
		temp_texture = GetTexture(path);

		glGenTextures(1, &dest->image);
		glBindTexture(GL_TEXTURE_2D, dest->image);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, temp_texture->sizeX, temp_texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, temp_texture->data);

		if(temp_texture)
		{
			if(temp_texture->data)
				free(temp_texture->data);

			free(temp_texture);
		}

		dest->image_name = NameFromPath(path);
		return 0;

};



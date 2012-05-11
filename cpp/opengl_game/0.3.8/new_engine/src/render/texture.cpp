#include "StdAfx.h"

#include "../misc.h"
#include <png.h>
#include "texture.h"

CGLImageData::CGLImageData():height(0), width(0), data(NULL), TexID(0), ckR(157), ckG(236), ckB(0)
{
}

bool CGLImageData::LoadBMP(const char * name)
{
	DWORD dwDIBSize;
	BITMAPFILEHEADER bfh ;
	BITMAPINFO bi;
	FILE *file = NULL;
	BYTE bpp = 0;
	PBYTE R, G, B;
	BYTE temp;

	if ((file = fopen(name, "rb")) == NULL)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, 
			"Cant load bmp file: \"%s\" file doesnt exist or can't open", name);
		return false;
	}
	if (fread(&bfh, 1, sizeof(BITMAPFILEHEADER), file) != sizeof(BITMAPFILEHEADER))
	{
		if (!feof(file))
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "FILE IO Error. Can't read data.");
		return false;
	}

	if (bfh.bfType != 0x4d42)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, 
			"Cant load bmp file: \"%s\" is not a BMP file", name);
		return false;
	}
	if (fread(&bi, 1, sizeof(BITMAPINFOHEADER), file) != sizeof(BITMAPINFOHEADER))
	{
		if (!feof(file))
			sLog(DEFAULT_LOG_NAME,LOG_WARNING_EV, "FILE IO Error. Can't read data.");
		return false;
	}
	if (bi.bmiHeader.biBitCount != 24)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, 
			"Cant load bmp file: \"%s\" only 24 bits per pixel supported", name);
		return false;
	}

	width	= bi.bmiHeader.biWidth;
	height	= bi.bmiHeader.biHeight;
	bpp		= bi.bmiHeader.biBitCount / 8;

	dwDIBSize = bpp*width*height;
	data = new BYTE[dwDIBSize];

	if (fread(data, 1, dwDIBSize, file) != dwDIBSize)
	{
		if (!feof(file))
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "FILE IO Error. Can't read data.");
		return false;
	}

	for( UINT i = 0; i < (height*width); i++)
	{
		B = data + i*3 ;
		R = data + i*3 + 2;
		G = data + i*3 + 1;
		temp	= *B;
		*B		= *R;
		*R		= temp;
	}

	fclose(file);
	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "BMP image \"%s\" loaded", name); 
	return true;
}

bool  CGLImageData::MakeRGBA()
{
	if (data ==  NULL)
		return false;
	PBYTE _data;
	_data = new BYTE [width*height*4];
	if (_data == NULL)
		return false;
	PBYTE Rs,Gs,Bs,Ap,Rp,Gp,Bp;
	for(UINT i = 0;i < height*width; i++)
	{
		Rs = data + i*3;
		Gs = data + i*3 + 1;
		Bs = data + i*3 + 2;
		Rp = _data + i*4 ;
		Gp = _data + i*4 + 1;
		Bp = _data + i*4 + 2;
		Ap = _data + i*4 + 3;
		if ( *Rs == ckR && *Bs == ckB && *Gs == ckG ) //  Here is the color key check
			*Ap = 0;								// the color key is 255 0 255  
		else
			*Ap = 255;
		*Rp =*Rs;
		*Gp =*Gs;
		*Bp =*Bs;
	}
	delete [] data;
	data = _data;
	return true;
}

bool CGLImageData::LoadPNG(const char * filename)
{
	FILE *png_file = fopen(filename, "rb");
	if (png_file == NULL)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, 
			"Can't Load PNG file: \"%s\" can't open or doesn't exist", filename);
		return false;
	}

	BYTE header[PNG_SIG_BYTES];

	fread(header, 1, PNG_SIG_BYTES, png_file);
	if (png_sig_cmp(header, 0, PNG_SIG_BYTES))
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, 
			"Can't load PNG file: \"%s\" is not a PNG file.", filename);
		return false;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_infop end_info = png_create_info_struct(png_ptr);

	if(setjmp(png_jmpbuf(png_ptr)))
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, 
			"Can't load PNG file: \"%s\". Some error occured.", filename);
		return false;
	}
	png_init_io(png_ptr, png_file);
	png_set_sig_bytes(png_ptr, PNG_SIG_BYTES);
	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);

	png_uint_32 bit_depth, color_type;
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8(png_ptr);

	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);
	else if(color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png_ptr);
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);
	else
		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	png_read_update_info(png_ptr, info_ptr);

	png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	png_uint_32 numbytes = rowbytes*(height);
	png_byte* pixels = (png_byte *)malloc(numbytes);
	png_byte** row_ptrs = (png_byte **)malloc((height) * sizeof(png_byte*));

	
	for (UINT i=0; i<(height); i++)
		row_ptrs[i] = pixels + ((height) - 1 - i)*rowbytes;

	png_read_image(png_ptr, row_ptrs);

	free(row_ptrs);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(png_file);

	data = pixels;


	sLog(DEFAULT_LOG_NAME, LOG_INFO_EV, "PNG image \"%s\" loaded", filename); 
	return true;
}


bool CGLImageData::MakeTexture()
{
	if (data == NULL) 
		return false;
	if ((width&(width-1)) != 0) 
		return false;
	if ((height&(height-1)) != 0) 
		return false;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return true;
}

GLuint CGLImageData::GetTexID()
{
	if (TexID == 0)
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "GLImageData. Trying to access TexID but it is 0");
	return TexID;
}

bool CGLImageData::LoadTexture(const char *filename)
{
	if (!LoadBMP(filename))
		if(!LoadPNG(filename))
		{
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Can't load image->");
			return false;
		}
		else
		{
			if (!MakeTexture())
			{
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Can't load texture in video adapter.");
				return false;
			}
		}
	else
	{
		if(!MakeRGBA())
		{
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Can't load texture.");
			return false;
		}
		if (!MakeTexture())
		{
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Can't load texture in video memory.");
			return false;
		}
	}
	{
		delete [] data;
		data = NULL;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Texture* textures = NULL;		// ”казатель на массив загруженных текстур
UINT texture_count = 0;			//  оличество загруженных текстур


/**
*	f - ‘лаг, надо или не надо проводить замену по колоркею. red, green, blue -- цвет колорке€. percent -- процент
*	прозрачности, который мы хотим задать непопадающим под колоркей част€м картинки
*/
void SetTransparent(PBYTE image, bool f, UINT width, UINT height, BYTE red, BYTE green, BYTE blue, BYTE percent)
{
	UINT s = height*width;
	PBYTE newdata = new BYTE [s*4];	
	PBYTE Rs, Gs, Bs, Ad = 0, Rd, Gd, Bd;	// s значит source, d -- destination

	for(UINT i = 0; i < s; i++)
	{
		Rs = image + i*3;
		Gs = image + i*3 + 1;
		Bs = image + i*3 + 2;
		Rd = newdata + i*4 ;
		Gd = newdata + i*4 + 1;
		Bd = newdata + i*4 + 2;
		Ad = newdata + i*4 + 3;
		if(f)
		{
			if(*Rs != red  || *Gs != green || *Bs != blue)
				*Ad = BYTE(255.0f / 100.0f * (float)percent);
		}
		else
			*Ad = 255;
		*Rd =*Rs;
		*Gd =*Gs;
		*Bd =*Bs;
	}
	delete [] image;
	image = newdata;
}

// load texture
// path - in file, texture
// out_name - name of the texture, will be used in game

bool LoadTexture(const char* file_name, char* out_name)
{
	CGLImageData ImageData;
	if (ImageData.LoadTexture(file_name))
	{
		Texture* texture = new Texture;
		if(!out_name)
		{
			// take texture name from path
			// e.g. from 'textures/sky.bmp' will be 'sky'

			char* part = GetNameFromPath(file_name);

			if(!part)
			{
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to get name from texture path");
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

		texture->width = ImageData.width;
		texture->height = ImageData.height;
		texture->tex = ImageData.GetTexID();

		texture_count++;
		textures = (Texture*)realloc(textures, texture_count * sizeof(Texture));
		memcpy(&textures[texture_count - 1], texture, sizeof(Texture));
		delete texture;

		return true;
	}

	return false;
}

// ќсвобождает пам€ть от текстуры
void FreeTexture(Texture* texture)
{
	if(!texture)
		return;

	Texture* old_memory = textures;
	Texture* new_memory = NULL;
	if (texture_count > 1)
	{
		new_memory = (Texture*)malloc((texture_count-1) * sizeof(Texture));
		memset(new_memory, '\0', (texture_count-1) * sizeof(Texture));

		UINT j = 0;
		for(UINT i = 0; i < texture_count; i++)
		{
			if(&old_memory[i] != texture)
			{
				memcpy(&new_memory[j], &old_memory[i], sizeof(Texture));
				j++;
			}
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


void FreeTexture(const char* texture_name)
{
	// just find font by name and free it, using FS_FreeFont(font_t*)

	FreeTexture(TextureByName(texture_name));
}

// free all textures
void FreeTextures(void)
{
	// TODO: ћедленно же!!111
	for(;;)
	{
		if(texture_count > 0)
			FreeTexture(&textures[texture_count-1]);
		else
			break;
	}
}


// get texture by name
Texture* TextureByName(const char* texture_name)
{
	if(texture_name)
	{
		for(UINT i = 0; i < texture_count; i++)
		{
			if(strcmp(textures[i].name, texture_name) == 0)
			{
				return &textures[i];
			}
		}
	}
	sLog(DEFAULT_LOG_NAME, "Error", "ѕопытка получить текстуру по имени не увенчалась успехом");
	return NULL;
}


#include "stdafx.h"
#include "texture.h"
#include "../misc.h"

#ifdef USE_ZLIB
//#pragma comment(lib, "./zlib/lib/zdll.lib")
#endif

#ifdef USE_PNG
#include "../libpng/include/png.h"
#pragma comment(lib, "../libpng/include/libpng.lib")
#endif

vector<Texture> textures;
unsigned int tex_count = 0;

#ifdef _DEBUG
	unsigned int RENDERED_TEXTURES = 0;
#endif

void SetTransparent(AUX_RGBImageRec* image, bool f, BYTE red, BYTE green, BYTE blue, BYTE percent)
{
	BYTE RRR;
	BYTE GGG;
	BYTE BBB;
	BYTE AAA;

	BYTE* newdata;


	newdata = (BYTE*)malloc(image->sizeX * image->sizeY * 4);

	int l = image->sizeX * image->sizeY;

	for(int i = 0; i < l; i++)
	{
		RRR = (image->data[i*3]);
		GGG = (image->data[i*3+1]);
		BBB = (image->data[i*3+2]);

		newdata[i*4] = RRR;
		newdata[i*4+1] = GGG;
		newdata[i*4+2] = BBB;


		if(f)
			if(RRR == red  && GGG == green && BBB == blue)
				AAA = 0;
			else
				AAA = 255 / 100 * percent;
		else
			AAA = 255;

		newdata[i*4+3] = AAA;
	}

	image->data = (BYTE*)malloc(image->sizeX * image->sizeY * 4);
	memcpy(image->data, newdata, image->sizeX * image->sizeY * 4);
	free(newdata);
}

bool LoadTexture(const char* file_name)
{
	Texture texture;
	string ext;
	AUX_RGBImageRec *image = NULL;

	ext = ExtFromPath(file_name);

	if(ext == "bmp")
	{
		image = auxDIBImageLoad(file_name);
	}

	if(!image)
	{
		image = NULL;
		return false;
	}
	else
	{
		SetTransparent(image, true, 157, 236, 0, 100);
		texture.width = image->sizeX;
		texture.height = image->sizeY;

		// Проверка на степень двойки
		if((texture.width & (texture.width - 1)) != 0 ||
			(texture.height & (texture.height - 1)) != 0)
		{
			WriteToLog(DEFAULT_LOG_NAME, "Warning: inappropriate texture size");
		}



		texture.name = new char[strlen(NameFromPath(string(file_name)).c_str())+1];
		sprintf(texture.name, "%s", NameFromPath(string(file_name)).c_str());
		glGenTextures(1, &texture.tex);
		glBindTexture(GL_TEXTURE_2D, texture.tex);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, image->sizeX, image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
		free(image);
		textures.push_back(texture);
		tex_count++;
		return true;
	}
}

bool LoadTexture(const char* file_name, char* out_name)
{
	LoadTexture(file_name);
	textures[tex_count-1].name = out_name;
	return true;
}

void DrawTextureAt(UINT x, UINT y, Texture* tex)
{
     if (tex != NULL)
     {
		 UINT dx;
		 UINT dy;

		 dy = tex->height;
		 dx = tex->width;

		 glEnable(GL_TEXTURE_2D);
		 glBindTexture(GL_TEXTURE_2D, tex->tex);

		 glBegin(GL_QUADS);
			  glTexCoord2i(0, 1); glVertex2i(x, y);
			  glTexCoord2i(1, 1); glVertex2i(x + dx, y);
			  glTexCoord2i(1, 0); glVertex2i(x + dx, y + dy);
			  glTexCoord2i(0, 0); glVertex2i(x, y + dy);
		  glEnd();

     }
}

void DrawTexture(UINT x, UINT y, UINT x1, UINT y1, UINT x2, UINT y2, Texture* tex,
				 bool mirrorX, bool mirrorY, UINT scaleX, UINT scaleY)
{
	if(tex)
	{
		UINT CWidth;
		UINT CHeight;
		UINT dx = x2-x1;
		UINT dy = y2-y1;

		CWidth = tex->width;
		CHeight = tex->height;
		float cx1 = (float)x1 / CWidth;
		float cx2 = (float)x2 / CWidth;
		float cy1 = 1 - (float)y1 / CHeight;
		float cy2 = 1 - (float)y2 / CHeight;

		UINT iy = CHeight - y1;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex->tex);

		if (!mirrorX && !mirrorY)
		{	 
			glBegin(GL_QUADS);
				glTexCoord2f(cx1, cy1); glVertex2i(x, y);
				glTexCoord2f(cx2, cy1); glVertex2i(x + dx, y);
				glTexCoord2f(cx2, cy2); glVertex2i(x + dx, y + dy);
				glTexCoord2f(cx1, cy2); glVertex2i(x, y + dy);
			glEnd();

		}
		else if(mirrorY) 
		{
			float cx2my = 1-float(CWidth-x2)/CWidth;
			float cy2my = 1-float(CHeight-y2)/CHeight;
			glBegin(GL_QUADS);
			glTexCoord2f(cx1, cy1); glVertex2i(x, y);
			glTexCoord2f(cx2my, cy1); glVertex2i(x + dx, y);
			glTexCoord2f(cx2my, cy2my); glVertex2i(x + dx, y + dy);
			glTexCoord2f(cx1, cy2my); glVertex2i(x, y + dy);
			glEnd();
		}
		else if(mirrorX) 
		{
			glBegin(GL_QUADS);
			glTexCoord2f(cx2, cy1); glVertex2i(x - dx/2 + dx/4, y);
			glTexCoord2f(cx1, cy1); glVertex2i(x + dx/2 + dx/4, y);
			glTexCoord2f(cx1, cy2); glVertex2i(x + dx/2 + dx/4, y + dy);
			glTexCoord2f(cx2, cy2); glVertex2i(x - dx/2 + dx/4, y + dy);
			glEnd();
		}
	}
	else
	{
		glPushAttrib(GL_CURRENT_BIT);
		glColor3f(1.0f, 0.5f, 0.25f);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glVertex2i(x, y);
			glVertex2i(x + (x2-x1), y);
			glVertex2i(x + (x2-x1), y + (y2-y1));
			glVertex2i(x, y + (y2-y1));
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glPopAttrib();
	}
#ifdef _DEBUG
	RENDERED_TEXTURES++;
#endif
}

Texture* TextureByName(const char* name)
{
	for(vector<Texture>::iterator it = textures.begin();
		it != textures.end();
		it++)
	{
		Texture* t = &(*it);

		if(strcmp(t->name, name) == 0)
			return t;
	}
	return NULL;
}
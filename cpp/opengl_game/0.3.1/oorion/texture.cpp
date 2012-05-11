#include "stdafx.h"
#include "texture.h"
#include "misc.h"


vector<texture_t> textures;
unsigned int tex_count = 0;



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
	texture_t texture;
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
		LogToFile("Failed in loading " + string(file_name));
		return false;
	}
	else
	{
		SetTransparent(image, true, 0, 0, 0, 100);
	
		//// this really sux, should think how improve this
		//if((NameFromPath(string(file_name).c_str()) != "mainscreen"))
		//	SetTransparent(image, true, alphaRed, alphaGreen, alphaBlue, 100);
		//else
		//	SetTransparent(image, false, alphaRed, alphaGreen, alphaBlue, 100);

		LogToFile("Found texture: " + string(NameFromPath(file_name)));

		texture.width = image->sizeX;
		texture.height = image->sizeY;

		texture.name = new char[strlen(NameFromPath(string(file_name)).c_str())+1];
		sprintf(texture.name, "%s\0", NameFromPath(string(file_name)).c_str());

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
}

void DrawTextureAt(UINT x, UINT y, texture_t* tex)
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
		  glDisable(GL_TEXTURE_2D);
     }
}

void DrawTexture(UINT x, UINT y, UINT x1, UINT y1, UINT x2, UINT y2, texture_t* tex,
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
}

texture_t* TextureByName(char* name)
{
	for(vector<texture_t>::iterator it = textures.begin();
		it != textures.end();
		it++)
	{
		texture_t* t = &(*it);

		if(strcmp(t->name, name) == 0)
			return t;
	}
	return NULL;
}
#include "stdafx.h"
#include "texture.h"
#include "misc.h"

extern int tex_count;
extern texture_t textures[20];

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
		tex_count++;
		SetTransparent(image, true, 0, 0, 0, 100);
	
		//// this really sux, should think how improve this
		//if((NameFromPath(string(file_name).c_str()) != "mainscreen"))
		//	SetTransparent(image, true, alphaRed, alphaGreen, alphaBlue, 100);
		//else
		//	SetTransparent(image, false, alphaRed, alphaGreen, alphaBlue, 100);

		LogToFile("Found texture: " + string(NameFromPath(file_name)));

		textures[tex_count-1].width = image->sizeX;
		textures[tex_count-1].height = image->sizeY;
		strcpy(textures[tex_count-1].name, NameFromPath(string(file_name)).c_str());

		glGenTextures(1, &textures[tex_count-1].tex);
		glBindTexture(GL_TEXTURE_2D, textures[tex_count-1].tex);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, image->sizeX, image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);

		free(image);

		return true;
	}
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
		 //glColor3f(1.0, 1.0, 1.0);
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

		CWidth = tex->width;
		CHeight = tex->height;
			 glEnable(GL_TEXTURE_2D);
			 glBindTexture(GL_TEXTURE_2D, tex->tex);
			 if (!mirrorX && !mirrorY)
		{	 
			glBegin(GL_QUADS);
				glTexCoord2f((float)x1 / CWidth, 1 - (float)y1 / CHeight); glVertex2i(x, y);
				glTexCoord2f((float)x2 / CWidth, 1 - (float)y1 / CHeight); glVertex2i(x + x2-x1, y);
				glTexCoord2f((float)x2 / CWidth, 1 - (float)y2 / CHeight); glVertex2i(x + x2-x1, y + y2-y1);
				glTexCoord2f((float)x1 / CWidth, 1 - (float)y2 / CHeight); glVertex2i(x, y + y2-y1);
			glEnd();

		}
			 else
		{
			if(mirrorX) 
			{
			glBegin(GL_QUADS);
				  glTexCoord2f(float(x1)/CWidth, float(y1)/CHeight); glVertex2i(x, y);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, float(y1)/CHeight); glVertex2i(x + (x2-x1)*(scaleX/100), y);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x + (x2-x1)*(scaleX/100), y + (y2-y1)*scaleY/100);
				  glTexCoord2f(float(x1)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x, y + (y2-y1)*scaleY/100);
			glEnd();
			}
			if(mirrorY) 
			{
			  glBegin(GL_QUADS);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x, y);
				  glTexCoord2f(float(x1)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x + (x2-x1)*scaleX/100, y);
				  glTexCoord2f(float(x1)/CWidth, float(y1)/CHeight); glVertex2i(x + (x2-x1)*scaleX/100, y + (y2-y1)*scaleY/100);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, float(y1)/CHeight); glVertex2i(x, y + (y2-y1)*scaleY/100);
			  glEnd();
			}
		}
	}
}


texture_t* TextureByName(texture_t* textures, char* name)
{
	for(int i = 0; i < tex_count; i++)
	{
		try
		{
			if(strcmp(textures[i].name, name) == 0)
			{
				return &textures[i];
			}
		}
		catch(...)
		{
			MessageBox(NULL, "Can't find texture name", "Error", NULL);
			
		}

	}

	return NULL;
}

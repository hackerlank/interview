#include "stdafx.h"
#include "texture.h"
#include "misc.h"

extern int tex_count;

//#define dc 0

bool LoadTexturesFromFolder(LPCTSTR lpszDirectoryName, texture_t* textures)
{
    char tmp[MAX_PATH]; // временный массив
    char name[MAX_PATH];
	string ext;

    HANDLE hSearch = NULL;
    WIN32_FIND_DATA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	strcpy(name, lpszDirectoryName);
	strcpy(tmp, name);
	strcat(tmp, "\\*.*");

	LogToFile("Searching textures in: " + string(name));

	hSearch = FindFirstFile(tmp, &wfd);

	if(hSearch != INVALID_HANDLE_VALUE)
		do
		{
			if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2)) continue;

			if(wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				char next[MAX_PATH];

				strcpy(next, name);
				strcat(next, "\\");
				strcat(next, wfd.cFileName);
				LoadTexturesFromFolder(next, textures);
			}
			else
			{
				ext = ExtFromPath(wfd.cFileName);

				if(ext == "bmp" || ext == "png" || ext == "pcx" || ext == "tga")
				{
					char file[MAX_PATH];

					LogToFile("Found texture: " + string(wfd.cFileName));
				

					strcpy(file, name);
					strcat(file, "\\");
					strcat(file, wfd.cFileName);

					LoadTexture(file, &textures[tex_count], 255, 255, 255);
					tex_count++;
				}
			}

		}
		while(FindNextFile(hSearch, &wfd));

	FindClose(hSearch);
	return true;
}

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





bool LoadTexture(const char file_name[], texture_t* dest, int alphaRed, int alphaGreen, int alphaBlue)
{
	string ext;
	AUX_RGBImageRec *image = NULL;

	ext = ExtFromPath(file_name);
		//LogToFile(ext);
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
	
		// this really sux, should think how improve this
		if((NameFromPath(string(file_name).c_str()) != "mainscreen"))
			SetTransparent(image, true, alphaRed, alphaGreen, alphaBlue, 100);
		else
			SetTransparent(image, false, alphaRed, alphaGreen, alphaBlue, 100);

		dest->width = image->sizeX;
		dest->height = image->sizeY;
		strcpy(dest->name, NameFromPath(string(file_name)).c_str());

		glGenTextures(1, &dest->tex);
		glBindTexture(GL_TEXTURE_2D, dest->tex);
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
	glColor4ub(255, 255, 255, 255);
	if(tex)
	{
		UINT CWidth;
		UINT CHeight;
		CWidth = tex->width;
		CHeight = tex->height;
			 glEnable(GL_TEXTURE_2D);
			 glBindTexture(GL_TEXTURE_2D, tex->tex);
			 if (mirrorX==false&&mirrorY==false)
		{
			 //glColor4f(1.0, 1.0, 1.0, 1.0);
			 
			  glBegin(GL_QUADS);
				  glTexCoord2f(float(x1)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x, y);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x + (x2-x1)*scaleX/100, y);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, float(y1)/CHeight); glVertex2i(x + (x2-x1)*scaleX/100, y + (y2-y1)*scaleY/100);
				  glTexCoord2f(float(x1)/CWidth, float(y1)/CHeight); glVertex2i(x, y + (y2-y1)*scaleY/100);
			glEnd();

			//glColor4f(1.0, 1
		}
			 else
		{
			if(mirrorX) 
			{
			//glColor3f(1.0, 1.0, 1.0); 
			glBegin(GL_QUADS);
				  glTexCoord2f(float(x1)/CWidth, float(y1)/CHeight); glVertex2i(x, y);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, float(y1)/CHeight); glVertex2i(x + (x2-x1)*(scaleX/100), y);
				  glTexCoord2f(1-float(CWidth-x2)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x + (x2-x1)*(scaleX/100), y + (y2-y1)*scaleY/100);
				  glTexCoord2f(float(x1)/CWidth, 1-float(CHeight-y2)/CHeight); glVertex2i(x, y + (y2-y1)*scaleY/100);
			glEnd();
			}
			if(mirrorY) 
			{
			  //glColor3f(1.0, 1.0, 1.0);
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

UINT FindByName(texture_t textures[], char* name)
{
	for(int i = 0; i < tex_count; i++)
	{
		try
		{
			if(strcmp(textures[i].name, name) == 0)
			{
				return i;
			}
		}
		catch(...)
		{
			MessageBox(NULL, "Can't find texture name", "Error", NULL);
			
		}

	}

	return -1;
}

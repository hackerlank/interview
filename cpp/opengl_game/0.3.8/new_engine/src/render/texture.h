#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#define PNG_SIG_BYTES 8

struct Texture{
	GLuint tex;
	char* name;
	UINT width;
	UINT height;
	Texture()
	{
		tex = -1;
		name = "";
		width = 0;
		height = 0;
	}
};

/**
*	Этот класс внизу обёрнут в LoadTexture
*	значение ColorKey  спрятанов конструкторе
*/
class CGLImageData
{
public:
	PBYTE data;
	unsigned int height;
	unsigned int width;
	BYTE ckR, ckG, ckB;
	CGLImageData();
	bool	MakeTexture();
	bool    LoadTexture(const char *filename);
	virtual GLuint GetTexID();
	~CGLImageData()
	{
		if (data != NULL)
			delete [] data;
	}
private:
	bool isDrawing;
	bool	LoadBMP(const char *filename);
	bool	LoadPNG(const char *filename);
	bool	MakeRGBA();
	//RGBAub Pixel(int x, int y);

protected:
	GLuint TexID;
};


//bool LoadTexture(const char* file_name);
bool LoadTexture(const char* file_name, char* out_name);

void FreeTexture(Texture* texture);
void FreeTexture(const char* texture_name);
void FreeTextures();

//void DrawTextureAt(UINT x, UINT y, Texture* tex);
//void DrawTexture(UINT x, UINT y, UINT x1, UINT y1, UINT x2, UINT y2, Texture* tex,
//				 bool mirrorX, bool mirrorY, UINT scaleX, UINT scaleY);

Texture* TextureByName(const char* name);
void SetTransparent(BYTE** image, UINT width, UINT height, BYTE red, BYTE green, BYTE blue, BYTE percent);


#endif
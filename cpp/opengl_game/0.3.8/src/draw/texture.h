#ifndef __TEXTURE_H_
#define __TEXTURE_H_

struct Texture {
		GLuint tex;
		char* name;
		UINT width;
		UINT height;
};

bool LoadTexture(const char* file_name);
bool LoadTexture(const char* file_name, char* out_name);
void DrawTextureAt(UINT x, UINT y, Texture* tex);
void DrawTexture(UINT x, UINT y, UINT x1, UINT y1, UINT x2, UINT y2, Texture* tex,
				 bool mirrorX, bool mirrorY, UINT scaleX, UINT scaleY);

Texture* TextureByName(const char* name);
void SetTransparent(AUX_RGBImageRec* image, bool f, BYTE red, BYTE green, BYTE blue, BYTE percent);


#endif
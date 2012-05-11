typedef struct tagTEXTURE {
		GLuint tex;
		char name[50];
		UINT width;
		UINT height;
} texture_t;

bool LoadTexturesFromFolder(LPCTSTR lpszDirectoryName, texture_t* textures);
bool LoadTexture(const char file_name[], texture_t* dest, int alphaRed, int alphaGreen, int alphaBlue);
void DrawTextureAt(UINT x, UINT y, texture_t* tex);
void DrawTexture(UINT x, UINT y, UINT x1, UINT y1, UINT x2, UINT y2, texture_t* tex,
				 bool mirrorX, bool mirrorY, UINT scaleX, UINT scaleY);

UINT FindByName(texture_t* textures, char* name);
void SetTransparent(AUX_RGBImageRec* image, BYTE red, BYTE green, BYTE blue, BYTE percent);
typedef struct tagTEXTURE {
		GLuint tex;
		char* name;
		UINT width;
		UINT height;
} texture_t;

bool LoadTexturesFromFolder(LPCTSTR lpszDirectoryName, texture_t* textures);
bool LoadTexture(const char* file_name);
bool LoadTexture(const char* file_name, char* out_name);
void DrawTextureAt(UINT x, UINT y, texture_t* tex);
void DrawTexture(UINT x, UINT y, UINT x1, UINT y1, UINT x2, UINT y2, texture_t* tex,
				 bool mirrorX, bool mirrorY, UINT scaleX, UINT scaleY);

texture_t* TextureByName(char* name);
void SetTransparent(AUX_RGBImageRec* image, bool f, BYTE red, BYTE green, BYTE blue, BYTE percent);

//namespace oo_textures {
//		static vector<texture_t> textures;
//		static unsigned int tex_count = 0;
//}
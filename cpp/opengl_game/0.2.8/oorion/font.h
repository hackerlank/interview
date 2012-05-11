typedef struct tagFONT {
		GLuint base;
		char* name;
		UCHAR height;
		HFONT font;
		LONG weight;
		char* tex_name;
} font_t;

bool LoadFont(char* in_name, BYTE height, LONG weight, char* out_name = NULL);
bool LoadFontFromFile(char* file_name, UINT d_size, int d_margin);
font_t* FontByName(char* name);
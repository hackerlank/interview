typedef struct tagFONT {
		GLuint base;
		char* name;
		UCHAR height;
		HFONT font;
		LONG weight;
} font_t;

bool LoadFont(char* in_name, BYTE height, LONG weight, char* out_name = NULL);
font_t* FontByName(char* name);
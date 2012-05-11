typedef struct tagFONT {
		GLuint base;
		char name[20];
		UCHAR height;
		HFONT font;
		LONG weight;


} font_t;

bool LoadFont(char in_name[], font_t* font, BYTE height, LONG weight, char* out_name = NULL);
font_t* FontByName(font_t* fonts, char* name);

typedef struct tagFONT {
		GLuint data;
		char* name;
		UCHAR height;
		HFONT font;
		LONG weight;
} font_t;

bool LoadFont(char in_name[], font_t* font, BYTE height, LONG weight);
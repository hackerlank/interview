typedef struct tagFONT {
		GLuint base;
		char* name;
		UCHAR height;
		HFONT font;
		LONG weight;

		LPGLYPHMETRICSFLOAT  lpgmf; 

} font_t;

bool LoadFont(char in_name[], font_t* font, BYTE height, LONG weight);

#ifndef __FONT_H
#define __FONT_H

struct Font {
		GLuint base;
		char* name;
		UCHAR height;
		HFONT font;
		LONG weight;
		char* tex_name;
#ifdef USE_FONT_OUTLINES
		GLYPHMETRICSFLOAT gmf[256];
#endif
};

bool LoadFont(const char* in_name, BYTE height, LONG weight, const char* out_name);
bool LoadFontFromFile(const char* file_name, UINT d_size, int d_margin);
Font* FontByName(char* name);

#endif
//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Render
//
// DESCRIPTION:
//	Text drawing
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "r_text.h"
#include <gl/gl.h>





// text buffer to store raster-rendered lines

buf_text_line*		LINE_BUFFER				= NULL;
uint				ACTUAL_LINE_COUNT		= 0;
uint				PERFORMED_LINE_COUNT	= 0;



// print text line in (x,y) coord on screen with specified font

void R_DrawText(const char* text, uint length, const font_t* font, word x, word y)
{
	if(!text || !font)
		return;

	if(ACTUAL_LINE_COUNT == PERFORMED_LINE_COUNT)
	{
		PERFORMED_LINE_COUNT += 10;
		LINE_BUFFER = (buf_text_line*)realloc(LINE_BUFFER, PERFORMED_LINE_COUNT * sizeof(buf_text_line));
	}

	buf_text_line tmpline;

	tmpline.text = (char*)text;
	tmpline.length = length;
	tmpline.x = x;
	tmpline.y = y;
	tmpline.font = (font_t*)font;
	
	memcpy(&LINE_BUFFER[ACTUAL_LINE_COUNT], &tmpline, sizeof(buf_text_line));

	ACTUAL_LINE_COUNT++;

}

void R_AddTextBuffer(const buf_text_line* buffer, uint line_count)
{
	if(!buffer || !line_count)
		return;

	if(ACTUAL_LINE_COUNT + line_count > PERFORMED_LINE_COUNT)
	{
		PERFORMED_LINE_COUNT += (ACTUAL_LINE_COUNT + line_count - PERFORMED_LINE_COUNT) * 2;
		LINE_BUFFER = (buf_text_line*)realloc(LINE_BUFFER, PERFORMED_LINE_COUNT * sizeof(buf_text_line));
	}

	memcpy(&LINE_BUFFER[ACTUAL_LINE_COUNT], buffer, line_count * sizeof(buf_text_line));

	ACTUAL_LINE_COUNT += line_count;
}



void R_RenderText(void)
{
	if(!LINE_BUFFER)
		return;

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glLoadIdentity();

	for(uint i = 0; i < ACTUAL_LINE_COUNT; i++)
	{
		buf_text_line* line = &LINE_BUFFER[i];	

		glRasterPos2i(line->x, line->y);
		glListBase(line->font->base);
		glCallLists(line->length, GL_UNSIGNED_BYTE, line->text);

	}

	glPopMatrix();
}


void R_CleanTextBuffer(void)
{
	ACTUAL_LINE_COUNT = 0;
}


void R_TextCleanup(void)
{
	if(LINE_BUFFER)
		free(LINE_BUFFER);

	ACTUAL_LINE_COUNT = 0;
	PERFORMED_LINE_COUNT = 0;
	LINE_BUFFER = NULL;
}

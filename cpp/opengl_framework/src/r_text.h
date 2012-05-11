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

#include "fs_def.h"
#include "fs_font.h"


typedef struct {
	char* text; // null-terminated string
	uint length; //
	word x;
	word y;
	font_t* font;

} buf_text_line;



// print text line in (x,y) coord on screen with specified font

void R_DrawText(const char* text, uint length, const font_t* font, word x, word y);


// add line-buffer to the final output

void R_AddTextBuffer(const buf_text_line* buffer, uint line_count);


// actually render all printed text

void R_RenderText(void);


// clears text buffer

void R_CleanTextBuffer(void);


// free text buffer, have to call on application exit

void R_TextCleanup(void);
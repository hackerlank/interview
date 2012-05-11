//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
//	Main rendering routines
//	and other related stuff
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "fs_def.h"
#include "fs_data.h"
#include "fs_texture.h"
#include "fs_scene.h"

#include "r_text.h"
#include "r_objects.h"




#include <gl/gl.h>



// count fps
uint fps		=	1;
uint frames		=	0;
uint prev_tick	=	0;



void FS_SetOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, screen_width, screen_height, 0, -1.0, 1.0);
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -(float)screen_height, 0.0f);

	glMatrixMode(GL_MODELVIEW);
}

void FS_ResetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
}










// just clear scene

void FS_ClearScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}



// render scene using specified render_param_t struct.
// if NULL - render as default

void FS_RenderScene(render_param_t* r)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	FS_ResetPerspectiveProjection();


	// add here drawing stuff

	//glRotatef(15.0f, 0.0f, 0.0f, 0.0f);
	R_RenderAllObjects();


	// orthographical projection
	FS_SetOrthoProjection();

	static const font_t* font = FS_FontByName(DEFAULT_FONT);
	R_DrawText("huita", 5, font, 50, 70);

	R_RenderText();


	// after all increment frames counter

	frames++;

	// check if second elapsed

	if(appl_time - prev_tick >= 1000)
	{
		fps = frames;
		frames = 0;
		prev_tick = appl_time;
	}

	R_CleanTextBuffer();
}



// flip buffers and actually show rendered scene to user
extern HDC dc;

void FS_FlushScene(void)
{
	glFlush();
	
	SwapBuffers(dc);
}


//?
char fps_buffer[16];

void FS_PrintFPS(void)
{
	sprintf(fps_buffer, "FPS: %u\0", fps);
	static const font_t* font = FS_FontByName(DEFAULT_FONT);

	uint l = strlen(fps_buffer);
	R_DrawText(fps_buffer, l, font, FPS_X, FPS_Y);
}


// free memory, remove all objects, etc..

void FS_FreeScene(void)
{
	R_RemoveAllObjects();

	R_TextCleanup();
}

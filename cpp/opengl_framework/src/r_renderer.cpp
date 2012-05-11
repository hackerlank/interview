//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Render
//
// DESCRIPTION:
//	Renderer initializing and related routines
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "r_renderer.h"
#include "m_tools.h"

#include <gl/gl.h>
#include <gl/glu.h>

HDC		dc		=	NULL;
HGLRC	rc		=	NULL;

uint screen_width = 0;
uint screen_height = 0;


bool R_InitRenderer(config_t* cfg, HWND window)
{
	if(!cfg)
	{
		I_ERROR("No config to init renderer");
		return false;
	}

	dc = GetDC(window);
	
	if(!dc)
	{
		I_ERROR("Failed to get DC");
		return false;
	}

	// pixel format descriptor
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = cfg->bpp;

	// pixel format
	GLuint pf;

	pf = ChoosePixelFormat(dc, &pfd);

	if(!pf)
	{
		I_ERROR("Failed to choose pixel format");
		return false;
	}

	if(!SetPixelFormat(dc, pf, &pfd))
	{
		I_ERROR("Failed to set pixel format");
		return false;
	}

	rc = wglCreateContext(dc);

	if(!rc)
	{
		I_ERROR("Failed to create rc");
		return false;
	}

	if(!wglMakeCurrent(dc, rc))
	{
		I_ERROR("Failed to set current rc");
		return false;
	}

	// here go settings, viewport, clearcolor, glEnable/disable etc...

	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glReadBuffer(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_BLEND);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	R_ResizeRenderer(cfg->scr_width, cfg->scr_height, window);

	return true;
}


// resize viewport
void R_ResizeRenderer(uint width, uint height, HWND window)
{
	RECT rect;
	GetClientRect(window, &rect);

	int dx = rect.right - rect.left;
	int dy = rect.bottom - rect.top;

	glViewport(dx - width, height - dy, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// lower left corner is (0,0)
	//glOrtho(0, width, height, 0, -1.0, 1.0);
	gluPerspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);

	
	glTranslatef(0.0f, 0.0f, -1.0f);
	glRotatef(15.0f, 1.0f, 0.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// --
	screen_width = width;
	screen_height = height;
	
}



void R_FreeRenderer(HWND window)
{
	if(rc)
	{
		if(!wglMakeCurrent(NULL, NULL))
		{
			I_ERROR("Failed to release current context");
		}

		if(!wglDeleteContext(rc))
		{
			I_ERROR("Failed to delete rendering context");
		}
		rc = NULL;
	}

	if(dc)
	{
		if(ReleaseDC(window, dc) == 1)
		{
			I_ERROR("Failed to release device context");
		}

	}
}
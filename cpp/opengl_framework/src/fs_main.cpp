//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Engine core
//
// DESCRIPTION:
// Contains G_MainLoop()
//
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "fs_def.h"
#include "fs_data.h"

int current_state = 0;

int appl_time = 0;

int startup_time = 0;

#include "r_renderer.h"
#include "w_window.h"
#include "fs_scene.h"

#include "fs_font.h"
#include "fs_texture.h"

#include "fs_lib.h"

#include "m_tools.h"

#include "r_objects.h"


// TODO: доделать систему логирования


// game window is active?

extern bool active;


//?

void G_MainLoop(void)
{
	startup_time = GetTickCount();

	current_state = GS_FREE;

	W_Init();

	config_t cfg;

	cfg.bpp = 24;
	cfg.fullscreen = false;
	cfg.wnd_width = 1024;
	cfg.wnd_height = 768;
	cfg.scr_width = 1024;
	cfg.scr_height = 768;
	cfg.show_fps = true;

	HWND game_window = W_CreateWindow(&cfg);

	R_InitRenderer(&cfg, game_window);
	R_InitObjectSystem();

	// Load system font
	// This is used for printing out fps and some debug information

	FS_LoadWindowsFont(DEFAULT_FONT_NAME, 14, 400, DEFAULT_FONT);

	FS_CreateCube(50, 20, 0, 5);

	//FS_CreateCube(12, 10, 0, 10);
	
	//FS_CreateCube(50, 0, 0, 10);
	//FS_CreateCube(-50, 0, 0, 10);

	//int ddd = FS_CreateCube(0, 0, 0, 10);

	//FS_SetObjectLabel(ddd, FS_FontByName(DEFAULT_FONT), "kubik");

	do 
	{
		// count application time

		appl_time = GetTickCount() - startup_time;

		// WNDPROC

		MSG msg;

		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		// Render scene
		if(active)
		{

			FS_ClearScene();

			FS_RenderScene(NULL);

			if(cfg.show_fps)
				FS_PrintFPS();

			FS_FlushScene();
		}
		else
		{
			if(! current_state && GS_EXITING)
				WaitMessage();
		}

	} while(! current_state && GS_EXITING);

	FS_FreeScene();

	FS_FreeFonts();

	FS_FreeTextures();

	R_FreeRenderer(game_window);
	R_FreeObjectSystem();

	W_DestroyWindow(game_window);

	W_Free();

}
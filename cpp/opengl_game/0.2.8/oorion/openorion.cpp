#include "stdafx.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glaux.lib")
#pragma comment (lib, "glu32.lib")




#include "config.h"

#include "misc.h"
#include "font.h"
#include "texture.h"
#include "draw.h"

#include "game.h"
#include "gui.h"
#include "data.h"
#include "interpret.h"

#include "bmp.h"

#include "osl.h"

#include "scene.h"

#include "openorion.h"

#include "gui_misc.h"

#include "gui_ctrlalloc.h"
#include "gui_basecontrol.h"
#include "gui_vidget.h"




#ifdef _DEBUG_TIMINGS


		unsigned int drawtime;
		unsigned int drawtime_prv;
		unsigned int drawtime_med;
		unsigned int drawtime_med_dr;
		unsigned int cleartime;
		unsigned int cleartime_prv;
		unsigned int cleartime_med;
		unsigned int cleartime_med_dr;
		unsigned int mainlooptime;
		unsigned int mainlooptime_prv;
		unsigned int mainlooptime_med;
		unsigned int mainlooptime_med_dr;
		unsigned int fliptime;
		unsigned int fliptime_prv;
		unsigned int fliptime_med;
		unsigned int fliptime_med_dr;

#endif

		
		extern unsigned int startup_time;
		extern unsigned int current_time;
		extern unsigned int previous_time;

		extern unsigned int lasttick;
		extern unsigned int frames;
		extern unsigned int fps;

		extern bool keys[256];
		extern int mouseX;
		extern int mouseY;
		extern bool mouse[3];
		extern vector<UCHAR> pressed_keys;
		extern vector<UCHAR> unpressed_keys;

		extern unsigned int current_state;

		extern vector<font_t> fonts;
		extern char path_textures[MAX_PATH];




	extern HDC hDC;



//config cfg;


bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp);


bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp)
{
	AUX_RGBImageRec bmp;
	long size;
	char file_name[100];

	static int counter = 0;
	static int startup_time;
	static int last_time;
	int now_time = 0;

		counter++;

		sprintf(file_name, "screenshot%d.bmp", counter++);

		bmp.sizeX = scr_width;
		bmp.sizeY = scr_height;

		int in_bpp;

		switch(scr_bpp)
		{
		case 16:
			in_bpp = 24;
		case 24:
			in_bpp = 32;
		}


		size = bmp.sizeX * bmp.sizeY * int(in_bpp / 8);

		bmp.data = new GLubyte[size];

		glReadPixels(0, 0, bmp.sizeX, bmp.sizeY, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.data);

		if(!bmpWrite(&bmp, file_name) == 0)
		{
			delete[] bmp.data;
			return false;
		}
		else
		{
			delete[] bmp.data;
			return true;
		}

	return false;
}





void CountFPS()
{
	if(GetTickCount() - lasttick >= 1000)
	{
		fps = frames;
		frames = 0;
		lasttick = GetTickCount();

#ifdef _DEBUG_TIMINGS

		if (fps != 0)
		{
			drawtime_med_dr = drawtime_med / fps;
			cleartime_med_dr = cleartime_med / fps;
			fliptime_med_dr = fliptime_med / fps;
			mainlooptime_med_dr = mainlooptime_med / fps;
			drawtime_med = 0;
			cleartime_med = 0;
			fliptime_med = 0;
			mainlooptime_med = 0;
		}
#endif
	}

}







static void InitOGLSettings()
{

	// enable textures
	 glEnable(GL_TEXTURE_2D);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// enable alpha
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);

	// enable depth
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	// reading is set to read from back buffer
	//glReadBuffer(GL_BACK);

	// black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // default depth
	//glClearDepth(1.0f);


	//glEnable(GL_LINE_SMOOTH);

	//glLineWidth(3);

}


void MainLoop()
{
	MSG msg;

#ifdef _DEBUG_TIMINGS
	mainlooptime = GetTickCount() - mainlooptime_prv;
#endif
	//unpressed_keys.clear();
	//if(pressed_keys.size() == 0)
	//	unpressed_keys.clear();

	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
			DispatchMessage(&msg);

				if (keys[VK_F11])
				{
					LogToFile("Saving screenshot...");
					if(gui)
						gui->LogOnScreen("Saving screenshot");
					if(!SaveScreenshot(cfg.scr_width, cfg.scr_height, cfg.scr_bpp))
						LogToFile("Failed save screenshot");
				}

				if (keys[VK_ESCAPE])
				{
					current_state = GAME_EXITING;
				}

				if (keys[VK_F5])
				{
					cfg.debug = !cfg.debug;
				}

	}

	current_time = (GetTickCount() - startup_time);

	if(gui)
		gui->Process();
	
	CountFPS();	

#ifdef _DEBUG_TIMINGS

	mainlooptime_prv = GetTickCount();

	mainlooptime_med += mainlooptime;

#endif

}



void InitGUI()
{
	gui = new GUI();
}

void FreeGUI()
{
	if(gui)
	{
		gui->FreeMessages();
		gui->FreeScreenMessages();
		gui->FreeControls();
		delete gui;
	}
	gui = NULL;
}


void FreeFonts()
{
	for(vector<font_t>::iterator it = fonts.begin();
		it != fonts.end();
		it++)
	{
		font_t* f = &(*it);
		glDeleteLists(f->base, 96);
	}
	fonts.clear();
}

void DumpFonts()
{
	LogToFile("Dumping fonts...");
	char output[100];

	for(vector<font_t>::iterator it = fonts.begin();
		it != fonts.end();
		it++)
	{	
		font_t* f = &(*it);
		sprintf_s(output, 100, "%s %u %u", f->name, f->height, f->weight);
		LogToFile(string(output));
	}

	LogToFile("Finished dumping...");
}



//void InitTextures()
//{
//	char result[100];
//
//	strcpy(result, path_textures);
//	strcat(result, "\\*.bmp");
//	SearchFiles(result, (LPSEARCHFUNC)LoadTexture, true);
//}


int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
	startup_time = GetTickCount();
	mouse[0] = false;
	mouse[1] = false;
	mouse[2] = false;
	srand((unsigned)time(NULL));

	LogToFile("Log started");

	InitWindow(hInstance);

	osl_Load_v_04("init.txt");

	InitPaths();
	LogPaths();

	InitOGLSettings();


	while(current_state != GAME_EXITING)
	{
		MainLoop();
		ClearScene();
		DrawScene();
		Flip();
	}


	// Now terminating all
	FreeGUI();
	FreeFonts();

	KillWindow(hInstance);

	LogToFile("Finished logging");
	return 0;
}
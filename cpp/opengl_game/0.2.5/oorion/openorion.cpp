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






extern const int menu_length;
extern const int main_length;
extern const int difficult_length;
extern const int size_length;
extern const int race_length;
extern const int opponents_length;

extern unsigned int current_state;

extern const LPSTR names_menu[];
extern const LPSTR names_main[];
extern const LPSTR names_difficulty[];
extern const LPSTR names_size[];
extern const LPSTR names_race[];
extern const LPSTR names_opponents[];


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
		delete gui;
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



void InitTextures()
{
	char result[100];

	strcpy(result, path_textures);
	strcat(result, "\\*.bmp");
	SearchFiles(result, (LPSEARCHFUNC)LoadTexture, true);
}


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
	LoadConfig("default.cfg", &cfg);
	ResizeScene(cfg.scr_width, cfg.scr_height);
	osl_Load_v_04("test.txt");

	InitPaths();
	LogPaths();

	InitTextures();
	InitOGLSettings();



	while(current_state != GAME_EXITING)
	{
		MainLoop();
		ClearScene();
		DrawScene();
		Flip();
	}


	// Now terminating all
	FreeFonts();
	FreeGUI();
	KillWindow(hInstance);

	LogToFile("Finished logging");




}
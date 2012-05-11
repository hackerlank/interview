#include "stdafx.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glaux.lib")
#pragma comment (lib, "glu32.lib")

#include "misc.h"

#include "iichan/iichan.h"
#include "iichan/iichan_resource.h"
#include "gui/gui.h"
#include "draw/bmp.h"
#include "osl/osl.h"
#include "scene.h"

#include "sound/snd.h"
#include "openorion.h"


extern unsigned int current_state;
extern vector<font_t> fonts;
extern vector<RESOURCE> resources;
extern vector<texture_t> textures;
extern bool active;

unsigned int startup_time;
unsigned int current_time;
unsigned int previous_time;
unsigned int frames = 0;
unsigned int fps = 1;

bool keys[256];
vector<UCHAR> pressed_keys;






extern char path_app[MAX_PATH];

#define KEYDOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)



bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp);

inline void MainLoop();

bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp)
{
	AUX_RGBImageRec bmp;

	char file_name[100];

	static int counter = 0;

count:
	counter++;
	sprintf(file_name, "screenshot%d.bmp", counter);

	if(FileExists(file_name))
		goto count;


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

	long size = bmp.sizeX * bmp.sizeY * (in_bpp / 8);

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

	return true;
}

void CountFPS()
{
	static int lasttick;
	if(current_time - lasttick >= 1000)
	{
		fps = frames;
		frames = 0;
		lasttick = current_time;
	}
}


void ProcessKeyboard()
{
	static string keyboard;
	static int prev;

	for(UCHAR i = 0; i < 255; i++)
	{
		if(KEYDOWN(i))
		{
			keys[i] = true;
			
			if(keyboard.find(char(i)) == keyboard.npos)
			{
				// key pressed
				keyboard += char(i);
				pressed_keys.push_back(i);

				if(keys[VK_F5])
				{
					cfg.debug = !cfg.debug;
				}
				if(keys[cfg.screenshot])
				{
					LogToFile("Saving screenshot...");
					if(gui)
					{
						char* msg = new char[32];
						sprintf(msg, "Saving screenshot...");
						gui->LogOnScreen(msg);
					}
					if(!SaveScreenshot(cfg.window_width, cfg.window_height, cfg.scr_bpp))
						LogToFile("Failed to save screenshot");
				}
				if(keys[VK_ESCAPE])
				{
					current_state = GAME_EXITING;
				}
			}
			prev = current_time;
		}
		else
		{
			keys[i] = false;
			int p = keyboard.find(char(i));
			if(p != keyboard.npos)
			{
				// key unpressed
				std::swap(keyboard.at(p), *(keyboard.end()-1));
				keyboard.resize(keyboard.size() - 1);
				std::swap(pressed_keys.at(p), *(pressed_keys.end()-1));
				pressed_keys.pop_back();
			}
		}
	}
}

void MainLoop()
{
	current_time = (GetTickCount() - startup_time);

	ProcessKeyboard();

	if(gui)
		gui->Process();
	if(iichan)
		iichan->Move();
	
	CountFPS();	
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
		gui = NULL;
	}
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

void FreeTextures()
{
	textures.clear();
}

void FreeResources()
{
	resources.clear();
}

void FreeIICHAN()
{
	if(iichan)
	{
		delete iichan;
		iichan = NULL;
	}
}

void FreeSND()
{
	if(snd)
	{
		snd->DestroyBass();
		delete snd;
		snd = NULL;
	}	
}

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
	InitPaths();
	SetCurrentDirectory(path_app);
	LogToFile("Log started");
	LogPaths();


	// -----------------------
	cfg.window_height = 480;
	cfg.window_width = 640;
	cfg.scr_width = 640;
	cfg.scr_height = 480;
	cfg.scr_bpp = 16;
	cfg.fullscreen = false;
	// -----------------------

	hInst = hInstance;
	startup_time = GetTickCount();
	srand((unsigned)time(NULL));
	InitWindow(hInstance, true);
	osl_Load_v_04("scripts/init.txt");
	current_state = GAME_MAINMENU;
	
	while(current_state != GAME_EXITING)
	{
		MSG msg;
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		if(active)
		{
			MainLoop();
			ClearScene();
			DrawScene();
			Flip();
		}
	} 

	FreeSND();
	FreeIICHAN();
	FreeGUI();
	FreeFonts();
	KillWindow(hInstance);
	LogToFile("Finished logging");
	return 0;
}


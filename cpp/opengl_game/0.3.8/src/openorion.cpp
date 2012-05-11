#include "stdafx.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glaux.lib")
#pragma comment (lib, "glu32.lib")

#include "misc.h"
#include "osl/osl.h"
#include "scene.h"

extern char path_app[MAX_PATH];
extern unsigned int startup_time;
extern unsigned int current_state;
extern bool active;
extern HINSTANCE hInst;
extern config cfg;

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
	InitPaths();
	SetCurrentDirectory(path_app);
	CheckPaths();
	CleanupLogs();
	LogPaths();

	CheckDefaultConfig();

	hInst = hInstance;
	startup_time = GetTickCount();
	srand((unsigned)time(NULL));
	scene::InitWindow(hInstance, cfg.fullscreen);

	//scene::PrintExtensions();
	LogToFile("Checking for required OpenGL-extensions...");
	//scene::InitExtensions();


	osl_Init();

	if(!osl_Load("scripts/init.txt",false))
	{
		WriteToLog("No init: aborting game");
		current_state = GAME_EXITING;
	}
	else
	{
		current_state = GAME_MAINMENU;
	}
	
	do 
	{
		MSG msg;
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}

		if(active)
		{
			scene::MainLoop();
			scene::ClearScene();
			scene::DrawScene();
			scene::Flip();
		}
		else
		{
			if(current_state != GAME_EXITING)
				WaitMessage();
		}
	} while(current_state != GAME_EXITING);

	osl_Free();

	//scene::FreeExtensions();
	scene::FreeSND();
	scene::Freeii();
	scene::FreeConsole();
	scene::FreeGUI();
	scene::FreeFonts();
	scene::KillWindow(hInstance);
	EndupLogs();
	return 0;
}
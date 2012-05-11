#include "stdafx.h"

#define _DEBUG_TIMINGS
//#define _COUNT_ONLY_FPS

#define OORION_EXPORTS


#include "config.h"
#include "game.h"
#include "misc.h"
#include "font.h"
#include "texture.h"
#include "draw.h"
#include "gui.h"

#include "openorion.h"

#include "..\oorion\oorion.h"

#pragma comment(lib, "oorion.lib")

#define SECOND_ELAPSED current_time - previous_time >= 1000
#define GAMENAME "OpenOrion"
#define VERSION "version 0.1.1"
#define PRINT_VERSION
#define VERSION_X 450
#define VERSION_Y 70

//#define MAINMENU_NAME "main.menu"

long __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{

	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_KEYDOWN:
		{
			keys[wParam] = true;
			return 0;
		}
	case WM_KEYUP:
		{
			keys[wParam] = false;
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam) + 25;
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			mouse[0] = true;
			//switch(wParam)
			//{
			//case MK_LBUTTON:
			//	mouse[0] = true;
			//case MK_MBUTTON:
			//	mouse[2] = true;
			//case MK_RBUTTON:
			//	mouse[1] = true;
			//}
			return 0;
		}
	case WM_LBUTTONUP:
		{
			mouse[0] = false;
			//switch(wParam)
			//{
			//	
			//case MK_LBUTTON:
			//	mouse[0] = false;
			//case MK_MBUTTON:
			//	mouse[2] = false;
			//case MK_RBUTTON:
			//	mouse[1] = false;
			//}
			return 0;
		}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	
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

void PrintDebug()
{
	glColor4ub(0, 255, 0, 255);
#ifndef _COUNT_ONLY_FPS
	char time_elapsed[10];
	char mx[6];
	char my[6];
	_itoa(current_time / 1000 , time_elapsed , 10);
	_itoa(mouseX, mx, 10);
	_itoa(mouseY, my, 10);
	TextOut("Time elapsed: " + string(time_elapsed), &fonts[0], &hDC, 5, 25);
	TextOut("Mouse X: " + string(mx), &fonts[0], &hDC, 5, 55);
	TextOut("Mouse Y: " + string(my), &fonts[0], &hDC, 5, 70);
#endif
#ifdef _DEBUG_TIMINGS
	char draw[10];
	char flip[10];
	char clear[10];
	char mainloop[10];
	_itoa(drawtime_med_dr, draw, 10);
	_itoa(fliptime_med_dr, flip, 10);
	_itoa(cleartime_med_dr, clear, 10);
	_itoa(mainlooptime_med_dr, mainloop, 10);
	TextOut("Average clear time: " + string(clear), &fonts[0], &hDC, 5, 85);
	TextOut("Average mainloop time: " + string(mainloop), &fonts[0], &hDC, 5, 100);
	TextOut("Average draw time: " + string(draw), &fonts[0], &hDC, 5, 115);
	TextOut("Average flip time: " + string(flip), &fonts[0], &hDC, 5, 130);
#endif


	char frames_per_second[10];
	_itoa(fps, frames_per_second, 10);

	TextOut("Average frames per second: " + string(frames_per_second), &fonts[0], &hDC, 5, 40);
	//TextOut("abc", &fonts[0], &hDC, 5, 80);



}


void ClearScene()
{
#ifdef _DEBUG_TIMINGS
	cleartime = GetTickCount() - cleartime_prv;
#endif

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

#ifdef _DEBUG_TIMINGS

	cleartime_prv = GetTickCount();

	cleartime_med += cleartime;

#endif
}

void DrawScene()
{
#ifdef _DEBUG_TIMINGS
	drawtime = GetTickCount() - drawtime_prv;
#endif
	
	if(gui) gui->DrawAll();

#ifdef _DEBUG_
	PrintDebug();
#endif
#ifdef PRINT_VERSION
	#ifdef VERSION
		if(current_state == GAME_MAINMENU)
			TextOut(VERSION, &fonts[0], &hDC, VERSION_X, VERSION_Y);
	#endif
#endif

	frames++;

#ifdef _DEBUG_TIMINGS

	drawtime_prv = GetTickCount();

	drawtime_med += drawtime;

#endif
}

void MainLoop()
{

#ifdef _DEBUG_TIMINGS
	mainlooptime = GetTickCount() - mainlooptime_prv;
#endif

	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
			DispatchMessage(&msg);

				if (keys[VK_F11])
				{
					LogToFile("Saving screenshot");
					SaveScreenshot(cfg.scr_width, cfg.scr_height, cfg.scr_bpp);
				}

				if (keys[VK_ESCAPE])
				{
					current_state = GAME_EXITING;

				}
	}

	current_time = (GetTickCount() - startup_time);

	if(gui) gui->Process();
	
	CountFPS();	

#ifdef _DEBUG_TIMINGS

	mainlooptime_prv = GetTickCount();

	mainlooptime_med += mainlooptime;

#endif
}

void ResizeScene(int scr_width, int scr_height)
{
	glViewport(0, 0, scr_width, scr_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, scr_width, scr_height, 0.0);
	glMatrixMode(GL_MODELVIEW);
}

void Flip(const HDC hDC)
{
#ifdef _DEBUG_TIMINGS
	fliptime = GetTickCount() - fliptime_prv;
#endif

	glFlush();
	SwapBuffers(hDC);

#ifdef _DEBUG_TIMINGS

	fliptime_prv = GetTickCount();

	fliptime_med += fliptime;

#endif
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

void InitWindow(HINSTANCE hInstance)
{

	// TODO: make it search all available configs and ask what to load from or something
	if(!cfg.LoadFromFile("default.cfg")) LogToFile("debug.log", "Failed to load config");

	window_rect.left = 0;
	window_rect.right = cfg.scr_width;
	window_rect.top = 0;
	window_rect.bottom = cfg.scr_height;

	window_class.hInstance = hInstance;
	window_class.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hbrBackground = NULL;
	window_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = "OpenOrionClass";

	if(!RegisterClass(&window_class))
	{
		MessageBox(NULL, "Failed to register window class", "RegisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		LogToFile("debug.log", "Failed to register window class");
		PostQuitMessage(-1);
	}

	if(cfg.fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_OVERLAPPED;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPED;
	}

	AdjustWindowRectEx(&window_rect, dwStyle, false, dwExStyle);

	if(cfg.fullscreen)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    // Очистка для хранения установок
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // Размер структуры Devmode
		dmScreenSettings.dmPelsWidth = cfg.scr_width;        // Ширина экрана
		dmScreenSettings.dmPelsHeight = cfg.scr_height;        // Высота экрана
		dmScreenSettings.dmBitsPerPel = cfg.scr_bpp;        // Глубина цвета
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пикселя
		if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			cfg.fullscreen = false;
			LogToFile("debug.log","Failed to set fullscreen mode");
			MessageBox(NULL, "Failed to set fullscreen mode", "ChangeDisplaySettings() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
		}
		ShowCursor(false);
	}


	game_window = CreateWindowEx(dwExStyle,
			window_class.lpszClassName, GAMENAME,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
			dwStyle,
			GetSystemMetrics(0) - GetSystemMetrics(0)/2 - window_rect.right / 2,
			GetSystemMetrics(1) - GetSystemMetrics(1)/2 - window_rect.bottom / 2,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			NULL, NULL, hInstance, NULL);

	if(!game_window)
	{
		
			LogToFile("debug.log","Failed to create game window");
			MessageBox(NULL, "Failed to create game window", "CreateWindowEx() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
					
			
	}

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = cfg.scr_bpp;

	hDC = GetDC(game_window);

	if(!hDC)
	{
			LogToFile("debug.log","Failed to create device context");
			MessageBox(NULL, "Failed to create device context", "GetDC() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	pf = ChoosePixelFormat(hDC, &pfd);

	if(!pf)
	{
			LogToFile("debug.log","Failed to choose pixel format");
			MessageBox(NULL, "Failed to set pixel format", "ChoosePixelFormat() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	if(!SetPixelFormat(hDC, pf, &pfd))
	{
			LogToFile("debug.log","Failed to set pixel format");
			MessageBox(NULL, "Failed to set pixel format", "SetPixelFormat() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	hRC = wglCreateContext(hDC);

	if(!hRC)
	{
			LogToFile("debug.log","Failed to create rendering context");
			MessageBox(NULL, "Failed to create rendering context", "wglCreateContext() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
			LogToFile("debug.log","Failed to make current context");
			MessageBox(NULL, "Failed to make current context", "wglMakeCurrent() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	ShowWindow(game_window, SW_SHOW);
	SetForegroundWindow(game_window);
	SetFocus(game_window);
}

void KillWindow(HINSTANCE hInstance)
{
	if(cfg.fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}

	if(hRC)
	{
		if(!wglMakeCurrent(NULL, NULL))
		{
			LogToFile("debug.log","Failed to release current context");
			MessageBox(NULL, "Failed to release current context", "wglMakeCurrent() Error", MB_OK | MB_ICONEXCLAMATION);
		}

		if(!wglDeleteContext(hRC))
		{
			LogToFile("debug.log","Failed to delete rendering context");
			MessageBox(NULL, "Failed to delete rendering context", "wglDeleteContext() Error", MB_OK | MB_ICONEXCLAMATION);
		}

		hRC = NULL;
	}

	if(hDC && !ReleaseDC(game_window, hDC))
	{
		LogToFile("debug.log","Failed to release device context");
		MessageBox(NULL, "Failed to release device context", "ReleaseDC() Error", MB_OK | MB_ICONEXCLAMATION);
		hDC = NULL;
	}

	if(game_window && !DestroyWindow(game_window))
	{
		LogToFile("debug.log","Failed to destroy game window");
		MessageBox(NULL, "Failed to destroy game window", "DestroyWindow() Error", MB_OK | MB_ICONEXCLAMATION);
		game_window = NULL;
	}

	if(!UnregisterClass(window_class.lpszClassName, hInstance))
	{
		LogToFile("debug.log","Failed to unregister game window class");
		MessageBox(NULL, "Failed to unregister game window class", "UnregisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		hInstance = NULL;
	}
				
}



void InitGUI()
{
	gui = new GUI();

	// initting main menu
	gui->mainmenu = new GUIcontrol("MainMenu");
	gui->mainmenu->SetTexture(&textures[FindByName(textures, "mainscreen")]);
	gui->mainmenu->SetPos(0, 0);
	gui->mainmenu->SetPatternCount(1);
	gui->mainmenu->SetPatternWidth(gui->mainmenu->tex->width);
	gui->mainmenu->SetPatternHeight(gui->mainmenu->tex->height);
	gui->mainmenu->SetCurrentPattern(0);
	gui->mainmenu->SetVisible(true);
	gui->mainmenu->SetColor(255, 255, 255, 255);

	gui->AddControl(gui->mainmenu);

	GUIcontrol* startbutton = new GUIcontrol("StartButton");
	startbutton->SetTexture(&textures[FindByName(textures, "button_long")]);
	startbutton->SetPos(50, 150);
	startbutton->SetPatternCount(3);
	startbutton->SetPatternWidth(220);
	startbutton->SetPatternHeight(30);
	startbutton->SetCurrentPattern(0);
	startbutton->SetVisible(true);
	startbutton->SetColor(255, 255, 255, 255);
	startbutton->SetCaptionFont(&fonts[0]);
	startbutton->SetCaptionColor(255, 255, 255, 255);
	startbutton->SetCaptionPos(30, 7);
	startbutton->SetCaption("New game");
	GUIcontrol* loadbutton = new GUIcontrol("LoadButton");
	loadbutton->SetTexture(&textures[FindByName(textures, "button_long")]);
	loadbutton->SetPos(50, 190);
	loadbutton->SetPatternCount(3);
	loadbutton->SetPatternWidth(220);
	loadbutton->SetPatternHeight(30);
	loadbutton->SetCurrentPattern(0);
	loadbutton->SetVisible(true);
	loadbutton->SetColor(255, 255, 255, 255);
	loadbutton->SetCaptionFont(&fonts[0]);
	loadbutton->SetCaptionColor(255, 255, 255, 255);
	loadbutton->SetCaptionPos(30, 7);
	loadbutton->SetCaption("Load game");
	GUIcontrol* creditsbutton = new GUIcontrol("CreditsButton");
	creditsbutton->SetTexture(&textures[FindByName(textures, "button_long")]);
	creditsbutton->SetPos(50, 230);
	creditsbutton->SetPatternCount(3);
	creditsbutton->SetPatternWidth(220);
	creditsbutton->SetPatternHeight(30);
	creditsbutton->SetCurrentPattern(0);
	creditsbutton->SetVisible(true);
	creditsbutton->SetColor(255, 255, 255, 255);
	creditsbutton->SetCaptionFont(&fonts[0]);
	creditsbutton->SetCaptionColor(255, 255, 255, 255);
	creditsbutton->SetCaptionPos(30, 7);
	creditsbutton->SetCaption("Credits");
	GUIcontrol* exitbutton = new GUIcontrol("ExitButton");
	exitbutton->SetTexture(&textures[FindByName(textures, "button_long")]);
	exitbutton->SetPos(50, 270);
	exitbutton->SetPatternCount(3);
	exitbutton->SetPatternWidth(220);
	exitbutton->SetPatternHeight(30);
	exitbutton->SetCurrentPattern(0);
	exitbutton->SetVisible(true);
	exitbutton->SetColor(255, 255, 255, 255);
	exitbutton->SetCaptionFont(&fonts[0]);
	exitbutton->SetCaptionColor(255, 255, 255, 255);
	exitbutton->SetCaptionPos(30, 7);
	exitbutton->SetCaption("Exit");
	gui->mainmenu->AddChild(startbutton);
	gui->mainmenu->AddChild(loadbutton);
	gui->mainmenu->AddChild(creditsbutton);
	gui->mainmenu->AddChild(exitbutton);
	//gui->mainmenu->SetVisible(false);


	

	//gui = new GUI();
	//gui->ShowMessage("Hello player", "Open Orion", GUI_WARNING);
	//gui->ShowMessage("bugaga", "Test", GUI_NOTIFY);
}

void InitFonts()
{
	//LoadFontFromFile("tempesta.bmp", &fonts[0], 5, 5);
	LoadFont("Courier New", &fonts[0], 10, FW_NORMAL);
}



int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
	LogToFile(DEFAULT_LOG_NAME, "Log started");

	InitWindow(hInstance);
	startup_time = GetTickCount();
	previous_time = 0;
	frames = 0;
	fps = 1;

#ifdef _DEBUG_TIMINGS

	drawtime = 0;
	drawtime_prv = 0;
	drawtime_med = 1;
	cleartime = 0;
	cleartime_prv = 0;
	cleartime_med = 1;
	mainlooptime = 0;
	mainlooptime_prv = 0;
	mainlooptime_med = 1;
	fliptime = 0;
	fliptime_prv = 0;
	fliptime_med = 1;

#endif

	current_state = GAME_MAINMENU;
	mouseX = 0;
	mouseY = 0;
	mouse[0] = false;
	mouse[1] = false;
	mouse[2] = false;
	tex_count = 0;
	font_count = 0;

	InitPaths();
	LogPaths();

	
	LoadTexturesFromFolder(path_textures, textures);

	InitFonts();
	InitGUI();

	

	

	InitOGLSettings();
	ResizeScene(cfg.scr_width, cfg.scr_height);
		
	while(current_state != GAME_EXITING)
	{
		ClearScene();
		MainLoop();

		DrawScene();
		Flip(hDC);
	}

	// Now terminating all
	KillWindow(hInstance);

	LogToFile(DEFAULT_LOG_NAME, "Finished logging");




}
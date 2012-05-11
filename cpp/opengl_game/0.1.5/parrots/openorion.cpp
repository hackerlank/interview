#include "stdafx.h"

#include "config.h"

#include "misc.h"
#include "font.h"
#include "texture.h"
#include "draw.h"

#include "game.h"
#include "gui.h"

#include "openorion.h"

#include "..\oorion\oorion.h"

#pragma comment(lib, "oorion.lib")

extern const int menu_length;
extern const int main_length;
extern const int difficult_length;
extern const int size_length;
extern const int race_length;
extern const int opponents_length;

extern const LPSTR names_menu[];
extern const LPSTR names_main[];
extern const LPSTR names_difficulty[];
extern const LPSTR names_size[];
extern const LPSTR names_race[];
extern const LPSTR names_opponents[];



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
			mouseY = HIWORD(lParam);
			if(!fullscreenflag) mouseY += WINDOW_CAPTION_DY;
			return 0;
		}
	case WM_LBUTTONDOWN:
		{
			mouse[0] = true;
			return 0;
		}
	case WM_LBUTTONUP:
		{
			mouse[0] = false;
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
	TextOut("Time elapsed: " + string(time_elapsed), DEBUG_FONT, &hDC, 5, 230);
	TextOut("Mouse X: " + string(mx), DEBUG_FONT, &hDC, 5, 55);
	TextOut("Mouse Y: " + string(my), DEBUG_FONT, &hDC, 5, 70);
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
	TextOut("Average clear time: " + string(clear), DEBUG_FONT, &hDC, 5, 85);
	TextOut("Average mainloop time: " + string(mainloop), DEBUG_FONT, &hDC, 5, 100);
	TextOut("Average draw time: " + string(draw), DEBUG_FONT, &hDC, 5, 115);
	TextOut("Average flip time: " + string(flip), DEBUG_FONT, &hDC, 5, 130);
#endif
#ifdef _DEBUG_GUI
		char count[10];
		itoa(gui->queueCount, count, 10);

		TextOut("gui->queueCount: " + string(count), DEBUG_FONT, &hDC, 5, 145);
#endif

#ifdef PRINT_VERSION
	#ifdef VERSION
		#ifdef VERSION_X
			#ifdef VERSION_Y
				if(current_state == GAME_MAINMENU)
					TextOut(VERSION, DEBUG_FONT, &hDC, VERSION_X, VERSION_Y);
			#else
				if(current_state == GAME_MAINMENU)
					TextOut(VERSION, DEBUG_FONT, &hDC, VERSION_X, 25);
			#endif
		#else
			#ifdef VERSION_Y
				if(current_state == GAME_MAINMENU)
					TextOut(VERSION, DEBUG_FONT, &hDC, 5, VERSION_Y);
			#else
				if(current_state == GAME_MAINMENU)
					TextOut(VERSION, DEBUG_FONT, &hDC, 5, 25);
			#endif
		#endif		
	#endif
#endif

#ifdef _DEBUG_GAME
				if(game)
					game->DebugToScreen();
#endif


	char frames_per_second[10];
	_itoa(fps, frames_per_second, 10);

	TextOut("Frames per second: " + string(frames_per_second), DEBUG_FONT, &hDC, 5, 40);

#ifdef _DEBUG_PICKS
	char diff_pck[10];
	char size_pck[10];
	char race_pck[10];
	char opp_pck[10];

	_itoa(difficulty_pick, diff_pck,10);
	_itoa(size_pick, size_pck, 10);
	_itoa(race_pick, race_pck, 10);
	_itoa(opponents_pick, opp_pck, 10);

	TextOut("Difficulty pick: " + string(diff_pck), DEBUG_FONT, &hDC, 5, 170);
	TextOut("Size pick: " + string(size_pck), DEBUG_FONT, &hDC, 5, 185);
	TextOut("Race pick: " + string(race_pck), DEBUG_FONT, &hDC, 5, 200);
	TextOut("Opponents pick: " + string(opp_pck), DEBUG_FONT, &hDC, 5, 215);
#endif





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
	//glColor4ub(255, 255, 255, 255);
#ifdef _DEBUG_TIMINGS
	drawtime = GetTickCount() - drawtime_prv;
#endif
	
	if(gui) gui->DrawAll();
	if(game) game->Draw();

#ifdef _DEBUG_
	PrintDebug();
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
					LogToFile("Saving screenshot...");
					gui->LogOnScreen("Saving screenshot");
					if(!SaveScreenshot(cfg.scr_width, cfg.scr_height, cfg.scr_bpp))
						LogToFile("Failed save screenshot");
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

	fullscreenflag = cfg.fullscreen;

	if(!RegisterClass(&window_class))
	{
		MessageBox(NULL, "Failed to register window class", "RegisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		LogToFile("debug.log", "Failed to register window class");
		PostQuitMessage(-1);
	}

	if(cfg.fullscreen)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));    // Очистка для хранения установок
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // Размер структуры Devmode
		dmScreenSettings.dmPelsWidth = cfg.scr_width;        // Ширина экрана
		dmScreenSettings.dmPelsHeight = cfg.scr_height;        // Высота экрана
		dmScreenSettings.dmBitsPerPel = cfg.scr_bpp;        // Глубина цвета
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пикселя

		DWORD disp;

		disp = ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		switch(disp)
		{
		case DISP_CHANGE_SUCCESSFUL:
			{
				fullscreenflag = true;
				ShowCursor(true);
				break;
			}
		case DISP_CHANGE_BADDUALVIEW:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_BADDUALVIEW)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADDUALVIEW", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_BADFLAGS:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_BADFLAGS)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADFLAGS", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_BADMODE:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_BADMODE)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADMODE", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_BADPARAM:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_BADPARAM)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADPARAM", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_FAILED:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_FAILED)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_FAILED", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_NOTUPDATED:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_NOTUPDATED)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_NOTUPDATED", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_RESTART:
			{
				fullscreenflag = false;
				ShowCursor(true);
				LogToFile("Failed to set fullscreen mode: error (DISP_CHANGE_RESTART)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_RESTART", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		}

	}

	if(fullscreenflag)
	{
		dwExStyle = WS_EX_APPWINDOW;
		//dwStyle = WS_OVERLAPPED;
		dwStyle = WS_POPUP;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPED;
	}

	AdjustWindowRectEx(&window_rect, dwStyle, false, dwExStyle);


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
	if(fullscreenflag)
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
	//
	// Initting GUI
	//
	gui = new GUI();

	gui->mainmenu = new GUIcontrol("MainMenu");
	gui->difficultymenu = new GUIcontrol("DifficultyMenu");
	gui->sizemenu = new GUIcontrol("SizeMenu");
	gui->racemenu = new GUIcontrol("RaceMenu");
	gui->opponentsmenu = new GUIcontrol("OpponentsMenu");



	gui->AddControl(gui->mainmenu);
	gui->AddControl(gui->difficultymenu);
	gui->AddControl(gui->sizemenu);
	gui->AddControl(gui->racemenu);
	gui->AddControl(gui->opponentsmenu);

	for(std::vector<GUIcontrol*>::iterator it = gui->controls.begin();
		it != gui->controls.end();
		it++)
	{
		(*it)->SetTexture(&textures[FindByName(textures, "mainscreen")]);
		(*it)->SetPos(0, 0);
		(*it)->SetPatternCount(1);
		(*it)->SetPatternWidth(gui->mainmenu->tex->width);
		(*it)->SetPatternHeight(gui->mainmenu->tex->height);
		(*it)->SetCurrentPattern(0);
		(*it)->SetColor(255, 255, 255, 255);
		(*it)->Centrify();

	}


	//
	// Initting main menu
	//

	GUIcontrol* startbutton = new GUIcontrol("Start");
	GUIcontrol* loadbutton = new GUIcontrol("Load");
	GUIcontrol* optionsbutton = new GUIcontrol("Options");
	GUIcontrol* creditsbutton = new GUIcontrol("Credits");
	GUIcontrol* exitbutton = new GUIcontrol("Exit");

	gui->mainmenu->AddChild(startbutton);
	gui->mainmenu->AddChild(loadbutton);
	gui->mainmenu->AddChild(optionsbutton);
	gui->mainmenu->AddChild(creditsbutton);
	gui->mainmenu->AddChild(exitbutton);

	int i = -1;
	for(std::vector<GUIcontrol*>::iterator it = gui->mainmenu->children.begin();
		it != gui->mainmenu->children.end();
		it++)
	{
		i++;
		(*it)->SetTexture(&textures[FindByName(textures, "button_long")]);
		(*it)->SetPos(250, 150 + i * 40);
		(*it)->SetPatternCount(3);
		(*it)->SetPatternWidth(220);
		(*it)->SetPatternHeight(30);
		(*it)->SetCurrentPattern(0);
		(*it)->SetVisible(false);
		(*it)->SetColor(255, 255, 255, 255);


		(*it)->SetCaptionFont(&fonts[0]);
		(*it)->SetCaptionColor(255, 255, 255, 255);
		(*it)->SetCaptionPos(30, 7);
		(*it)->SetCaption((*it)->name);

		if((*it)->name == "Start")
		{
			(*it)->SetNext(gui->difficultymenu);
			(*it)->SetAction(actOpenNext);
		}
		if((*it)->name == "Exit")
		{
			(*it)->SetAction(actExit);
		}
	}

	//
	// Initting difficulty menu
	//

	GUIcontrol* easybutton = new GUIcontrol("Easy");
	GUIcontrol* mediumbutton = new GUIcontrol("Medium");
	GUIcontrol* hardbutton = new GUIcontrol("Hard");
	GUIcontrol* impossiblebutton = new GUIcontrol("Impossible");
	GUIcontrol* backdiff = new GUIcontrol("Back");

	gui->difficultymenu->AddChild(easybutton);
	gui->difficultymenu->AddChild(mediumbutton);
	gui->difficultymenu->AddChild(hardbutton);
	gui->difficultymenu->AddChild(impossiblebutton);
	gui->difficultymenu->AddChild(backdiff);

	i = -1;
	for(std::vector<GUIcontrol*>::iterator it = gui->difficultymenu->children.begin();
		it != gui->difficultymenu->children.end();
		it++)
	{
		i++;
		(*it)->SetTexture(&textures[FindByName(textures, "button_long")]);
		(*it)->SetPos(250, 150 + i * 40);
		(*it)->SetPatternCount(3);
		(*it)->SetPatternWidth(220);
		(*it)->SetPatternHeight(30);
		(*it)->SetCurrentPattern(0);
		(*it)->SetVisible(false);
		(*it)->SetColor(255, 255, 255, 255);

		(*it)->SetCaptionFont(&fonts[0]);
		(*it)->SetCaptionColor(255, 255, 255, 255);
		(*it)->SetCaptionPos(30, 7);
		(*it)->SetCaption((*it)->name);
		(*it)->SetNext(gui->sizemenu);
		(*it)->SetAction(actOpenNext);

		if((*it)->name == "Back")
		{
			(*it)->SetNext(gui->mainmenu);
			(*it)->SetAction(actOpenNext);
		}

	}

	//
	// Initting size menu
	//

	GUIcontrol* tinybutton = new GUIcontrol("Tiny");
	GUIcontrol* smallbutton = new GUIcontrol("Small");
	GUIcontrol* averagebutton = new GUIcontrol("Average");
	GUIcontrol* bigbutton = new GUIcontrol("Big");
	GUIcontrol* hugebutton = new GUIcontrol("Huge");
	GUIcontrol* backbutton2 = new GUIcontrol("Back");

	gui->sizemenu->AddChild(tinybutton);
	gui->sizemenu->AddChild(smallbutton);
	gui->sizemenu->AddChild(averagebutton);
	gui->sizemenu->AddChild(bigbutton);
	gui->sizemenu->AddChild(hugebutton);
	gui->sizemenu->AddChild(backbutton2);

	i = -1;
	for(std::vector<GUIcontrol*>::iterator it = gui->sizemenu->children.begin();
		it != gui->sizemenu->children.end();
		it++)
	{
		i++;
		(*it)->SetTexture(&textures[FindByName(textures, "button_long")]);
		(*it)->SetPos(250, 150 + i * 40);
		(*it)->SetPatternCount(3);
		(*it)->SetPatternWidth(220);
		(*it)->SetPatternHeight(30);
		(*it)->SetCurrentPattern(0);
		(*it)->SetVisible(false);
		(*it)->SetColor(255, 255, 255, 255);

		(*it)->SetCaptionFont(&fonts[0]);
		(*it)->SetCaptionColor(255, 255, 255, 255);
		(*it)->SetCaptionPos(30, 7);
		(*it)->SetCaption((*it)->name);
		(*it)->SetAction(actOpenNext);
		(*it)->SetNext(gui->racemenu);

		if((*it)->name == "Back")
		{
			(*it)->SetNext(gui->difficultymenu);
			(*it)->SetAction(actOpenNext);
		}
	}

	//
	// Initting race menu
	//

	GUIcontrol* alkari = new GUIcontrol("Alkari");
	GUIcontrol* bulrathi = new GUIcontrol("Bulrathi");
	GUIcontrol* klackon = new GUIcontrol("Klackon");
	GUIcontrol* sakkra = new GUIcontrol("Meklar");
	GUIcontrol* psilon = new GUIcontrol("Psilon");
	GUIcontrol* mrrshan = new GUIcontrol("Mrrshan");
	GUIcontrol* meklar = new GUIcontrol("Darlok");
	GUIcontrol* human = new GUIcontrol("Human");
	GUIcontrol* darlok = new GUIcontrol("Sakkra");
	GUIcontrol* silicoid = new GUIcontrol("Silicoid");
	GUIcontrol* backrace = new GUIcontrol("Back");

	gui->racemenu->AddChild(alkari);
	gui->racemenu->AddChild(bulrathi);
	gui->racemenu->AddChild(klackon);
	gui->racemenu->AddChild(sakkra);
	gui->racemenu->AddChild(psilon);
	gui->racemenu->AddChild(mrrshan);
	gui->racemenu->AddChild(meklar);
	gui->racemenu->AddChild(human);
	gui->racemenu->AddChild(darlok);
	gui->racemenu->AddChild(silicoid);
	gui->racemenu->AddChild(backrace);

	i = -1;
	for(std::vector<GUIcontrol*>::iterator it = gui->racemenu->children.begin();
		it != gui->racemenu->children.end();
		it++)
	{
		i++;
		(*it)->SetTexture(&textures[FindByName(textures, "button_long")]);
		(*it)->SetPos(250, 150 + i * 40);
		(*it)->SetPatternCount(3);
		(*it)->SetPatternWidth(220);
		(*it)->SetPatternHeight(30);
		(*it)->SetCurrentPattern(0);
		(*it)->SetVisible(false);
		(*it)->SetColor(255, 255, 255, 255);

		(*it)->SetCaptionFont(&fonts[0]);
		(*it)->SetCaptionColor(255, 255, 255, 255);
		(*it)->SetCaptionPos(30, 7);
		(*it)->SetCaption((*it)->name);
		(*it)->SetAction(actOpenNext);
		(*it)->SetNext(gui->opponentsmenu);

		if((*it)->name == "Back")
		{
			(*it)->SetNext(gui->sizemenu);
			(*it)->SetAction(actOpenNext);
		}
	}

	//
	// Initting opponents menu
	//

	GUIcontrol* opponents1 = new GUIcontrol("One opponent");
	GUIcontrol* opponents2 = new GUIcontrol("Two opponents");
	GUIcontrol* opponents3 = new GUIcontrol("Three opponents");
	GUIcontrol* opponents4 = new GUIcontrol("Four opponents");
	GUIcontrol* opponents5 = new GUIcontrol("Five opponents");
	//GUIcontrol* opponents6 = new GUIcontrol("Six opponents");
	//GUIcontrol* opponents7 = new GUIcontrol("Seven opponents");
	//GUIcontrol* opponents8 = new GUIcontrol("Eight opponents");
	//GUIcontrol* opponents9 = new GUIcontrol("Nine opponents");
	GUIcontrol* backopp = new GUIcontrol("Back");

	gui->opponentsmenu->AddChild(opponents1);
	gui->opponentsmenu->AddChild(opponents2);
	gui->opponentsmenu->AddChild(opponents3);
	gui->opponentsmenu->AddChild(opponents4);
	gui->opponentsmenu->AddChild(opponents5);
	gui->opponentsmenu->AddChild(backopp);
	//gui->racemenu->AddChild(opponents1);
	//gui->racemenu->AddChild(opponents1);
	//gui->racemenu->AddChild(opponents1);
	//gui->racemenu->AddChild(opponents1);
	//gui->racemenu->AddChild(opponents1);

	i = -1;
	for(std::vector<GUIcontrol*>::iterator it = gui->opponentsmenu->children.begin();
		it != gui->opponentsmenu->children.end();
		it++)
	{
		i++;
		(*it)->SetTexture(&textures[FindByName(textures, "button_long")]);
		(*it)->SetPos(250, 150 + i * 40);
		(*it)->SetPatternCount(3);
		(*it)->SetPatternWidth(220);
		(*it)->SetPatternHeight(30);
		(*it)->SetCurrentPattern(0);
		(*it)->SetVisible(false);
		(*it)->SetColor(255, 255, 255, 255);

		(*it)->SetCaptionFont(&fonts[0]);
		(*it)->SetCaptionColor(255, 255, 255, 255);
		(*it)->SetCaptionPos(30, 7);
		(*it)->SetCaption((*it)->name);
		//(*it)->SetAction(actOpenNext);
		//(*it)->SetNext(gui->opponentsmenu);
		(*it)->SetAction(actStart);

		if((*it)->name == "Back")
		{
			(*it)->SetNext(gui->racemenu);
			(*it)->SetAction(actOpenNext);
		}
	}


}

void InitFonts()
{
	LoadFont("Courier New", &fonts[0], 10, FW_NORMAL);
	LoadFont("Courier New", &fonts[1], 14, FW_NORMAL);
	LoadFont("Times New Roman", &fonts[2], 14, FW_BOLD);
	LoadFont("Courier New", &fonts[3], 10, FW_NORMAL);
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
	difficulty_pick = 0;
	size_pick = 0;
	race_pick = 0;
	opponents_pick = 0;
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

	srand((unsigned)time(NULL));

	gui->ViewMenu(0);

	InitOGLSettings();
	ResizeScene(cfg.scr_width, cfg.scr_height);
		
	while(current_state != GAME_EXITING)
	{
		MainLoop();
		ClearScene();
		DrawScene();
		Flip(hDC);
	}

	// Now terminating all
	KillWindow(hInstance);

	LogToFile(DEFAULT_LOG_NAME, "Finished logging");




}
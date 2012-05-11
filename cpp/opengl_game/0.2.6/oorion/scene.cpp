#include "stdafx.h"
#include "font.h"
#include "texture.h"
#include "gui.h"
#include "game.h"

#include "draw.h"
#include "misc.h"
#include "config.h"

#include "scene.h"

	bool keys[256];
	int mouseX = 0;
	int mouseY = 0;
	bool mouse[3];

	HGLRC hRC;
	HDC hDC;
	HWND game_window;
	WNDCLASS window_class;
	RECT window_rect;
	DEVMODE dmScreenSettings;
	int dwExStyle;
	int dwStyle;
	bool fullscreenflag;
	PIXELFORMATDESCRIPTOR pfd;
	int pf;

	unsigned int startup_time;
	unsigned int current_time;
	unsigned int previous_time;
	unsigned int frames = 0;
	unsigned int fps = 1;
	unsigned int lasttick = 0;

	extern GUI* gui;
	extern GAME* game;
	extern config cfg;

	extern unsigned int current_state;

	extern unsigned int fps;
	extern int race_pick;
	extern int size_pick;
	extern int difficulty_pick;
	extern int opponents_pick;

	extern unsigned int frames;




#ifdef _DEBUG_TIMINGS
	extern int drawtime;
	extern int drawtime_prv;
	extern int drawtime_med;
	extern int drawtime_med_dr;
	extern int cleartime;
	extern int cleartime_prv;
	extern int cleartime_med;
	extern int cleartime_med_dr;
	extern int mainlooptime;
	extern int mainlooptime_prv;
	extern int mainlooptime_med;
	extern int mainlooptime_med_dr;
	extern int fliptime;
	extern int fliptime_prv;
	extern int fliptime_med;
	extern int fliptime_med_dr;
#endif





long __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
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
	case WM_RBUTTONDOWN:
		{
			mouse[1] = true;
			return 0;
		}
	case WM_RBUTTONUP:
		{
			mouse[1] = false;
			return 0;
		}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
	
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


void PrintDebug()
{
	glColor4ub(0, 255, 0, 255);
#ifdef DRAW_TIME_ELAPSED
	char time_elapsed[32];
	sprintf(time_elapsed, "Time elapsed: %u", current_time / 1000);
	PrintText(time_elapsed, FontByName(DEBUG_FONT), 5, 230);
#endif
#ifdef _DEBUG_MOUSE
	char mx[4];
	sprintf(mx, "Mouse X: %u", mouseX);
	PrintText(mx, FontByName(DEBUG_FONT), 5, 55);
	char my[4];
	sprintf(my, "Mouse Y: %u", mouseY);
	PrintText(my, FontByName(DEBUG_FONT), 5, 70);
#endif

#ifdef _DEBUG_TIMINGS
	char draw[32];
	char flip[32];
	char clear[32];
	char mainloop[32];
	sprintf(draw, "Average draw time: %u", drawtime_med_dr);
	sprintf(flip, "Average flip time: %u", fliptime_med_dr);
	sprintf(clear, "Average clear time: %u", cleartime_med_dr);
	sprintf(mainloop, "Average mainloop time: %u", mainlooptime_med_dr);

	PrintText(clear, FontByName(DEBUG_FONT), 5, 85);
	PrintText(mainloop, FontByName(DEBUG_FONT), 5, 100);
	PrintText(draw, FontByName(DEBUG_FONT), 5, 115);
	PrintText(flip, FontByName(DEBUG_FONT), 5, 130);
#endif
#ifdef _DEBUG_GUI

	if(gui)
	{
		if(gui->GetMessages())
		{
			char queue[32];
			sprintf(queue, "Messages in msg_allocator: %u", gui->GetMessages()->GetMessageCount());

			PrintText(queue, FontByName(DEBUG_FONT), 5, 75);
		}
	}
#endif

#ifdef PRINT_VERSION
	#ifdef VERSION
		#ifdef VERSION_X
			#ifdef VERSION_Y
				if(current_state == GAME_MAINMENU)
					PrintText(VERSION, FontByName(DEBUG_FONT), VERSION_X, VERSION_Y);
			#else
				if(current_state == GAME_MAINMENU)
					PrintText(VERSION, FontByName(DEBUG_FONT), VERSION_X, 25);
			#endif
		#else
			#ifdef VERSION_Y
				if(current_state == GAME_MAINMENU)
					PrintText(VERSION, FontByName(DEBUG_FONT), 5, VERSION_Y);
			#else
				if(current_state == GAME_MAINMENU)
					PrintText(VERSION, FontByName(DEBUG_FONT), 5, 25);
			#endif
		#endif		
	#endif
#endif

#ifdef _DEBUG_GAME
				if(game)
					game->DebugToScreen();
#endif
#ifdef _DEBUG_FPS
	char frames_per_second[32];
	sprintf(frames_per_second, "Frames per second: %u", fps);
	PrintText(frames_per_second, FontByName(DEBUG_FONT), 5, 40);
#endif

#ifdef _DEBUG_PICKS
	char diff_pck[32];
	char size_pck[32];
	char race_pck[32];
	char opp_pck[32];

	sprintf(diff_pck, "Difficulty pick: %u", difficulty_pick);
	sprintf(size_pck, "Size pick: %u", size_pick);
	sprintf(race_pck, "Race pick: %u", race_pick);
	sprintf(opp_pck, "Opponents pick: %u", opponents_pick);

	PrintText(diff_pck, FontByName(DEBUG_FONT), 5, 170);
	PrintText(size_pck, FontByName(DEBUG_FONT), 5, 185);
	PrintText(race_pck, FontByName(DEBUG_FONT), 5, 200);
	PrintText(opp_pck, FontByName(DEBUG_FONT), 5, 215);
#endif
}


void DrawScene()
{
#ifdef _DEBUG_TIMINGS
	drawtime = GetTickCount() - drawtime_prv;
#endif
	
	if(gui)
		gui->Draw();
	if(game)
		game->Draw();
	if(cfg.debug)
		PrintDebug();

	frames++;

#ifdef _DEBUG_TIMINGS

	drawtime_prv = GetTickCount();

	drawtime_med += drawtime;

#endif
}


void ApplyConfig(config* cfg)
{
	ResizeScene(cfg->scr_width, cfg->scr_height);
}


void ResizeScene(int scr_width, int scr_height)
{
	RECT new_rect;
	new_rect.left = 0;
	new_rect.top = 0;
	new_rect.right = scr_width;
	new_rect.bottom = scr_height;

	AdjustWindowRectEx(&new_rect, dwStyle, false, dwExStyle); 

	SetWindowPos(game_window,
		HWND_TOP,
		GetSystemMetrics(0) - GetSystemMetrics(0)/2 - new_rect.right / 2,
		GetSystemMetrics(1) - GetSystemMetrics(1)/2 - new_rect.bottom / 2,
		new_rect.right - window_rect.left,
		new_rect.bottom - window_rect.top,
		SWP_SHOWWINDOW);

	glViewport(0, 0, scr_width, scr_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, scr_width, scr_height, 0.0);
	glMatrixMode(GL_MODELVIEW);

	window_rect = new_rect;
}




void Flip()
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


void InitWindow(HINSTANCE hInstance)
{
	window_rect.left = 0;
	window_rect.right = DEFAULT_WINDOW_WIDTH;
	window_rect.top = 0;
	window_rect.bottom = DEFAULT_WINDOW_HEIGHT;

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
		
			LogToFile("Failed to create game window");
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
			LogToFile("Failed to create device context");
			MessageBox(NULL, "Failed to create device context", "GetDC() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	pf = ChoosePixelFormat(hDC, &pfd);

	if(!pf)
	{
			LogToFile("Failed to choose pixel format");
			MessageBox(NULL, "Failed to set pixel format", "ChoosePixelFormat() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	if(!SetPixelFormat(hDC, pf, &pfd))
	{
			LogToFile("Failed to set pixel format");
			MessageBox(NULL, "Failed to set pixel format", "SetPixelFormat() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	hRC = wglCreateContext(hDC);

	if(!hRC)
	{
			LogToFile("Failed to create rendering context");
			MessageBox(NULL, "Failed to create rendering context", "wglCreateContext() Error", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
			LogToFile("Failed to make current context");
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
			LogToFile("Failed to release current context");
			MessageBox(NULL, "Failed to release current context", "wglMakeCurrent() Error", MB_OK | MB_ICONEXCLAMATION);
		}

		if(!wglDeleteContext(hRC))
		{
			LogToFile("Failed to delete rendering context");
			MessageBox(NULL, "Failed to delete rendering context", "wglDeleteContext() Error", MB_OK | MB_ICONEXCLAMATION);
		}

		hRC = NULL;
	}

	if(hDC && !ReleaseDC(game_window, hDC))
	{
		LogToFile("Failed to release device context");
		MessageBox(NULL, "Failed to release device context", "ReleaseDC() Error", MB_OK | MB_ICONEXCLAMATION);
		hDC = NULL;
	}

	if(game_window && !DestroyWindow(game_window))
	{
		LogToFile("Failed to destroy game window");
		MessageBox(NULL, "Failed to destroy game window", "DestroyWindow() Error", MB_OK | MB_ICONEXCLAMATION);
		game_window = NULL;
	}

	if(!UnregisterClass(window_class.lpszClassName, hInstance))
	{
		LogToFile("Failed to unregister game window class");
		MessageBox(NULL, "Failed to unregister game window class", "UnregisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		hInstance = NULL;
	}			
}
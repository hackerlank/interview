#include "stdafx.h"

#define OORION_EXPORTS


#include "config.h"
#include "game.h"
#include "misc.h"
#include "font.h"
#include "texture.h"
#include "draw.h"
#include "gui.h"

#include "openorion.h"

#include "..\oorion\oorion.h";


#pragma comment(lib, "oorion.lib")

#define SECOND_ELAPSED current_time - previous_time >= 1000
#define GAMENAME "OpenOrion"

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

void PrintDebug()
{
	char time_elapsed[10];
	char frames_per_second[10];
	char mx[6];
	char my[6];

	_itoa(current_time / 1000 , time_elapsed , 10);
	_itoa(fps, frames_per_second, 10);
	_itoa(mouseX, mx, 10);
	_itoa(mouseY, my, 10);


	TextOut("Time elapsed: " + string(time_elapsed), &fonts[0], &hDC, 5, 25, 0, 255, 0, 255);

	TextOut("Frames per second: " + string(frames_per_second), &fonts[0], &hDC, 5, 40, 0, 255, 0, 255);

	TextOut("Mouse X: " + string(mx), &fonts[0], &hDC, 5, 55, 0, 255, 0, 255);
	TextOut("Mouse Y: " + string(my), &fonts[0], &hDC, 5, 70, 0, 255, 0, 255);
}


void ClearScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
}

void DrawScene()
{
	frames++;

	gui->Draw();

	//TextOutD("lalala", &fonts[0], &hDC, 70, 300, 0, 255, 0, 255, 1);
	//CircleAnim(300,300, 0, 255, 0, 255, 30, 1, true);
	//Box(400,250,30,40,0,0,255,255);

#ifdef _DEBUG
	PrintDebug();
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
	glFlush();
	SwapBuffers(hDC);
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

void CountFPS()
{
	if(previous_time == 0 || SECOND_ELAPSED)
	{
		previous_time = current_time;
		fps = frames;
		frames = 0;
	}

}

void MainLoop()
{
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
	//if (SECOND_ELAPSED) scr->Tick();
	CountFPS();
	gui->Tick();
		
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
	gui->ShowMessage("Hello player", "Open Orion", GUI_WARNING);

}

void InitFonts()
{
	LoadFont("Courier New", &fonts[0], 12, FW_NORMAL);
	LoadFont("Arial", &fonts[1], 10, FW_NORMAL);
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
	current_state = GAME_RUNNING;
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
		MainLoop();
		ClearScene();
		DrawScene();

		Flip(hDC);
	}

	// Now terminating all
	KillWindow(hInstance);

	LogToFile(DEFAULT_LOG_NAME, "Finished logging");




}
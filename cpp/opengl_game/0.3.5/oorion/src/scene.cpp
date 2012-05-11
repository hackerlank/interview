#include "stdafx.h"
#include "draw/font.h"
#include "gui/gui.h"
#include "iichan/iichan.h"
#include "draw/draw.h"
#include "misc.h"
#include "config.h"
#ifdef _DEBUG_GUI
	#include "gui_misc.h"
	#include "gui_msgalloc.h"
#endif
#include "scene.h"
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
extern GUI* gui;
extern IICHAN* iichan;
extern unsigned int current_state;
extern config cfg;
extern HINSTANCE hInst;
extern unsigned int fps;
extern unsigned int frames;
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
int mouseX = 0;
int mouseY = 0;
bool mouse[3] = { false, false, false };
HGLRC hRC = NULL;
HDC hDC = NULL;
HWND game_window = NULL;
WNDCLASS window_class;
RECT window_rect;
DEVMODE dmScreenSettings;
int dwStyle = 0;
bool fullscreenflag = false;
PIXELFORMATDESCRIPTOR pfd;
int pf = 0;
bool active = true;
// --------------------------------------------------------------------------



long __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
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
	case WM_ACTIVATE:
		{
			switch(wParam)
			{
			case WA_ACTIVE:
			case WA_CLICKACTIVE: active = true; break;
			case WA_INACTIVE: active = false; break;
			}
			return 0;
		}
	case WM_DESTROY:
	case WM_QUIT:
		hDC = NULL;
		game_window = NULL;
		current_state = GAME_EXITING;
		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}



void ClearScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void PrintFPS()
{
	static const font_t* font = FontByName(DEBUG_FONT);
	glColor4ub(0, 255, 0, 255);
	char frames_per_second[16];
	sprintf(frames_per_second, "FPS: %u\0", fps);
	PrintText(frames_per_second, font, 5, 40);
}


void PrintDebug()
{
	static const font_t* font = FontByName(DEBUG_FONT);
	glColor4ub(0, 255, 0, 255);
#ifdef DRAW_TIME_ELAPSED
	char time_elapsed[32];
	sprintf(time_elapsed, "Time elapsed: %u", current_time / 1000);
	PrintText(time_elapsed, font, 5, 230);
#endif
#ifdef _DEBUG_MOUSE
	char mx[4];
	sprintf(mx, "Mouse X: %u", mouseX);
	PrintText(mx, font, 5, 55);
	char my[4];
	sprintf(my, "Mouse Y: %u", mouseY);
	PrintText(my, font, 5, 70);
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
					PrintText(VERSION, font, VERSION_X, VERSION_Y);
			#else
				if(current_state == GAME_MAINMENU)
					PrintText(VERSION, font, VERSION_X, 25);
			#endif
		#else
			#ifdef VERSION_Y
				if(current_state == GAME_MAINMENU)
					PrintText(VERSION, font, 5, VERSION_Y);
			#else
					PrintText(ENGINE, font, 5, 10);
					PrintText(VERSION, font, 5, 25);				
			#endif
		#endif		
	#endif
#endif

#ifdef _DEBUG_GAME
				if(game)
					game->DebugToScreen();
#endif

#ifdef _DEBUG_KEYBOARD
	int i = 0;
	PrintText("Keyboard: ", font, 5, 230);
	for(vector<UCHAR>::iterator it = pressed_keys.begin();
		it != pressed_keys.end();
		it++)
	{
		i++;
		char text = (*it);
		PrintTextSafe(&text, 1, font, 65+i*15, 230);
	}
#endif
}


void DrawScene()
{
	if(iichan)
		iichan->Draw();

	if(gui)
		gui->Draw();

	if(cfg.debug)
		PrintDebug();

	if(cfg.show_fps)
		PrintFPS();

	frames++;
}

static void InitOGLSettings()
{
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glReadBuffer(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void ApplyConfig(config* cfg)
{
	ResizeScene(cfg->window_width,
		cfg->window_height,
		cfg->scr_width,
		cfg->scr_height,
		cfg->fullscreen);
}



void ResizeScene(UINT window_width, UINT window_height,
				 UINT scr_width, UINT scr_height, bool fullscreen)
{
	RECT new_rect;
	new_rect.left = 0;
	new_rect.top = 0;
	new_rect.right = window_width;
	new_rect.bottom = window_height;

	if(fullscreen)
	{
		dwStyle = WS_POPUP;
	}
	else
	{
		dwStyle = WS_CAPTION | WS_BORDER;
	}

	AdjustWindowRect(&new_rect, dwStyle, false);
	if(!fullscreen)
	{
		SetWindowPos(game_window,
			HWND_TOP,
			GetSystemMetrics(0) - GetSystemMetrics(0)/2 - new_rect.right / 2,
			GetSystemMetrics(1) - GetSystemMetrics(1)/2 - new_rect.bottom / 2,
			new_rect.right - (new_rect.left),
			new_rect.bottom - (new_rect.top),
			SWP_SHOWWINDOW);

		InitOGLSettings();

		glViewport(0, 0,
			window_width,
			window_height);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluOrtho2D(0.0,
				scr_width,
				scr_height,
				0.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
	}
	else
	{
		KillWindow(hInst);
		InitWindow(hInst, true);

		SetWindowPos(game_window,
			HWND_TOP,
			0, 0,
			window_width,
			window_height,
			SWP_SHOWWINDOW);

			memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
			dmScreenSettings.dmSize = sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth = scr_width;
			dmScreenSettings.dmPelsHeight = scr_height;
			dmScreenSettings.dmBitsPerPel = cfg.scr_bpp;
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			
			InitOGLSettings();
			
			glViewport(0, 0,
				window_width,
				window_height);

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				gluOrtho2D(0.0,
					window_width,
					window_height,
					0.0);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				DWORD disp;

			//disp = ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
				disp = DISP_CHANGE_SUCCESSFUL;
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
		fullscreenflag = fullscreen;
}




void Flip()
{
	glFlush();
	SwapBuffers(hDC);
}


void InitWindow(HINSTANCE hInstance, bool fullscreen)
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

	//dwStyle = WS_CAPTION | WS_BORDER;

	//if(fullscreen)
	//	dwStyle = WS_POPUP;

	if(!RegisterClass(&window_class))
	{
		MessageBox(NULL, "Failed to register window class", "RegisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		LogToFile("debug.log", "Failed to register window class");
		PostQuitMessage(-1);
	}

	game_window = CreateWindow(window_class.lpszClassName,
		GAMENAME,
		dwStyle,
		GetSystemMetrics(0) - GetSystemMetrics(0)/2 - window_rect.right / 2,
		GetSystemMetrics(1) - GetSystemMetrics(1)/2 - window_rect.bottom / 2,
		window_rect.right - window_rect.left,
		window_rect.bottom - window_rect.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(!game_window)
	{
			LogToFile("Failed to create game window");
			MessageBox(NULL, "Failed to create game window", "CreateWindowEx() Error", MB_OK |									MB_ICONEXCLAMATION);
			PostQuitMessage(-1);						
	}

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
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
			MessageBox(NULL, "Failed to set pixel format", "ChoosePixelFormat() Error", MB_OK |									MB_ICONEXCLAMATION);
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
			MessageBox(NULL, "Failed to create rendering context", "wglCreateContext() Error", MB_OK |							MB_ICONEXCLAMATION);
			PostQuitMessage(-1);
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
			LogToFile("Failed to make current context");
			MessageBox(NULL, "Failed to make current context", "wglMakeCurrent() Error", MB_OK |								MB_ICONEXCLAMATION);
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

	if(hDC && ReleaseDC(game_window, hDC) != 1)
	{
		LogToFile("Failed to release device context");
		MessageBox(NULL, "Failed to release device context", "ReleaseDC() Error", MB_OK | MB_ICONEXCLAMATION);
		hDC = NULL;
	}

	if(game_window && DestroyWindow(game_window) == 0)
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
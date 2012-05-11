#include "StdAfx.h"

#include "config.h"
#include "scene.h"

#include "main.h"
#include "misc.h"

#include "script/script.h"
#include "script/timerevent.h"

#include "game/game.h"

#include "game/camera.h"

#include "render/font.h"
#include "render/draw.h"
#include "render/texture.h"
#include "render/renderer.h"

//////////////////////////////////////////////////////////////////////////

#define FPSTICK 1000

//////////////////////////////////////////////////////////////////////////
// Либы для OpenGL
#pragma comment(lib, "OpenGl32.lib")		
#pragma comment(lib, "Glu32.lib")

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Подсчет времени и FPS
UINT fps = 0;			// Количество отрисованых в прошлую секунду кадров 
UINT frames = 0;		// Количество отрисованых с начала этой секунды кадров 
DWORD startup_time = 0;	// Начальное время (прмерно время запуска приложения)
DWORD last_time = 0;
DWORD current_time = 0;	// Текущее время

DWORD internal_time = 0;	// Внутреннее время движка. Считается только когда окно активно.

DWORD last_fps_tick = 0;		// Время предыдущего тика фпс
DWORD last_timerevent_tick = 0;	// Время предыдущего тика таймера событий
DWORD last_game_tick = 0;		// Время предыдущего тика игры
DWORD last_keyb_tick = 0;		// Время предыдущей реакции на зажатую клавишу

///
// Переменные окна
extern HINSTANCE hInst;

HGLRC hRC = NULL;
HDC hDC = NULL;
HWND game_window = NULL;
WNDCLASS window_class;
RECT window_rect;
DEVMODE dmScreenSettings;
int dwStyle = 0;
BOOL fullscreenflag = FALSE;
PIXELFORMATDESCRIPTOR pfd;
int pf = 0;
///

UINT mouseX = 0;		// Координаты курсора
UINT mouseY = 0;		// Координаты курсора
bool mouse[3] = { false, false, false };		// Состояние кнопок мыши
bool keys[256];			// Массив нажатых клавиш

ProgrammStates current_state = PROGRAMM_RUNNING;				// Текущее состояние игры




config cfg;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Обработчик сообщений
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
			if (current_state != PROGRAMM_EXITING)
			{
				switch(wParam)
				{
				case WA_ACTIVE:
				case WA_CLICKACTIVE: current_state = PROGRAMM_RUNNING; break;
				case WA_INACTIVE: current_state = PROGRAMM_INACTIVE; break;
				}
			}
			return 0;
		}
	case WM_DESTROY:
	case WM_QUIT:
		hDC = NULL;
		game_window = NULL;
		current_state = PROGRAMM_EXITING;
		return (long)DefWindowProc(hWnd, uMsg, wParam, lParam);
	default:
		return (long)DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return (long)DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Обработка клавитуры
void scene::ProcessKeyboard()
{
	//static unsigned int _prev_time;
	static unsigned char _prev_key;


	memset(keys, false, 256);
	for(UCHAR i = 0; i < 255; i++)
	{
		if(KEYDOWN(i))
		{
			if(i == _prev_key)
			{
				if(internal_time - last_keyb_tick >= 5)
				{
					keys[i] = true;
				}
			}
			else
				keys[i] = true;

			last_keyb_tick = internal_time;
			_prev_key = i;
		}
	}

	//if (keys[VK_ESCAPE])
	//{
	//	current_state = PROGRAMM_EXITING;
	//}
	
	if(keys[VK_F5])
	{
		cfg.debug = !cfg.debug;
	}
	/*if(keys[cfg.screenshot])
	{
		LogToFile("Saving screenshot...");
		if(gui)
		{
			char* msg = new char[32];
			sprintf(msg, "Saving screenshot...");
			gui->LogOnScreen(msg);
		}
		if(!scene::SaveScreenshot(cfg.window_width, cfg.window_height, cfg.scr_bpp))
			LogToFile("Failed to save screenshot");
	}*/
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Подсчет FPS
__INLINE void CountFPS()
{
	if(internal_time - last_fps_tick >= FPSTICK)
	{
		fps = frames;
		frames = 0;
		last_fps_tick = internal_time;
	}
}

// Вывод FPS на экран
// TODO: Когда появится интерфейс, перевети вывод текста на него.
__INLINE void PrintFPS()
{
	extern float CAMERA_OFF_X;
	extern float CAMERA_OFF_Y;


	static const Font* font = FontByName(DEFAULT_FONT);
	glColor4ub(0, 255, 0, 255);
	char frames_buf[16];
	sprintf(frames_buf, "FPS: %u\0", fps);
	DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 0.0f - CAMERA_OFF_Y);
}

#ifdef DEBUG_PRINT

float DBG_PRINT_VAL1 = 0;
float DBG_PRINT_VAL2 = 0;
//float DBG_PRINT_VAL3 = 0;
//float DBG_PRINT_VAL4 = 0;
//float DBG_PRINT_VAL5 = 0;
//float DBG_PRINT_VAL6 = 0;
//float DBG_PRINT_VAL7 = 0;
//float DBG_PRINT_VAL8 = 0;
//float DBG_PRINT_VAL9 = 0;

__INLINE void DbgPrint()
{
	static const Font* font = FontByName(DEFAULT_FONT);
	glColor4ub(0, 255, 0, 255);
	char frames_buf[128];

	extern float CAMERA_OFF_X;
	extern float CAMERA_OFF_Y;
	//extern float CAMERA_X;
	//extern float CAMERA_Y;
	
	//sprintf(frames_buf, "CAMX: %f\0", CAMERA_X);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 10.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "CAMY: %f\0", CAMERA_Y);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 20.0f - CAMERA_OFF_Y);

	sprintf(frames_buf, "VAL1: %f\0", DBG_PRINT_VAL1);
	DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 30.0f - CAMERA_OFF_Y);

	DBG_PRINT_VAL1 = 0;
	sprintf(frames_buf, "VAL2: %f\0", DBG_PRINT_VAL2);
	DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 40.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL3: %f\0", DBG_PRINT_VAL3);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 50.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL4: %f\0", DBG_PRINT_VAL4);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 60.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL5: %f\0", DBG_PRINT_VAL5);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 70.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL6: %f\0", DBG_PRINT_VAL6);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 80.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL7: %f\0", DBG_PRINT_VAL7);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 90.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL8: %f\0", DBG_PRINT_VAL8);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 100.0f - CAMERA_OFF_Y);

	//sprintf(frames_buf, "VAL9: %f\0", DBG_PRINT_VAL9);
	//DrawText(frames_buf, strlen(frames_buf), font, 5.0f - CAMERA_OFF_X, 110.0f - CAMERA_OFF_Y);

}

#endif // DEBUG_PRINT

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Игровой цикл
CFont font;

// Главный цикл
void scene::MainLoop()
{
	font.filename = "fonts/font.fif";
	font.LoadFromFile();

	do 
	{
		MSG msg;
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}


		if(current_state == PROGRAMM_RUNNING)
		{
			scene::ClearScene();
			scene::UpdateScene();			
			CountFPS();
			scene::DrawScene();
			scene::Flip();
		}
		else
		{
			if(current_state != PROGRAMM_EXITING)
			{
				DWORD t = GetTickCount();
				WaitMessage();
				// Определяем время, которое провели в неактивном состоянии
				t = GetTickCount() - t;
				// Так все таймеры это время неактивности не заметят.
				last_time += t;

			}
		}
	} while(current_state != PROGRAMM_EXITING);
}

// Обновление сцены, обработка все логики и т.п.
void scene::UpdateScene()
{
	current_time = (GetTickCount() - startup_time);
	internal_time += current_time - last_time;
	last_time = current_time;

	ProcessKeyboard();
	game::HandleKeyboard();

	if (internal_time - last_timerevent_tick >= TIMEREVENTTICK)
	{
		ProcessTimerEvents();
		last_timerevent_tick = internal_time;
	}

	if (internal_time - last_game_tick >= GAMETICK)
	{
		game::UpdateGame();
		last_game_tick = internal_time;
	}


/*
	if(TERMINATE_IICHAN)
	{
		delete iichan;
		iichan = NULL;
	}

	if(iichan)
		iichan->Move();
	if(gui)
		gui->Process();

	CountFPS();	
	*/
}

// Очистка сцены
void scene::ClearScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	r_ZeroRenderData();

#ifdef _DEBUG
	//RENDERED_TEXTURES = 0;
#endif
}

// Отрисовка всего
void scene::DrawScene()
{
	/*glColor4ub(255 - GLOBAL_COLOR_RED,
		255 - GLOBAL_COLOR_GREEN,
		255 - GLOBAL_COLOR_BLUE,
		255 - GLOBAL_COLOR_ALPHA);*/
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	font.p = Vector2(100, 100);
	font.Print("Test! It is working+");
// 	CPrimitiveRender pr;
// 	pr.depth = 0.0f;
// 	pr.doUseCurrentCoordSystem = false;
// 	pr.grInYan(Vector2(0.0f, 0.0f), 10);

	//glDisable(GL_BLEND);

	//UpdateCameraPosition();

	game::DrawGame();

	r_RenderAll();
	
	//r_ClearRenderData();
/*	if(GLOBAL_QUAKE_ENABLED)
	{
		glPushMatrix();
		ApplyQuake();
	}
*/
/*
	if(iichan)
		iichan->Draw();
*/
	// матрица восстанавливается перед отрисовкой HUD
/*
	if(gui)
		gui->Draw();
*/

/*
	if(cfg.debug)
		PrintDebug();

*/
	if(cfg.show_fps)
	{
		PrintFPS();
	}

#ifdef DEBUG_PRINT
	DbgPrint();

#endif // DEBUG_PRINT

	frames++;
}

// Флип :) (блин, мне нравится, как это слово звучит)
void scene::Flip()
{
	glFlush();
	SwapBuffers(hDC);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Инициализация
void scene::InitScene()
{
	startup_time = GetTickCount();
	InitWindow(hInst, FALSE);

	// Создание скриптовой подсистемы и исполнение скрипта инициализаии.
	if(SCRIPT::InitScript() || SCRIPT::ExecFile(DEFAULT_INIT_SCRIPT))
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "No init: aborting game");
		current_state = PROGRAMM_EXITING;
	}
	else
	{
		current_state = PROGRAMM_RUNNING;
	}


	//scene::PrintExtensions();
	//LogToFile("Checking for required OpenGL-extensions...");
	//scene::InitExtensions();

	//render_data* rd = NULL;
	//rd = MakeRenderData(10, 10, 0, 0, 125, 155, TextureByName("unyl_tan_figure"), false);
	//r_AddRenderData(rd);
	//free(rd);

	//rd = MakeRenderData(150, 130, 0, 0, 125, 155, TextureByName("unyl_tan_figure"), true);
	//r_AddRenderData(rd);
	//free(rd);

	//rd = MakeRenderData(30, 20, 0, 0, 512, 256, TextureByName("logo-cold"), 0);
	//r_AddRenderData(rd);
	//free(rd);
}

// Применяет конфиг и вызывает изменение окна
void scene::ApplyConfig(config* cfg)
{
	scene::ResizeScene(cfg->window_width,
		cfg->window_height,
		cfg->scr_width,
		cfg->scr_height,
		cfg->fullscreen);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Деинициализации

// Освобождает все подсистемы и сцену
void scene::FreeScene()
{
	//scene::FreeExtensions();
	//scene::FreeSND();
	//scene::Freeii();
	//scene::FreeConsole();
	//scene::FreeGUI();

	DeleteAllEvents();
	game::FreeGame();
		
	r_ClearRenderData();
	FreeFonts();
	FreeTextures();

	SCRIPT::FreeScript();
	scene::KillWindow(hInst);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Окно игры

// Создание окна
void scene::InitWindow(HINSTANCE hInstance, BOOL fullscreen)
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
	window_class.lpszClassName = "IIChantraWindowClass";

	dwStyle = WS_CAPTION | WS_BORDER;

	if(fullscreen)
		dwStyle = WS_POPUP;

	if(!RegisterClass(&window_class))
	{
		MessageBox(NULL, "Failed to register window class", "RegisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to register window class");
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
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to create game window");
		MessageBox(NULL, "Failed to create game window", "CreateWindowEx() Error", MB_OK |									MB_ICONEXCLAMATION);
		PostQuitMessage(-1);						
	}

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = 32;//cfg.scr_bpp; // PZDC

	hDC = GetDC(game_window);

	if(!hDC)
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to create device context");
		MessageBox(NULL, "Failed to create device context", "GetDC() Error", MB_OK | MB_ICONEXCLAMATION);
		PostQuitMessage(-1);
	}

	pf = ChoosePixelFormat(hDC, &pfd);

	if(!pf)
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to choose pixel format");
		MessageBox(NULL, "Failed to set pixel format", "ChoosePixelFormat() Error", MB_OK |									MB_ICONEXCLAMATION);
		PostQuitMessage(-1);
	}

	if(!SetPixelFormat(hDC, pf, &pfd))
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to set pixel format");
		MessageBox(NULL, "Failed to set pixel format", "SetPixelFormat() Error", MB_OK | MB_ICONEXCLAMATION);
		PostQuitMessage(-1);
	}

	hRC = wglCreateContext(hDC);

	if(!hRC)
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to create rendering context");
		MessageBox(NULL, "Failed to create rendering context", "wglCreateContext() Error", MB_OK |							MB_ICONEXCLAMATION);
		PostQuitMessage(-1);
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
		sLog(DEFAULT_LOG_NAME, LOG_ERROR_EV, "Failed to make current context");
		MessageBox(NULL, "Failed to make current context", "wglMakeCurrent() Error", MB_OK |								MB_ICONEXCLAMATION);
		PostQuitMessage(-1);
	}

	ShowWindow(game_window, SW_SHOW);
	SetForegroundWindow(game_window);
	SetFocus(game_window);
}

// Уничтожает окно
void scene::KillWindow(HINSTANCE hInstance)
{
	if(fullscreenflag)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if(hRC)
	{
		if(!wglMakeCurrent(NULL, NULL))
		{
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to release current context");
			MessageBox(NULL, "Failed to release current context", "wglMakeCurrent() Error", MB_OK | MB_ICONEXCLAMATION);
		}

		if(!wglDeleteContext(hRC))
		{
			sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to delete rendering context");
			MessageBox(NULL, "Failed to delete rendering context", "wglDeleteContext() Error", MB_OK | MB_ICONEXCLAMATION);
		}

		hRC = NULL;
	}

	if(hDC && ReleaseDC(game_window, hDC) != 1)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to release device context");
		MessageBox(NULL, "Failed to release device context", "ReleaseDC() Error", MB_OK | MB_ICONEXCLAMATION);
		hDC = NULL;
	}

	if(game_window && DestroyWindow(game_window) == 0)
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to destroy game window");
		MessageBox(NULL, "Failed to destroy game window", "DestroyWindow() Error", MB_OK | MB_ICONEXCLAMATION);
		game_window = NULL;
	}

	if(!UnregisterClass(window_class.lpszClassName, hInstance))
	{
		sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to unregister game window class");
		MessageBox(NULL, "Failed to unregister game window class", "UnregisterClass() Error", MB_OK | MB_ICONEXCLAMATION);
		hInstance = NULL;
	}			
}

// Настройки OpenGL
static void InitOGLSettings()
{
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearDepth( 1.0f );              // Разрешить очистку буфера глубины
	glEnable( GL_DEPTH_TEST );            // Разрешить тест глубины
	glDepthFunc( GL_LEQUAL );            // Тип теста глубины

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
	glReadBuffer(GL_BACK);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glDisable(GL_BLEND);
}

// Изменяет размер окна. Вызывает настройку OpenGL.
void scene::ResizeScene(UINT window_width, UINT window_height,
						UINT scr_width, UINT scr_height, BOOL fullscreen)
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

	AdjustWindowRect(&new_rect, dwStyle, FALSE);
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

		//gluOrtho2D(0.0, scr_width, scr_height, 0.0);
		glOrtho(0.0, scr_width, scr_height, 0.0, cfg.near_z, cfg.far_z);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else
	{
		scene::KillWindow(hInst);
		scene::InitWindow(hInst, TRUE);

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

		disp = ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		switch(disp)
		{
		case DISP_CHANGE_SUCCESSFUL:
			{
				fullscreenflag = TRUE;
				ShowCursor(TRUE);
				break;
			}
		case DISP_CHANGE_BADDUALVIEW:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_BADDUALVIEW)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADDUALVIEW", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_BADFLAGS:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_BADFLAGS)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADFLAGS", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_BADMODE:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_BADMODE)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADMODE", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_BADPARAM:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_BADPARAM)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_BADPARAM", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_FAILED:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_FAILED)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_FAILED", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_NOTUPDATED:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_NOTUPDATED)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_NOTUPDATED", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		case DISP_CHANGE_RESTART:
			{
				fullscreenflag = FALSE;
				ShowCursor(TRUE);
				sLog(DEFAULT_LOG_NAME, LOG_WARNING_EV, "Failed to set fullscreen mode: error (DISP_CHANGE_RESTART)");
				MessageBox(NULL, "Failed to set fullscreen mode", "DISP_CHANGE_RESTART", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		}

	}
	fullscreenflag = fullscreen;
}


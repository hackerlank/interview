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
#include "draw/effects.h"
#include "draw/bmp.h"
#include "console.h"
#include "sound/snd.h"
#include "draw/vbo.h"
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
extern GUI* gui;
extern ii* iichan;
extern Console* console;
extern unsigned int current_state;
extern config cfg;
extern HINSTANCE hInst;

extern BOOL GLOBAL_QUAKE_ENABLED;

extern BYTE GLOBAL_COLOR_RED;
extern BYTE GLOBAL_COLOR_GREEN;
extern BYTE GLOBAL_COLOR_BLUE;
extern BYTE GLOBAL_COLOR_ALPHA;

extern BOOL TERMINATE_IICHAN;

extern bool VBO_ENABLED;

// TODO: избавиться от зависимостей
extern unsigned int current_state;
extern vector<Font> fonts;
extern vector<RESOURCE> resources;
extern vector<Texture> textures;


extern string dbgTime;
#ifdef _DEBUG
extern string dbgKeyboard;
extern string dbgQuake;
extern string dbgAI;
extern string dbgHUD;
extern string dbgPause;
extern string dbgWeather;
extern string dbgMenu;
extern string dbgDialog;
extern string dbgWorldX;
extern string dbgWorldY;
extern string dbgEventCount;
extern string dbgSpriteCount;

extern unsigned int RENDERED_TEXTURES;
string dbgRenderedTextures;
extern string dbgEventSystemTime;

#endif


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
vector<UCHAR> pressed_keys;
bool keys[256];
unsigned int startup_time;
unsigned int current_time;
unsigned int previous_time;
unsigned int frames = 0;
unsigned int fps = 1;
ii* iichan = NULL;
GUI* gui = NULL;
HINSTANCE hInst = NULL;
SND* snd = NULL;
Console *console = NULL;
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
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void scene::ClearScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

#ifdef _DEBUG
	RENDERED_TEXTURES = 0;
#endif
}

void PrintFPS()
{
	static const Font* font = FontByName(DEBUG_FONT);
	glColor4ub(0, 255, 0, 255);
	char frames_per_second[16];
	sprintf(frames_per_second, "FPS: %u\0", fps);
	PrintText(frames_per_second, font, 5, 40);
}

bool scene::SaveScreenshot(int scr_width, int scr_height, int scr_bpp)
{
	AUX_RGBImageRec bmp;
	char file_name[MAX_PATH];
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

	bool result = (bmpWrite(&bmp, file_name) != 0);
	delete[] bmp.data;
	return result;
}

void scene::CountFPS()
{
	static int lasttick;
	if(current_time - lasttick >= 1000)
	{
		fps = frames;
		frames = 0;
		lasttick = current_time;
	}
}

void scene::ProcessKeyboard()
{
	static unsigned int _prev_time;
	static unsigned char _prev_key;


	memset(keys, false, 256);
	for(UCHAR i = 0; i < 255; i++)
	{
		if(KEYDOWN(i))
		{
			if(i == _prev_key)
			{
				if(current_time - _prev_time >= 50)
				{
					keys[i] = true;
				}
			}
			else
				keys[i] = true;

			_prev_time = current_time;
			_prev_key = i;
		}
	}

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
		if(!scene::SaveScreenshot(cfg.window_width, cfg.window_height, cfg.scr_bpp))
			LogToFile("Failed to save screenshot");
	}
}

void scene::InitGUI()
{
	gui = new GUI();
}

void scene::FreeGUI()
{
	if(gui)
	{
		delete gui;
		gui = NULL;
	}
}

void scene::InitConsole()
{
	console = new Console();
}

void scene::FreeConsole()
{
	if(console)
	{
		delete console;
		console = NULL;
	}
}

void scene::FreeFonts()
{
	for(vector<Font>::iterator it = fonts.begin();
		it != fonts.end();
		it++)
	{
		Font* f = &(*it);
		if(f->name)
			delete[] f->name;
		if(f->tex_name)
			delete[] f->tex_name;

		glDeleteLists(f->base, 256);
	}
	fonts.clear();
}

void scene::FreeTextures()
{
	for(vector<Texture>::iterator it = textures.begin();
		it != textures.end();
		it++)
	{
		Texture* t = &(*it);
		if(t->name)
			delete[] t->name;

		glDeleteTextures(1, &(t->tex));
	}
	textures.clear();
}

void scene::FreeResources()
{
	resources.clear();
}

void scene::Freeii()
{
	if(iichan)
	{
		delete iichan;
		iichan = NULL;
	}
}

void scene::FreeSND()
{
	if(snd)
	{
		delete snd;
		snd = NULL;
	}	
}

void scene::MainLoop()
{
	current_time = (GetTickCount() - startup_time);

	ProcessKeyboard();

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
}


void PrintDebug()
{
	static const Font* font = FontByName(DEBUG_FONT);
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

#ifdef _DEBUG
	glColor4ub(192, 0, 0, 255);
	PrintText(dbgKeyboard.c_str(), font, 5, 55);
	PrintText(dbgQuake.c_str(), font, 5, 70);
	PrintText(dbgAI.c_str(), font, 5, 85);
	PrintText(dbgHUD.c_str(), font, 5, 100);
	PrintText(dbgPause.c_str(), font, 5, 115);
	PrintText(dbgWeather.c_str(), font, 5, 130);
	PrintText(dbgMenu.c_str(), font, 5, 145);
	PrintText(dbgDialog.c_str(), font, 5, 160);
	PrintText(dbgTime.c_str(), font, 5, 175);
	PrintText(dbgEventSystemTime.c_str(), font, 5, 190);
	PrintText(dbgWorldX.c_str(), font, 5, 205);
	PrintText(dbgWorldY.c_str(), font, 5, 220);
	PrintText(dbgEventCount.c_str(), font, 5, 235);
	PrintText(dbgSpriteCount.c_str(), font, 5, 250);
	dbgRenderedTextures = "Textures rendered: " + IntToString(RENDERED_TEXTURES);
	PrintText(dbgRenderedTextures.c_str(), font, 5, 265);
#endif
}


void scene::DrawScene()
{
	glColor4ub(255 - GLOBAL_COLOR_RED,
		255 - GLOBAL_COLOR_GREEN,
		255 - GLOBAL_COLOR_BLUE,
		255 - GLOBAL_COLOR_ALPHA);

	if(GLOBAL_QUAKE_ENABLED)
	{
		glPushMatrix();
		ApplyQuake();
	}

	if(iichan)
		iichan->Draw();

	// матрица восстанавливается перед отрисовкой HUD

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
	glDisable(GL_BLEND);
}


void scene::ApplyConfig(config* cfg)
{
	scene::ResizeScene(cfg->window_width,
		cfg->window_height,
		cfg->scr_width,
		cfg->scr_height,
		cfg->fullscreen);
}



void scene::ResizeScene(UINT window_width, UINT window_height,
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
		scene::KillWindow(hInst);
		scene::InitWindow(hInst, true);

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




void scene::Flip()
{
	glFlush();
	SwapBuffers(hDC);
}


void scene::InitWindow(HINSTANCE hInstance, bool fullscreen)
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

	dwStyle = WS_CAPTION | WS_BORDER;

	if(fullscreen)
		dwStyle = WS_POPUP;

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


void scene::KillWindow(HINSTANCE hInstance)
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

bool scene::ExtensionSupported(const char* ext)
{
	static const char* ext_list = (const char*)glGetString(GL_EXTENSIONS);
	const char* start = ext_list;
	const char* ptr;

	while((ptr = strstr(start, ext)) != NULL)
	{
		const char* end = ptr + strlen(ext);
		if(isspace(*end) || *end == '\0')
			return true;

		start = end;
	}
	return false;
}


void scene::PrintExtensions()
{
	const unsigned char* ext_list = glGetString(GL_EXTENSIONS);
	LogToFile("OpenGL enabled extensions:");
	
	char buffer[1024]; // надеюсь хватит
	int j = 0;
	for(int i = 0, j = 0; ext_list[i] != '\0'; i++)
	{
		if(!isspace(ext_list[i]))
			buffer[j++] = ext_list[i];
		else
		{
			buffer[j] = '\0';
			LogToFile(string(buffer));
			j = 0;
		}
	}

	if(j > 0)
	{
		buffer[j] = '\0';
		LogToFile(string(buffer));
	}
}

void scene::InitExtensions()
{
	if(scene::ExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
		LogToFile("GL_ARB_vertex_buffer_object supported");
		VBO_ENABLED = true;
	}
	else
	{
		LogToFile("GL_ARB_vertex_buffer_object not supported");
		VBO_ENABLED = false;
	}

	//

	if(VBO_ENABLED)
		InitVBO();
}

void scene::FreeExtensions()
{
	if(VBO_ENABLED)
		FreeVBO();
}
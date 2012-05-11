//-----------------------------------------------------------------------------
// Part of FS engine.
//
// CATEGORY:
// Windowing
//
// DESCRIPTION:
//	Window initializing and handling routines
//	WndProc
//-----------------------------------------------------------------------------

#include "stdafx.h"

#include "fs_data.h"
#include "m_tools.h"


#include "w_window.h"


// for R_ResizeRenderer

#include "r_renderer.h"


//?
bool active = true;

long WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
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
	case WM_SIZE:
		{
		R_ResizeRenderer(LOWORD(lParam), HIWORD(lParam), hWnd);
		return 0;
		}


	case WM_DESTROY:
	case WM_QUIT:
		current_state |= GS_EXITING;
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}



// w_window.h routines

extern HINSTANCE hInst;

//?

bool W_Init(void)
{
	WNDCLASS window_class;
	ZeroMemory(&window_class, sizeof(WNDCLASS));

	window_class.hInstance = hInst;
	window_class.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hbrBackground = NULL;
	window_class.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = WINDOW_CLASS_NAME;

	return (RegisterClass(&window_class) == 0);
}



//?

bool W_Free(void)
{
	return (UnregisterClass(WINDOW_CLASS_NAME, hInst) == 0);
}



//?

HWND W_CreateWindow(config_t* cfg)
{

	if(!cfg)
	{
		I_ERROR("No config to create window");
		return NULL;
	}

	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = cfg->wnd_width;
	rect.bottom = cfg->wnd_height;

	int style;

	cfg->fullscreen ? style = WS_POPUP : style = WS_OVERLAPPEDWINDOW;

	AdjustWindowRect(&rect, style, false);

	int x;
	int y;
	int w;
	int h;

	if(cfg->fullscreen)
	{
		x = 0;
		y = 0;
		w = cfg->wnd_width;
		h = cfg->wnd_height;
	}
	else
	{
		x = GetSystemMetrics(0) - GetSystemMetrics(0)/2 - rect.right / 2;
		y = GetSystemMetrics(1) - GetSystemMetrics(1)/2 - rect.bottom / 2;
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}

	HWND window = NULL;

	window = CreateWindow(WINDOW_CLASS_NAME, GAME_NAME, style, x, y, w, h,
		NULL, NULL, hInst, NULL);

	SetWindowPos(window, HWND_TOP, x, y, w, h,	SWP_SHOWWINDOW);

	UpdateWindow(window);

	ShowWindow(window, SW_SHOW);
	SetForegroundWindow(window);
	SetFocus(window);

	return window;
}



//?

void W_DestroyWindow(HWND window)
{
	DestroyWindow(window);
}
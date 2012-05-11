// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OORION_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OORION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef OORION_EXPORTS
#define OORION_API __declspec(dllexport)
#else
#define OORION_API __declspec(dllimport)
#endif

#ifdef __gl_h_
#define USE_OPENGL
#define _RENDERER "OPENGL"
#else
#define _RENDERER "NOT OPENGL"
#endif

OORION_API const string DEFAULT_LOG_PATH = "debug.log";
OORION_API const string DEFAULT_CFG_PATH = "default.cfg";
OORION_API const string DEFAULT_TEXTURES_PATH = "\\textures\\";
OORION_API const UINT DEFAULT_SCR_WIDTH = 640;
OORION_API const UINT DEFAULT_SCR_HEIGHT = 480;
OORION_API const unsigned char DEFAULT_SCR_BPP = 16;
OORION_API const bool DEFAULT_SCR_FULLSCREEN = false;
OORION_API const bool DEFAULT_SCR_VERT_SYNC = false;

OORION_API const UCHAR MAX_FONTS = 10;
OORION_API const UINT MAX_TEXTURES = 500;


namespace OORION {
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool keys[256];
	static bool mouse[3];
	static int mousex;
	static int mousey;
};

/*
------------------------------------------------
- Primary Game Window class
------------------------------------------------
*/
class OORION_API ooGAME {
public:
/* **************************************************************************************
	CONSTRUCTOR & DESTRUCTOR
 ************************************************************************************* */
	ooGAME(LPCSTR GameName, const char szWndClassName[], string cfg_path);
	~ooGAME();

/* **************************************************************************************
	WINDOWING
 ************************************************************************************* */
	HINSTANCE hInstance;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	HICON hIcon;
	HCURSOR hCursor;
	LPCSTR szClassName;
	WNDCLASS wc;
	RECT WindowRect;
	PIXELFORMATDESCRIPTOR pfd;
	UINT pf;
	DEVMODE dmScreenSettings;
	DWORD dwStyle;
	DWORD dwExStyle;
	MSG msg;
	string caption;
	
/* **************************************************************************************
	DISPLAY SETTINGS
 ************************************************************************************* */	
	bool scr_fullscreen;
	bool scr_vert_sync;
	UINT scr_width;
	UINT scr_height;
	unsigned char scr_bpp;
	static string RENDERER;

/* **************************************************************************************
	DISPLAY FUNCTIONS
 ************************************************************************************* */	
	int SetResolution(UINT, UINT);
	int SetFullscreen(void);
	void ResizeScene(void);
	void DrawScene(void);
	void ClearScene(void);
	void Flip(void);
/* **************************************************************************************
	LOGGING
 ************************************************************************************* */
	ofstream log;
	int StartLog(const string file_name);
	void WriteToLog(const string message);
	void WriteToLogInfo(const string message);
	void WriteToLogDebug(const string message);
	void WriteToLogError(const string message);
	void WriteToLogFatal(const string message);
	void EndLog();

/* **************************************************************************************
	CONFIG I/O
 ************************************************************************************* */
	fstream config;
	int ReadConfig(const string file_name);
	int WriteConfig(const string file_name);
	int WriteDefaultConfig(void);

/* **************************************************************************************
	GAME VARIABLES
 ************************************************************************************* */
	bool running;
	unsigned int fps;
	char path_log[MAX_PATH];
	char path_config[MAX_PATH];
	char path_app[MAX_PATH];
	char path_textures[MAX_PATH];

/* **************************************************************************************
	GAME FUNCTIONS
 ************************************************************************************* */
	void MainLoop(void);
	void HandleErrors(void);

/* **************************************************************************************
	TEXTURES & SPRITES
 ************************************************************************************* */
	texture textures[MAX_TEXTURES]; // TODO: dynamic array
	int tex_count;

	void LoadAllTextures(void);
	void UnloadAllTextures(void);

/* **************************************************************************************
	FONTS
 ************************************************************************************* */
	font fonts[MAX_FONTS]; // TODO: dynamic array

	void InitFonts(void);
	int AddStandardFont(char *font_name, char *name, UCHAR height);
	int AddFontFromBitmap(char *file_name);

/* **************************************************************************************
	TEXTOUT
 ************************************************************************************* */
	void TextOut(char *text, char *font_name, UINT x, UINT y, UCHAR Red, UCHAR Green, UCHAR Blue, UCHAR Alpha);

/* **************************************************************************************
	PIXEL DRAWINGS
 ************************************************************************************* */
	void PutPixel(UINT, UINT, UCHAR, UCHAR, UCHAR);
	void PutPixelf(float, float, float, float, float);

/* **************************************************************************************
	TIME
 ************************************************************************************* */
	DWORD startup_time; // milliseconds
	DWORD current_time; // seconds



/* **************************************************************************************
	OTHER
 ************************************************************************************* */
	void SaveScreenshot(void);
	void InitPaths(void);
	void WriteToLogPaths(void);
	
private:
	
	void KillAll(void);
	void SetPfd(void);
	int GetConfig(void);
	void InitRenderer(void);
	void ApplyWndStyle(void);


	bool SearchFiles(LPCTSTR lpszFileName, BOOL bInnerFolders);

	// TODO:
	// Add here format handlers
	void OnFoundBMP(LPCTSTR lpszFileName);
	void OnFoundTGA(LPCTSTR lpszFileName);

};




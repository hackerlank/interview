#include "config.h"

namespace scene
{
	void InitGUI();
	void FreeGUI();
	void InitConsole();
	void FreeConsole();
	void FreeFonts();
	void FreeTextures();
	void FreeResources();
	void Freeii();
	void FreeSND();
	void MainLoop();
	bool SaveScreenshot(int scr_width, int scr_height, int scr_bpp);
	void CountFPS();
	void ProcessKeyboard();
	void ResizeScene(UINT window_width, UINT window_height,
					 UINT scr_width, UINT scr_height, bool fullscreen);
	void ClearScene();
	void DrawScene();
	void Flip();
	void ApplyConfig(config* cfg);
	void InitWindow(HINSTANCE hInstance, bool fullscreen);
	void KillWindow(HINSTANCE hInstance);
	bool ExtensionSupported(const char* ext);
	void PrintExtensions();
	void InitExtensions();
	void FreeExtensions();
};
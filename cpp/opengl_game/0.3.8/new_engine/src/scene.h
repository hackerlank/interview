#ifndef __SCENE_H_
#define __SCENE_H_

namespace scene
{
	void MainLoop();
	void UpdateScene();
	void ClearScene();
	void DrawScene();
	void Flip();
	void InitWindow(HINSTANCE hInstance, BOOL fullscreen);
	void KillWindow(HINSTANCE hInstance);
	void InitScene();
	void FreeScene();
	void ResizeScene(UINT window_width, UINT window_height, UINT scr_width, UINT scr_height, BOOL fullscreen);
	void ApplyConfig(config* cfg);
	void ProcessKeyboard();
};



#endif // __SCENE_H_


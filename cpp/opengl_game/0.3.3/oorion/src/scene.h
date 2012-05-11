void ResizeScene(UINT window_width, UINT window_height,
				 UINT scr_width, UINT scr_height, bool fullscreen);
void ClearScene();
void DrawScene();
void Flip();

void ApplyConfig(config* cfg);

void InitWindow(HINSTANCE hInstance, bool fullscreen);
void KillWindow(HINSTANCE hInstance);
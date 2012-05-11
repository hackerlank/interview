HWND game_window;
WNDCLASS window_class;
RECT window_rect;
DEVMODE dmScreenSettings;
int dwExStyle;
int dwStyle;
bool keys[256];
config cfg;
int frames;
int fps;
int lasttick;


int drawtime;
int drawtime_prv;
int drawtime_med;
int drawtime_med_dr;
int cleartime;
int cleartime_prv;
int cleartime_med;
int cleartime_med_dr;
int mainlooptime;
int mainlooptime_prv;
int mainlooptime_med;
int mainlooptime_med_dr;
int fliptime;
int fliptime_prv;
int fliptime_med;
int fliptime_med_dr;



HDC hDC;
HGLRC hRC;
PIXELFORMATDESCRIPTOR pfd;
int pf;
MSG msg;
int startup_time;
int current_time;
int previous_time;

font_t fonts[256];
texture_t textures[256];
int tex_count;
int font_count;

char path_log[MAX_PATH];
char path_config[MAX_PATH];
char path_app[MAX_PATH];
char path_textures[MAX_PATH];
char path_fonts[MAX_PATH];

int mouseX;
int mouseY;
bool mouse[3];

//GUI* gui;
GUIcontrol* test_button;
GUIcontrol* test_panel;
GUI* gui;
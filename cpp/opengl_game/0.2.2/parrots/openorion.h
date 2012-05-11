HWND game_window;
WNDCLASS window_class;
RECT window_rect;
DEVMODE dmScreenSettings;
int dwExStyle;
int dwStyle;
bool keys[256];
bool fullscreenflag;
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

font_t fonts[5];
texture_t textures[20];
int tex_count;
int font_count;

char path_log[MAX_PATH];
char path_config[MAX_PATH];
char path_app[MAX_PATH];
char path_textures[MAX_PATH];
char path_fonts[MAX_PATH];
char path_def[MAX_PATH];
char path_scripts[MAX_PATH];
//string path_log;
//string path_config;
//string path_app;
//string path_textures;
//string path_fonts;
//string path_def;
//string path_scripts;



int mouseX;
int mouseY;
bool mouse[3];

GUI* gui;
GAME* game;

int map_dx;
int map_dy;

GAMEDEFINITION gamedef;
GL_DEFINITION_STORAGE gl_def_storage;
KEYWORD_STORAGE key_storage;




bool keys[256];



config cfg;
int frames = 0;
int fps = 1;
int lasttick = 0;

#ifdef _DEBUG_TIMINGS
int drawtime = 0;
int drawtime_prv = 0;
int drawtime_med = 0;
int drawtime_med_dr = 0;
int cleartime = 0;
int cleartime_prv = 0;
int cleartime_med = 0;
int cleartime_med_dr = 0;
int mainlooptime = 0;
int mainlooptime_prv = 0;
int mainlooptime_med = 0;
int mainlooptime_med_dr = 0;
int fliptime = 0;
int fliptime_prv = 0;
int fliptime_med = 0;
int fliptime_med_dr = 0;
#endif





int startup_time = 0;
int current_time = 0;
int previous_time = 0;

vector<font_t> fonts;
vector<texture_t> textures;
int tex_count = 0;
int font_count = 0;

char path_log[MAX_PATH];
char path_config[MAX_PATH];
char path_app[MAX_PATH];
char path_textures[MAX_PATH];
char path_fonts[MAX_PATH];
char path_def[MAX_PATH];
char path_scripts[MAX_PATH];




int mouseX = 0;
int mouseY = 0;
bool mouse[3];

GUI* gui;
GAME* game;

int map_dx = 0;
int map_dy = 0;
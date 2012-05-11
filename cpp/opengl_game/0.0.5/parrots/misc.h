#define DEFAULT_LOG_NAME "debug.log"
#define DEFAULT_CONFIG_NAME "default.cfg"
#define DEFAULT_TEXTURES_PATH "\\textures"

extern char path_log[MAX_PATH];
extern char path_config[MAX_PATH];
extern char path_app[MAX_PATH];
extern char path_textures[MAX_PATH];

string NameFromPath(string path);
string ExtFromPath(string path);
string IntToString(int intValue);
bool DirectoryExists(const char path[]);
bool FileExists(const char file_name[]);
bool _LogToFile(char file_name[], char* message);
bool LogToFile(string message);
bool LogToFile(char file_name[], string message);
void InitPaths(void);
void LogPaths(void);

void ShowNumber(int n);
bool PointInBox(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);






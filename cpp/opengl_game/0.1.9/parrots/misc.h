//#define DEFAULT_LOG_NAME "debug.log"
//#define DEFAULT_CONFIG_NAME "default.cfg"
//#define DEFAULT_TEXTURES_PATH "\\textures"
//#define DEFAULT_FONTS_PATH "\\fonts"

extern char path_log[MAX_PATH];
extern char path_config[MAX_PATH];
extern char path_app[MAX_PATH];
extern char path_textures[MAX_PATH];
extern char path_fonts[MAX_PATH];
extern char path_def[MAX_PATH];

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
bool PointInBox(int px, int py, int x1, int y1, int width, int height);
int Random(int limit);
int Random(int from, int till);

void DeleteSpaces(string s);
void DeleteComments(string s);

typedef void (*LPSEARCHFUNC)(LPCTSTR lpszFileName);
BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders);
                                           





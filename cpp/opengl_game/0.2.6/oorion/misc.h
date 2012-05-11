string NameFromPath(string path);
string ExtFromPath(string path);
string IntToString(int intValue);
bool DirectoryExists(const char* path);
bool FileExists(const char* file_name);
bool _LogToFile(char* file_name, char* message);
bool LogToFile(string message);
bool LogToFile(char* file_name, string message);
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
bool IsIntNumber(string s);
bool IsFloatNumber(string s);


                                           





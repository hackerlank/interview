#ifndef __MISC_H_
#define __MISC_H_

struct Point {
	int x;
	int y;
};


string NameFromPath(string path);
string ExtFromPath(string path);
string IntToString(int intValue);
bool DirectoryExists(const char* path);
bool FileExists(const char* file_name);
bool _LogToFile(char* file_name, char* message);
inline bool LogToFile(string message);
bool _LogToFileTrunc(char* file_name, char* message);
bool LogToFile(char* file_name, string message);
bool LogToFileNoCLRF(char* file_name, string message);
bool LogToFileNoTime(char* file_name, string message);





void InitPaths(void);
void LogPaths(void);
void CheckDefaultConfig();
void CheckPaths();
void ShowNumber(int n);
bool PointInBox(int px, int py, int x1, int y1, int width, int height);
int Random(int limit);
float Random(float limit);
int Random(int from, int till);
float Random(float from, float till);

void DeleteSpaces(string s);
void DeleteComments(string s);

void CleanupLogs();
void EndupLogs();

typedef void (*LPSEARCHFUNC)(LPCTSTR lpszFileName);
BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders);
bool IsIntNumber(string s);
bool IsFloatNumber(string s);

bool IsAlpabetic(UCHAR c);
bool IsNumeric(UCHAR c);
bool IsNumKeyb(UCHAR c);
bool IsSymbolKey(UCHAR c);

template <class T> int GetIndexById(vector<T*>* vec, int id)
{
	int index = 0;
	for(vector<T*>::iterator it = vec->begin();
		it != vec->end();
		it++)
	{
		T* obj = *it;
		if(obj->GetId() == id)
			return index;
		index++;
	}
	return -1;
}

#endif
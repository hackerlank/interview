#include "stdafx.h"
#include "misc.h"


char path_log[MAX_PATH];
char path_config[MAX_PATH];
char path_app[MAX_PATH];
char path_textures[MAX_PATH];
char path_fonts[MAX_PATH];
char path_objects[MAX_PATH];
char path_scripts[MAX_PATH];
char path_levels[MAX_PATH];
char path_dialogs[MAX_PATH];



string NameFromPath(string path)
{
	static const string delim = "\\/";
	string ret;

	UINT dot_pos = path.find_last_of('.');
	UINT delim_pos = path.find_last_of(delim);

	if(dot_pos >= 0 && delim_pos >= 0)
		ret = path.substr(delim_pos + 1, dot_pos-delim_pos-1);
	else
		ret = path;

	return ret;
}

int Random(int limit)
{
	errno_t err;
	unsigned int number;
	int result;

	err = rand_s(&number);

	if(err != 0)
	{
		LogToFile("rand_s() failed");
		return -1;
	}

	result = int(((float)number / (float)UINT_MAX * (float)limit));

	return result;
}

int Random(int from, int till)
{
	return Random(from) + (till - from);
}

string ExtFromPath(string path)
{
	string ret = "";

	for(int i = path.length(); i > 0; i--)
		if(path[i] == '.')
		{
			for(unsigned int j = i+1; j < path.length(); j++)
				ret.push_back(path[j]);
			return ret;
		}

	return ret;
}

string IntToString(int intValue)
{ 
	char *myBuff; 
	string strRetVal;
	myBuff = new char[16]; 
	memset(myBuff,'\0', 16); 
	_itoa(intValue, myBuff, 10); 
	strRetVal = myBuff; 
	delete[] myBuff; 
	return strRetVal; 
}

bool DirectoryExists(const char* path)
{
	if(GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES)
		return true;
	else
		return false;
}

bool FileExists(const char* file_name)
{
	if(GetFileAttributes(file_name) != INVALID_FILE_ATTRIBUTES)
		return true;
	else
		return false;
}

bool _LogToFile(char* file_name, char* message)
{
	static bool error = false;

	if(error)
		return false;

	time_t current_time;
	string time_stamp;
	ofstream f;
	int openmode;

	time(&current_time);
	time_stamp = string(ctime(&current_time));
	time_stamp = string(time_stamp, 11, 8) + ' ';

	// TODO: FIX IT

	// shitty code
	static bool f_main_log = true;
	static bool f_gui_log = true;
	static bool f_osl_log = true;

	openmode = ios::out | ios::app;

	if(f_main_log)
	{
		if(strcmp(file_name, DEFAULT_LOG_NAME) == 0)
		{
			openmode = ios::out | ios::trunc;
			f_main_log = false;
		}
	}

	if(f_gui_log)
	{
		if(strcmp(file_name, DEFAULT_GUI_LOG_NAME) == 0)
		{
			openmode = ios::out | ios::trunc;
			f_gui_log = false;
		}
	}

	if(f_osl_log)
	{
		if(strcmp(file_name, DEFAULT_OSL_LOG_NAME) == 0)
		{
			openmode = ios::out | ios::trunc;
			f_osl_log = false;
		}
	}


	string file = "";
	file += string(path_log);
	file += string(file_name);

	f.open(file.c_str(), openmode);
	if(f.is_open())
	{
		f << time_stamp <<  message << endl;
		f.close();
		return true;
	}
	else
	{
		char buf[512];
		sprintf(buf, "Failed to open log-file: %s", file.c_str());
		MessageBox(NULL, buf, NULL, NULL);
		error = true;
		return false;
	}
}

bool LogToFile(char* file_name, string message)
{
	return _LogToFile(file_name, (char*)message.c_str());
}

bool LogToFile(string message)
{
	return _LogToFile((char*)DEFAULT_LOG_NAME, (char*)message.c_str());
}

void InitPaths(void)
{
	// application
	GetCurrentDirectory(MAX_PATH, path_app);

	// log
	strcpy(path_log, path_app);
	strcat(path_log, DEFAULT_LOG_PATH);

	// config
	strcpy(path_config, path_app);
	strcat(path_config, DEFAULT_CONFIG_NAME);

	// textures
	strcpy(path_textures, path_app);
	strcat(path_textures, DEFAULT_TEXTURES_PATH);

	// fonts
	strcpy(path_fonts, path_app);
	strcat(path_fonts, DEFAULT_FONTS_PATH);

	// objects
	strcpy(path_objects, path_app);
	strcat(path_objects, DEFAULT_OBJECTS_PATH);
	
	// scripts
	strcpy(path_scripts, path_app);
	strcat(path_scripts, DEFAULT_SCRIPTS_PATH);

	// levels
	strcpy(path_levels, path_app);
	strcat(path_levels, DEFAULT_LEVELS_PATH);

	// dialogs
	strcpy(path_dialogs, path_app);
	strcat(path_dialogs, DEFAULT_DIALOGS_PATH);
}

void LogPaths(void)
{
	LogToFile("appl. path:\t" + string(path_app));
	LogToFile("log path:\t" + string(path_log));
	LogToFile("config path:\t" + string(path_config));
	LogToFile("textures path:\t" + string(path_textures));
	LogToFile("fonts path:\t" + string(path_fonts));
	LogToFile("objects path:\t" + string(path_objects));
	LogToFile("scripts path:\t" + string(path_scripts));
	LogToFile("dialogs path:\t" + string(path_dialogs));
}

void ShowNumber(int n)
{
	char* s = new char[10];
	sprintf(s, "%d", n);
	MessageBox(NULL, s, "ShowNumber() Call", NULL);
}

bool PointInBox(int px, int py, int x1, int y1, int width, int height)
{
	if(px >= x1 && px <= x1 + width && py >= y1 && py <= y1 + height)
		return true;
	else
		return false;
}

void DeleteSpaces(string s)
{
	unsigned int i = 0;
	while(i < s.length()-1)
	{
		i++;
		if(isspace(s[i]) != 0)
		{
			s.erase(i, 1);
			i--;
		}
	}
}

BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = TRUE)
{
    LPTSTR part;
    char tmp[MAX_PATH]; // временный массив
    char name[MAX_PATH];

    HANDLE hSearch = NULL;
    WIN32_FIND_DATA wfd;
    memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

    // сначало поиск внутри вложенных папках ...
    if(bInnerFolders)
    {
        if(GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
        strcpy(name, part);
        strcpy(part, "*.*");

        // если папки существуют, то делаем поиск
        wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
        if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
        do
        {
            // в каждой папке есть две папки с именами "." и ".."
            // и эти папки мы не трогаем

            // пропускаем папки "." и ".."
            if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))            
            continue;
        
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // если мы нашли папку
            {
                char next[MAX_PATH];
                if(GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
                strcpy(part, wfd.cFileName);
                strcat(next, "\\");
                strcat(next, name);

                SearchFiles(next, lpSearchFunc, TRUE);
            }
        }
        while (FindNextFile(hSearch, &wfd)); // ищем следующий файл

        FindClose (hSearch); // заканчиваем поиск
    }

    if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE) 
        return TRUE; // в противном случае выходим
    do
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // если мы нашли файл
    {
        char file[MAX_PATH];
        if(GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
        strcpy(part, wfd.cFileName);

        lpSearchFunc(file);
    }
    while (FindNextFile(hSearch, &wfd)); // ищем следующий файл
    FindClose (hSearch); // заканчиваем поиск

    return TRUE;
}

bool IsIntNumber(string s)
{
	int l = s.length();
	
	for(int i = 0; i < l; i++)
	{

		if(s[i] == '-' && i == 0 && l > 1)
			continue;
		else if(s[i] == '+' && i == 0 && l > 1)
			continue;
		else if(isdigit(s[i]))
			continue;
		else
			return false;
	}

	return true;
}

bool IsAlpabetic(char c)
{
	return (c > 32 && c < 96);
}

bool IsFloatNumber(string s)
{
	int l = s.length();
	bool dot = false;
	for(int i = 0; i < l; i++)
	{
		if(i == 0)
			if(!isdigit(s[i]))
				if(s[i] == '-')
					if(l > 1)
						continue;
					else
						return false;

		if(isdigit(s[i]))
			continue;
		else
		{
			if(s[i] == '.')
			{
				if(!dot)
				{
					dot = true;
					continue;
				}
				else
					return false;
			}
		}
	}

	if(!dot)
		return false;
	return true;
}



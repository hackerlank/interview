#include "stdafx.h"
#include "misc.h"
#include <direct.h>

char path_log[MAX_PATH];
char path_config[MAX_PATH];
char path_app[MAX_PATH];
char path_textures[MAX_PATH];
char path_fonts[MAX_PATH];
char path_objects[MAX_PATH];
char path_scripts[MAX_PATH];
char path_levels[MAX_PATH];
char path_dialogs[MAX_PATH];

//#define DISABLE_LOG



/* Процедура проверяет существует ли конфиг DEFAULT_CONFIG_NAME в директории
 * path_config.
 * Если конфига нет, создает его, указывая статические данные.
 */

void CheckDefaultConfig()
{
	char path_to_cfg[MAX_PATH];

	sprintf(path_to_cfg, "%s%s\0", path_config, DEFAULT_CONFIG_NAME);
	if(!FileExists(path_to_cfg))
	{
		WriteToLog("Default.cfg does not exist. Recreating...");
		ofstream f;
		f.open(path_to_cfg, ios::out);
		
		if(f.is_open())
		{
			f << "window_width = 640;\n";
			f << "window_height = 480;\n";
			f << "scr_width = 640;\n";
			f << "scr_height = 480;\n";
			f << "bpp = 16;\n";
			f << "fullscreen = 1;\n";
			f << "vert_sync = 0;\n";
			f << "debug = 0;\n";
			f << "show_fps = 1;\n";
			f << "# controls\n";
			f << "down = VK_DOWN;\n";
			f << "left = VK_LEFT;\n";
			f << "right = VK_RIGHT;\n";
			f << "jump = VK_SPACE;\n";
			f << "fire = VK_CONTROL;\n";
			f << "screenshot = VK_F11;\n";
			f << "# game settings\n";
			f << "bullet_speed = 10;\n";
			f << "bullet_damage = 60;\n";
			f << "hero_reload = 20;\n";
			f << "hero_vuln = 60;\n";
			f << "enemy_speed = 1;\n";
			f << "# world settings\n";
			f << "gravity = 9.8;\n";
			f.close();
			WriteToLog("Default.cfg succesfully recreated");
		}
		else
			WriteToLog("Failed to recreate default.cfg");
	}
}

/* Процедура проверяет существует ли указанные пути.
 * Если их нет, создает их.
 */

void CheckPaths()
{
	if(!DirectoryExists(path_log))
	{
		_mkdir(path_log);
	}
	if(!DirectoryExists(path_config))
	{
		_mkdir(path_config);
	}
	if(!DirectoryExists(path_textures))
	{
		_mkdir(path_textures);
	}
	if(!DirectoryExists(path_fonts))
	{
		_mkdir(path_fonts);
	}
	if(!DirectoryExists(path_objects))
	{
		_mkdir(path_objects);
	}
	if(!DirectoryExists(path_scripts))
	{
		_mkdir(path_scripts);
	}
	if(!DirectoryExists(path_levels))
	{
		_mkdir(path_levels);
	}
	if(!DirectoryExists(path_dialogs))
	{
		_mkdir(path_dialogs);
	}
}

/* Функция возвращает имя файла из его полного пути.
 * Например, для c:\test\file1.txt результат будет file1
 */

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

/* Функция возвращает случайное значение от 0 до limit */

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

float Random(float limit)
{
	errno_t err;
	unsigned int number;
	float result;

	err = rand_s(&number);

	if(err != 0)
	{
		LogToFile("rand_s() failed");
		return -1.0f;
	}

	result = (((float)number / (float)UINT_MAX * (float)limit));
	return result;
}

/* Функция возвращает случайное значение от from до till */

int Random(int from, int till)
{
	return Random(from) + Random(till - from);
}

float Random(float from, float till)
{
	float f = Random(from);
	return Random(f) + Random(till - from);
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
	return (GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES);
}

bool FileExists(const char* file_name)
{
	return (GetFileAttributes(file_name) != INVALID_FILE_ATTRIBUTES);
}

bool _LogToFile(char* file_name, char* message)
{
#ifdef DISABLE_LOG
	return false;
#endif
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

	openmode = ios::out | ios::app;

	string file;
	file += string(path_log);
	file += "\\";
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

bool _LogToFileTrunc(char* file_name, char* message)
{
#ifdef DISABLE_LOG
	return false;
#endif
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

	openmode = ios::out | ios::trunc;

	string file;
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
#ifdef DISABLE_LOG
	return false;
#endif
	return _LogToFile(file_name, (char*)message.c_str());
}

bool LogToFileNoCLRF(char* file_name, string message)
{
#ifdef DISABLE_LOG
	return false;
#endif
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

	openmode = ios::out | ios::app;

	string file;
	file += string(path_log);
	file += "\\";
	file += string(file_name);

	f.open(file.c_str(), openmode);
	if(f.is_open())
	{
		f << time_stamp <<  message;
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

bool LogToFileNoTime(char* file_name, string message)
{
#ifdef DISABLE_LOG
	return false;
#endif
	static bool error = false;

	if(error)
		return false;

	ofstream f;
	int openmode;

	openmode = ios::out | ios::app;

	string file;
	file += string(path_log);
	file += "\\";
	file += string(file_name);

	f.open(file.c_str(), openmode);
	if(f.is_open())
	{
		f << message << endl;
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

bool LogToFile(string message)
{
#ifdef DISABLE_LOG
	return false;
#endif
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
	strcat(path_config, DEFAULT_CONFIG_PATH);

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
#ifdef DISABLE_LOG
	return;
#endif
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
	return (px >= x1 && px <= x1 + width && py >= y1 && py <= y1 + height);
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

bool IsAlpabetic(UCHAR c)
{
//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
	return (c >= 0x41 && c <= 0x5A);
}

bool IsNumeric(UCHAR c)
{
//VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
	return (c >= 0x30 && c <= 0x39);
}

bool IsNumKeyb(UCHAR c)
{
	return (c >= VK_NUMPAD0 && c <= VK_DIVIDE);
}

bool IsSymbolKey(UCHAR c)
{
	return ((c >= VK_OEM_1 && c <= VK_OEM_3) || (c >= VK_OEM_4 && c <= VK_OEM_7));
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

void CleanupLogs()
{
	static char* cleanup_msg = "Log started";

	_LogToFileTrunc(DEFAULT_LOG_NAME, cleanup_msg);
	_LogToFileTrunc(DEFAULT_GUI_LOG_NAME, cleanup_msg);
	_LogToFileTrunc(DEFAULT_OSL_LOG_NAME, cleanup_msg);
}

void EndupLogs()
{
	static char* endup_msg = "Finished logging";

	_LogToFile(DEFAULT_LOG_NAME, endup_msg);
	_LogToFile(DEFAULT_GUI_LOG_NAME, endup_msg);
	_LogToFile(DEFAULT_OSL_LOG_NAME, endup_msg);
}
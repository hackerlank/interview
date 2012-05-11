#include "stdafx.h"
#include "misc.h"

string NameFromPath(string path)
{
	string ret = "";

	bool done = false; // костыль
	for(int i = path.length(); i > 0; i--)
	{
		if(path[i] == '.')
		{
			for(int j = i-1; path[j] != 92; j--)
				ret.push_back(path[j]);

			done = true;
			break;
		}
		if(done) break;
	}

	for(unsigned int z = 0; z < ret.length() / 2; z++)
	{
		char temp;

		temp = ret[z];
		ret[z] = ret[ret.length()-z-1];
		ret[ret.length()-z-1] = temp;
	}

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
	return Random(till) + from;
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
}

string IntToString(int intValue)
{ 
	char *myBuff; 
	string strRetVal;

	// Create a new char array 
	myBuff = new char[100]; 

	// Set it to empty 
	memset(myBuff,'\0',100); 

	// Convert to string 
	_itoa(intValue,myBuff,10); 

	// Copy the buffer into the string object 
	strRetVal = myBuff; 
   
	// Delete the buffer 
	delete[] myBuff; 

	return(strRetVal); 
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
	time_t current_time;
	string time_stamp;
	ofstream f;
	int openmode;


	time(&current_time);

	time_stamp = string(ctime(&current_time));
	time_stamp = string(time_stamp,11,8) + ' ';

	// TODO: FIX IT

	// shitty code
	static bool f_main_log = true;
	static bool f_interpret_log = true;
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

	if(f_interpret_log)
	{
		if(strcmp(file_name, DEFAULT_INTERPRET_LOG_NAME) == 0)
		{
			openmode = ios::out | ios::trunc;
			f_interpret_log = false;
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

	//if(count == 1)
	//	openmode = ios::out | ios::trunc;
	//else
		//openmode = ios::out | ios::app;

	string file = ".\\";
	file += string(DEFAULT_LOG_PATH);
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
		char buf[50];
		sprintf(buf, "Failed to open log-file: %s", file.c_str());
		MessageBox(NULL, buf, NULL, NULL);
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
	GetCurrentDirectory(255, path_app);

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

	// def
	strcpy(path_def, path_app);
	strcat(path_def, DEFAULT_DEFINITION_PATH);
	
	// scripts
	strcpy(path_scripts, path_app);
	strcat(path_scripts, DEFAULT_SCRIPTS_PATH);

}

void LogPaths(void)
{
	LogToFile("appl. path:\t" + string(path_app));
	LogToFile("log path:\t" + string(path_log));
	LogToFile("config path:\t" + string(path_config));
	LogToFile("textures path:\t" + string(path_textures));
	LogToFile("fonts path:\t" + string(path_fonts));
	LogToFile("defin. path:\t" + string(path_def));
	LogToFile("scripts path:\t" + string(path_scripts));
}

void ShowNumber(int n)
{
	char* s = new char[10];
	sprintf(s, "%u", n);
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
	char output[100];
	sprintf(output, "clearing string: %s", s.c_str());
	LogToFile(DEFAULT_MISC_LOG_NAME, output);

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

	sprintf(output, "string cleared (spaces): %s", s.c_str());
	LogToFile(DEFAULT_MISC_LOG_NAME, output);

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
			return false;
	}
	return true;
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

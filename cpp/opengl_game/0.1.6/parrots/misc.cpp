#include "stdafx.h"
#include "misc.h"

string NameFromPath(string path)
{
	string ret = "";

	bool done = false; // �������
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

	//MessageBox(NULL, ret.c_str(), "TEST", NULL);

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

	//ShowNumber(path.length());
	for(int i = path.length(); i > 0; i--)
		if(path[i] == '.')
		{
			for(unsigned int j = i+1; j < path.length(); j++)
				ret.push_back(path[j]);
			return ret;
		}


	//ShowNumber(i);

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

bool DirectoryExists(const char path[])
{
	if(GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES)
		return true;
	else
		return false;
}

bool FileExists(const char file_name[])
{
	if(GetFileAttributes(file_name) != INVALID_FILE_ATTRIBUTES)
		return true;
	else
		return false;
}

bool _LogToFile(char file_name[], char* message)
{
	time_t current_time;
	string time_stamp;
	ofstream f;
	static int count;
	int openmode;

	count++;

	time(&current_time);

	time_stamp = string(ctime(&current_time));
	time_stamp = string(time_stamp,11,8) + ' ';

	if(count == 1)
		openmode = ios::out | ios::trunc;
	else
		openmode = ios::out | ios::app;

	f.open(file_name, openmode);
	if(f.is_open())
	{
		f << time_stamp <<  message << endl;
		f.close();
		return true;
	}
	return false;
}

bool LogToFile(char file_name[], string message)
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
	strcat(path_log, "\\");
	strcat(path_log, DEFAULT_LOG_NAME);

	// config
	strcpy(path_config, path_app);
	strcat(path_config, "\\");
	strcat(path_config, DEFAULT_CONFIG_NAME);

	// textures
	strcpy(path_textures, path_app);
	strcat(path_textures, DEFAULT_TEXTURES_PATH);

	// fonts
	strcpy(path_fonts, path_app);
	strcat(path_fonts, DEFAULT_FONTS_PATH);

}

void LogPaths(void)
{
	LogToFile("appl. path:\t" + string(path_app));
	LogToFile("log path:\t" + string(path_log));
	LogToFile("config path:\t" + string(path_config));
	LogToFile("textures path:\t" + string(path_textures));
	LogToFile("fonts path:\t" + string(path_fonts));
}

void ShowNumber(int n)
{
	char buf[100];
	
	_itoa(n, buf, 10);

	MessageBox(NULL, buf, "ShowNumber() Call", NULL);
}

bool PointInBox(int px, int py, int x1, int y1, int width, int height)
{

	//if(px >= x1)
	//	if(py >= y1)
	//		if(px <= x2)
	//			if(py <= y3)
	//				return true;
	//			else
	//				return false;
	//		else
	//			return false;
	//	else
	//		return false;
	//else
	//	return false;

	if(px >= x1 && px <= x1 + width && py >= y1 && py <= y1 + height)
		return true;
	else
		return false;
}

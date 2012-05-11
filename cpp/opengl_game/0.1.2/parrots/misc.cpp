#include "stdafx.h"
#include "misc.h"
#include <math.h>


	// 12897
	// 12897 % 10 = 7
	// 12897 % 7
	//   12897 % 100 = 97
	//   12897 % ( 97 % 10)
	//		12897 % 1000 = 897
	//		12897 % ((897 % 100) % 10)
	//			12897 % 10000 = 2897
	//			12897 % (((2897 % 1000) % 100) % 10)


	// 444
// 444
// 444 % 10 = 4
// 444 / 4
	// 444 % 100 = 44
	// 444 / (44 / 10)


	// 4
	// 4 %

// 1222
// 1222 % 10 = 2
// 1222 % 2
	// 1222 % 100 = 22
	// 1222 % (22 % 10)
		// 1222 % 1000 = 222
		// 1222 % (222 % 100)

// 123
// 123 % 10 = 3
// 123 % 3
	//123 % 100 = 23
	//123 % (23 / 10)

// 12
// 12 % 10 = 2
// 12 % 2
	// 12 %

bool asdf(int n, int base)
{
	int ost = n % _Pow_int(10, base);
	bool temp = false;

	if(n < 10)
		return true;

		if(base > 1)
			temp = (n % ost == 0);
		else
			temp = (n % (ost / _Pow_int(10, base-1)) == 0);



	if(!temp)
		return false;
	else
	{
		if(n / _Pow_int(10, base) >= 10)
			return asdf(n, base + 1);
		else
			return true;
	}
}









		

	


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

	for(int z = 0; z < ret.length() / 2; z++)
	{
		char temp;

		temp = ret[z];
		ret[z] = ret[ret.length()-z-1];
		ret[ret.length()-z-1] = temp;
	}

	//MessageBox(NULL, ret.c_str(), "TEST", NULL);

	return ret;
}

string ExtFromPath(string path)
{
	string ret = "";

	//ShowNumber(path.length());
	for(int i = path.length(); i > 0; i--)
		if(path[i] == '.')
		{
			for(int j = i+1; j < path.length(); j++)
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
	itoa(intValue,myBuff,10); 

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
	
	itoa(n, buf, 10);

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

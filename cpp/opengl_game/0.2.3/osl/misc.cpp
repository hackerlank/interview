#include "stdafx.h"
#include "misc.h"

bool WriteToLog(char* file_name, string message)
{
	time_t current_time;
	string time_stamp;
	ofstream f;
	int openmode;
	static int count = 0;


	time(&current_time);

	time_stamp = string(ctime(&current_time));
	time_stamp = string(time_stamp,11,8) + ' ';

	count++;


	if(count == 1)
		openmode = ios::out | ios::trunc;
	else
		openmode = ios::out | ios::app;

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

void ShowNumber(int n)
{
	char* s = new char[10];
	sprintf(s, "%u", n);
	MessageBox(NULL, s, "ShowNumber() Call", NULL);
}
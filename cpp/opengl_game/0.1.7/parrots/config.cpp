#include "stdafx.h"
#include "config.h"
#include "misc.h"

bool config::LoadFromFile(char file_name[])
{
	int ok = 0;
	string s;
	string s_left;
	string s_right;
	int eq_pos = -1;
	int lines = 0;
	int params = 0;
	string error;
	

	ifstream cfg;

	cfg.open(file_name, ios::in);

	if(!cfg) return false;

	while(!cfg.eof())
	{
		cfg >> s;
		lines++;
		DeleteSpaces(s);
		DeleteComments(s);

		// find '='
		eq_pos = s.find('=');
		
		// if no '=' then error
		if(eq_pos == s.npos)
		{
			error = string(" [line" + IntToString(lines) + "] : '=' expected");
			MessageBox(NULL, error.c_str(), "Error reading config", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
		else
		{
			s_left = s.substr(0, eq_pos);
			s_right = s.substr(eq_pos + 1, s.length() - 1);

			if(s_left.empty() || s_right.empty())
			{
				error = string(" [line" + IntToString(lines) + "] : Wrong string");
				MessageBox(NULL, error.c_str(), "Error reading config", MB_OK | MB_ICONEXCLAMATION);
				return false;
			}
			else
			{
				if(s_left == "scr_width") scr_width = atoi(s_right.c_str());
				if(s_left == "scr_height") scr_height = atoi(s_right.c_str());
				if(s_left == "bpp") scr_bpp = atoi(s_right.c_str());
				if(s_left == "fullscreen") fullscreen = atoi(s_right.c_str()) !=0;
				if(s_left == "vert_sync") vert_sync = atoi(s_right.c_str()) !=0;
				if(s_left == "debug") debug = atoi(s_right.c_str()) !=0;
		
			};
					
		}
	}
	return true;
}

bool config::SaveToFile(char file_name[])
{
	ofstream cfg;

	cfg.open(file_name, ios::out | ios::trunc);

	if(!cfg) return false;

	cfg << "scr_width=" << scr_width;
	cfg << "scr_height=" << scr_height;
	cfg << "scr_bpp=" << scr_bpp;
	cfg << "fullscreen=" << fullscreen;
	cfg << "vert_sync=" << vert_sync;

	return cfg.fail();
}
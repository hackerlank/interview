#include "stdafx.h"
#include "loader.h"
#include "misc.h"

void RemoveAllSpaces(string from)
{
	int i = 0;
	while(i < from.length())
	{
		if(from[i] == ' ')
		{
			if(i == 0)
				from.erase(i, 1);
			if(i == from.length())
				from.erase(i, 1);

			if(from[i+1] == ' ')
				from.erase(i, 1);

			i--;
		}
	}
}



string GetWord(string from, int number, char cancel_symbol)
{
	string ret;
	int c = 0;
	bool done = false;
	//for(std::string::iterator i = from.begin(); i != from.end(); i++)
	for(int i = 0; i < from.length(); i++)
	{
		c++;

		int in = 0;
		int in_pos = 0;
		int in_pos_prev = 0;

		if(from[i] == cancel_symbol)
		{
			in++;
			in_pos_prev = in_pos;
			in_pos = c;

			if(in == number)
			{
				ret = from.substr(in_pos_prev, in_pos - in_pos_prev);
				done = true;
				break;
			}
			else
			{
				if(in == number - 1)
				{
					ret = from.substr(in_pos, from.length() - in_pos);
					done = true;
					break;
				}
			}
		}
		if(done) break;
	}
	return ret;
}

// TODO: 

//bool Load(LPSTR* filename)
//{
//	int current_state = 0;
//	ifstream f;
//	string s;
//
//	if(!FileExists(filename))
//	{
//		LogToFile("File " + string(filename) + " does not exist");
//		return false;
//	}
//	else
//	{
//		f.open(filename, ios::in);
//
//		while(f >> s)
//		{
//			RemoveAllSpaces(s);
//
//			string first_token;
//			string second_token;
//
//			first_token = GetWord(s, 1, ' ');
//
//			 now we got main token
//
//			if(f*/irst_token == "MENU")
//			{





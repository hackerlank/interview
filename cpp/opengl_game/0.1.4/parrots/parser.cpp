#include "stdafx.h"
#include "parser.h"

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




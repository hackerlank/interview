#include "stdafx.h"
#include "iichan_briefing.h"
#include <io.h>


bool LoadBriefing(const char* file_name, Briefing* br)
{
	if(!br)
		return false;

	FILE* f = NULL;
	f = fopen(file_name, "rb");
	if(f)
	{
		int i = 0;
		int lc = 0;

		int max_i = br->max_line_length;
		int space = 0;

		char* str = new char[max_i + 1];
		assert(str != NULL);

		br->line_count = 0;
		while(!feof(f))
		{
			char buf;
			int p;

			i++;
			fread(&buf, 1, sizeof(buf), f);
			p = ftell(f);

			if(buf == ' ')
				space = i;


			if(i == max_i || buf == '\n' || feof(f))
			{
				if(i == max_i)
				{
					fseek(f, p - i, SEEK_SET);
					memset(str, '\0', max_i + 1);
					fread(str, sizeof(char), space, f);
					string s = string(str);
					br->lines.push_back(s);
					br->line_count++;
					i = 0;
					space = 0;
				}
				else
				{
					if(buf == '\n' && i > 1)
					{
						fseek(f, p - i, SEEK_SET);
						memset(str, '\0', max_i + 1);
						fread(str, sizeof(char), i-2, f);
						string s = string(str);
						br->lines.push_back(s);
						br->line_count++;
						i = -2;
						space = 0;
					}
				}
			}
		}
		delete[] str;
		fclose(f);
		return true;
	}
	else
		return false;
}
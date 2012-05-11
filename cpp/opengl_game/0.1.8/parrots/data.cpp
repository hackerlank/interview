#include "stdafx.h"
#include "misc.h"
#include "data.h"

void GUIDEFINITION::Parse(GUI_DEFINITION_ELEM* it, string right, string value)
{
	if(right == "VISIBLE")
		it->visible = atoi(value.c_str()) !=0;
	else if(right == "TEXTURE_NAME")
		strcpy(it->texture_name, value.c_str());
	else if(right == "PATTERNCOUNT")
		it->pattern_count = atoi(value.c_str());
	else if(right == "PATTERNWIDTH")
		it->pattern_width = atoi(value.c_str());
	else if(right == "PATTERNCURRENT")
		it->pattern_current = atoi(value.c_str());
	else if(right == "PATTERNHEIGHT")
		it->pattern_height = atoi(value.c_str());
	else if(right == "X")
		it->x = atoi(value.c_str());
	else if(right == "Y")
		it->y = atoi(value.c_str());
	else if(right == "COLOR_RED")
		it->color_red = atoi(value.c_str());
	else if(right == "COLOR_GREEN")
		it->color_green = atoi(value.c_str());
	else if(right == "COLOR_BLUE")
		it->color_blue = atoi(value.c_str());
	else if(right == "COLOR_ALPHA")
		it->color_alpha = atoi(value.c_str());
	else if(right == "Y")
		it->y = atoi(value.c_str());
	else if(right == "CAPTION_FONT_NAME")
		strcpy(it->caption_font_name, value.c_str());
	else if(right == "CAPTION_X")
		it->caption_x = atoi(value.c_str());
	else if(right == "CAPTION_Y")
		it->caption_y = atoi(value.c_str());
	else if(right == "CAPTION_COLOR_RED")
		it->caption_color_red = atoi(value.c_str());
	else if(right == "CAPTION_COLOR_GREEN")
		it->caption_color_green = atoi(value.c_str());
	else if(right == "CAPTION_COLOR_BLUE")
		it->caption_color_blue = atoi(value.c_str());
	else if(right == "CAPTION_COLOR_ALPHA")
		it->caption_color_alpha = atoi(value.c_str());
	else if(right == "CAPTION")
		strcpy(it->caption, value.c_str());
	else if(right == "PARENT")
		strcpy(it->parent, value.c_str());
	else if(right == "ACTION")
		it->action = ACTION(atoi(value.c_str()));
	else if(right == "DATA")
		it->data = atoi(value.c_str());
	else if(right == "NEXT")
	{
		it->next = new char[value.length()];
		strcpy(it->next, value.c_str());
	}
	else
		{
			char output[50];
			sprintf(output, "File: %s, Line: %u - unrecognized line, ignored");
			LogToFile(output);
		}
}


bool GUIDEFINITION::LoadFromFile(char* file_name)
{
	ifstream f;
	char output[50];

	f.open(file_name, ios::in);

	if(!f.is_open())
	{
		sprintf(output, "Error opening file %s", file_name);
		LogToFile(output);
		return false;
	}
	else
	{
		sprintf(output, "Loading file %s", file_name);
		LogToFile(output);

		int lines = 0;

		GUI_DEFINITION_ELEM* elem = NULL;

		while(!f.eof())
		{
			string s;		

			f >> s;

			lines++;

			if(s.empty())
				continue;

#ifdef _DEBUG_LOAD_GUI
			sprintf(output, "string: %s", s.c_str());
			LogToFile(output);
#endif;



			int dot_pos = 0;

			dot_pos = s.find('.');

			if(dot_pos == s.npos)
			{
				sprintf(output, "File: %s, Line: %u - invalid line ('.' expected)", file_name, lines);
				LogToFile(output);
				return false;
			}
			else
			{
				int eq_pos = 0;
				eq_pos = s.find('=');
				if(eq_pos == s.npos)
				{
					sprintf(output, "File: %s, Line: %u - invalid line ('=' expected)", file_name, lines);
					LogToFile(output);
					return false;
				}
				else
				{
					string left;
					string right;
					string value;
					left = s.substr(0, dot_pos);
					right = s.substr(dot_pos + 1, s.length() - dot_pos  - (s.length() - eq_pos) - 1);
					value = s.substr(eq_pos + 1, s.length() - 1);
#ifdef _DEBUG_LOAD_GUI
					LogToFile("left: " + left);
					LogToFile("right: " + right);
					LogToFile("value: " + value);
#endif

					int i = -1;
					if(elements.size() > 0)
					{
						bool found = false;
						for(std::vector<GUI_DEFINITION_ELEM>::iterator it = elements.begin(); it != elements.end(); it++)
						{
							i++;
							if(it->name == left)
							{
								Parse(&elements[i], right, value);
								found = true;
								break;
							}
						}
						if(!found)
						{
							elem = new GUI_DEFINITION_ELEM();
							strcpy(elem->name,  left.c_str());
							elements.push_back(*elem);
							Parse(&elements[elements.size()-1], right, value);
						}
					}
					else
					{
							elem = new GUI_DEFINITION_ELEM();
							strcpy(elem->name,  left.c_str());
							elements.push_back(*elem);
							Parse(&elements[0], right, value);
					}
				}
			
			}
		}
	}
}
			



		
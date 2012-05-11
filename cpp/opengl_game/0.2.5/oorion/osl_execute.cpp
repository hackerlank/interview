#include "stdafx.h"
#include "font.h"
#include "texture.h"
#include "misc.h"
#include "config.h"
#include "draw.h"
#include "gui.h"
#include "osl_lexems.h"
#include "osl_misc.h"

#include "osl.h"

extern GUI* gui;
extern config cfg;
extern unsigned int current_state;

void DoEcho(char* data);
void DoExit(void);
void DoLog(string file, string data);


void osl_ExecuteSystemFunction(vector<LEXEM>* list)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Executing system function...");

	unsigned int param_count = 1;

	for(vector<LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		LEXEM lx = *it;

		if(osl_IsComma(lx.token))
			param_count++;
	}

	vector<LEXEM>::iterator i_first = list->begin();
	LEXEM first = *i_first;

	if(first.token == "echo")
	{
		vector<LEXEM>::iterator i_second = list->begin()+2;
		LEXEM second = *i_second;

		char* message = new char[second.token.length()+1];
		strcpy(message, (char*)second.token.data());

		message = osl_RemoveQuotesChr(message);


		DoEcho(message);
	}
	else if(first.token == "exit")
	{
		DoExit();
	}
	else if(first.token == "init")
	{
					vector<LEXEM>::iterator i_second = list->begin()+2;
					LEXEM second = *i_second;

					if(second.token == "gui")
					{
						if(!gui)
							gui = new GUI();
						else
							osl_Error("GUI already initialized");
					}
	}
	else if(first.token == "loadconfig")
	{
		vector<LEXEM>::iterator i_second = list->begin()+2;
		LEXEM second = *i_second;

		if(osl_IsStrConstant(second.token))
		{
			char* file = new char[second.token.length()+1];
			strcpy(file, (char*)second.token.data());
			file = osl_RemoveQuotesChr(file);

			LoadConfig(file, &cfg);
		}
		else
			osl_Error("LoadConfig(): Invalid argument");
	}
	else if(first.token == "loadfont")
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to LoadFont...");
		if(param_count == 4)
		{
			//LoadFont("Times New Roman", 14, FW_BOLD, "debug");
			vector<LEXEM>::iterator i_second = list->begin()+2;
			LEXEM second = *i_second;
			vector<LEXEM>::iterator i_third = list->begin()+4;
			LEXEM third = *i_third;
			vector<LEXEM>::iterator i_fourth = list->begin()+6;
			LEXEM fourth = *i_fourth;
			vector<LEXEM>::iterator i_fifth = list->begin()+8;
			LEXEM fifth = *i_fifth;

			try
			{
				LoadFont((char*)osl_RemoveQuotesStr(second.token).c_str(),
					osl_StrToInt(third.token),
					osl_StrToInt(fourth.token),
					(char*)osl_RemoveQuotesStr(fifth.token).c_str());
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadFont");
			}
			WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadFont OK");

		}
		else
			osl_Error("New() error: function requires two parametres");
	}

}


void DoEcho(char* data)
{
	if(gui)
		gui->LogOnScreen(data);
	else
		osl_Error("Echo() error: gui not initialized");
}

void DoExit(void)
{
	current_state = GAME_EXITING;
}

void DoLog(string file, string data);
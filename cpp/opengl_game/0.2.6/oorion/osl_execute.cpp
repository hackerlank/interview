#include "stdafx.h"
#include "font.h"
#include "texture.h"
#include "misc.h"
#include "config.h"
#include "draw.h"
#include "gui.h"
#include "osl_lexems.h"
#include "osl_misc.h"

#include "osl_makegui.h"

#include "scene.h"
#include "osl.h"

#include "game.h"


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
			ApplyConfig(&cfg);
		}
		else
			osl_Error("LoadConfig(): Invalid argument");
	}
	else if(first.token == "loadfont")
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to load font...");
		if(param_count == 4)
		{
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
			osl_Error("LoadFont() error: function requires two parametres");
	}
	else if(first.token == "loadtexture")
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to load texture...");
		if(param_count == 1)
		{
			vector<LEXEM>::iterator i_second = list->begin()+2;
			LEXEM second = *i_second;

			try
			{
				string s = osl_RemoveQuotesStr(second.token);
				char* n = new char[strlen(s.data())+1];
				sprintf(n, "%s\0", s.data());

				if(!LoadTexture(n))
					osl_Error("LoadTexture() failed");
				delete[] n;
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadTexture");
			}
			WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadTexture OK");
		}
		else
			osl_Error("LoadTexture() error: function requires one parameter");
	}
	else if(first.token == "newguielem")
	{
		if(param_count == 2)
		{
			vector<LEXEM>::iterator i_second = list->begin()+2;
			LEXEM second = *i_second;
			vector<LEXEM>::iterator i_third = list->begin()+4;
			LEXEM third = *i_third;

			NewGUIElem(osl_RemoveQuotesStr(second.token),
				osl_RemoveQuotesStr(third.token));
		}
		else
			osl_Error("NewGUIElem() error: function requires two parametres");
	}
	else if(first.token == "setguielem")
	{
		if(param_count == 3)
		{
			vector<LEXEM>::iterator i_second = list->begin()+2;
			LEXEM second = *i_second;
			vector<LEXEM>::iterator i_third = list->begin()+4;
			LEXEM third = *i_third;
			vector<LEXEM>::iterator i_fourth = list->begin()+6;
			LEXEM fourth = *i_fourth;

			SetGUIElem(osl_RemoveQuotesStr(second.token),
				osl_RemoveQuotesStr(third.token),
				osl_RemoveQuotesStr(fourth.token));
		}
		else if(param_count == 6)
		{
			vector<LEXEM>::iterator i_second = list->begin()+2;
			LEXEM second = *i_second;
			vector<LEXEM>::iterator i_third = list->begin()+4;
			LEXEM third = *i_third;
			vector<LEXEM>::iterator i_fourth = list->begin()+6;
			LEXEM fourth = *i_fourth;
			vector<LEXEM>::iterator i_fifth = list->begin()+8;
			LEXEM fifth = *i_fifth;
			vector<LEXEM>::iterator i_sixth = list->begin()+10;
			LEXEM sixth = *i_sixth;
			vector<LEXEM>::iterator i_seventh = list->begin()+12;
			LEXEM seventh = *i_seventh;

			SetGUIElem(osl_RemoveQuotesStr(second.token),
				osl_RemoveQuotesStr(third.token),
				osl_RemoveQuotesStr(fourth.token),
				osl_RemoveQuotesStr(fifth.token),
				osl_RemoveQuotesStr(sixth.token),
				osl_RemoveQuotesStr(seventh.token));
		}
		else
			osl_Error("NewGUIElem() error: incorrect number of parametres");
	}
	else
		osl_Error("Command is not implemented yet");


	
			




//NewGUIElem("GUIButton", "btnStartGame");
//SetGUIElem("btnStartGame", "Width", 220);
//SetGUIElem("btnStartGame", "Height", 30);
//SetGUIElem("btnStartGame", "X", 400);
//SetGUIElem("btnStartGame", "Y", 400);
//SetGUIElem("btnStartGame", "Color", 255, 255, 255, 255);
//SetGUIElem("btnStartGame", "Texture", "button_long");
//SetGUIElem("btnStartGame", "Visible", 1);
//SetGUIElem("btnStartGame", "Caption", "Start game");
//SetGUIElem("btnStartGame", "CaptionFont", "debug");
//SetGUIElem("btnStartGame", "CaptionColor", 255, 255, 255, 255);
//SetGUIElem("btnStartGame", "PatternCount", 3);
//SetGUIElem("btnStartGame", "CurrentPattern", 0);
//SetGUIElem("btnStartGame", "Data", 550);


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
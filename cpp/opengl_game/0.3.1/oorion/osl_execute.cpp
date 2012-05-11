#include "stdafx.h"
#include "font.h"
#include "texture.h"
#include "misc.h"
#include "config.h"
#include "draw.h"
#include "gui.h"
#include "osl_types.h"
#include "osl_misc.h"
#include "osl_lexems.h"

#include "osl_makegui.h"
#include "osl_makeiichan.h"

#include "scene.h"
#include "osl.h"

#include "game.h"


extern GUI* gui;
extern config cfg;
extern unsigned int current_state;

void DoEcho(char* data);
void DoExit(void);
void DoLog(string file, string data);
bool DoLoad(string file);


#define OSL_EXEC_OK			"1"
#define OSL_EXEC_FAIL		"0"







string osl_ExecuteSystemFunction(vector<OSL_LEXEM>* list)
{
	string _result;
	_result.resize(0);

	#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Executing system function...");
	#endif

	unsigned int param_count = 1;

	for(vector<OSL_LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		OSL_LEXEM lx = *it;

		if(osl_IsComma(lx.token))
			param_count++;
	}

	vector<OSL_LEXEM>::iterator i_first = list->begin();
	OSL_LEXEM first = *i_first;

	if(first.token == "echo")
	{
		vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
		OSL_LEXEM second = *i_second;

		char* message = new char[second.token.length()+1];
		strcpy(message, (char*)second.token.data());

		message = osl_RemoveQuotesChr(message);
		DoEcho(message);
	}
	else if(first.token == "load")
	{
		vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
		OSL_LEXEM second = *i_second;

		bool ok = DoLoad(osl_RemoveQuotesStr(second.token));

		ok == true ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}
	else if(first.token == "exit")
	{
		DoExit();
	}
	else if(first.token == "init")
	{
					vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
					OSL_LEXEM second = *i_second;

					if(second.token == "gui")
					{
						if(!gui)
						{
							gui = new GUI();
							_result = OSL_EXEC_OK;
						}
						else
						{
							osl_Error("GUI already initialized");
							_result = OSL_EXEC_FAIL;
						}
					}
					else
					{
						osl_Error("init(): unknown parameter");
						_result = OSL_EXEC_FAIL;
					}
	}
	else if(first.token == "loadconfig")
	{
		vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
		OSL_LEXEM second = *i_second;

		if(osl_IsStrConstant(second.token))
		{
			char* file = new char[second.token.length()+1];
			strcpy(file, (char*)second.token.data());
			file = osl_RemoveQuotesChr(file);

			bool ok = LoadConfig(file, &cfg);

			if(ok)
			{
				ApplyConfig(&cfg);
				_result = OSL_EXEC_OK;
			}
			else
				_result = OSL_EXEC_FAIL;
		}
		else
			osl_Error("LoadConfig(): Invalid argument");
	}
	else if(first.token == "loadfontfromfile")
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to load font from file...");

		if(param_count == 3)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;
			vector<OSL_LEXEM>::iterator i_third = list->begin()+4;
			OSL_LEXEM third = *i_third;
			vector<OSL_LEXEM>::iterator i_fourth = list->begin()+6;
			OSL_LEXEM fourth = *i_fourth;

			if(osl_IsIntConstant(third.token) &&
				osl_IsIntConstant(fourth.token))
			{
				try
				{
					if(LoadFontFromFile((char*)osl_RemoveQuotesStr(second.token).c_str(),
						osl_StrToInt(third.token),
						osl_StrToInt(fourth.token)))
					{
						WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadFontFromFile OK");
						_result = OSL_EXEC_OK;
					}
					else
					{
						WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadFontFromFile FAILED");
						_result = OSL_EXEC_FAIL;
					}
				}
				catch(...)
				{
					osl_Error("Exception in try block: LoadFontFromFile");
					_result = OSL_EXEC_FAIL;
				}			
			}
			else
			{
				osl_Error("LoadFontFromFile() error: incompatible types");
				_result = OSL_EXEC_FAIL;
			}
		}
		else
		{
			osl_Error("LoadFontFromFile() error: function require one parameter");
			_result = OSL_EXEC_FAIL;
		}
	}
	else if(first.token == "loadfont")
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to load font...");
		if(param_count == 4)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;
			vector<OSL_LEXEM>::iterator i_third = list->begin()+4;
			OSL_LEXEM third = *i_third;
			vector<OSL_LEXEM>::iterator i_fourth = list->begin()+6;
			OSL_LEXEM fourth = *i_fourth;
			vector<OSL_LEXEM>::iterator i_fifth = list->begin()+8;
			OSL_LEXEM fifth = *i_fifth;

			try
			{
				bool ok =
					LoadFont((char*)osl_RemoveQuotesStr(second.token).c_str(),
						osl_StrToInt(third.token),
						osl_StrToInt(fourth.token),
						(char*)osl_RemoveQuotesStr(fifth.token).c_str());

				ok == true ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadFont");
				_result = OSL_EXEC_FAIL;
			}
			WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadFont OK");

		}
		else
		{
			osl_Error("LoadFont() error: function requires two parametres");
			_result = OSL_EXEC_FAIL;
		}
	}
	else if(first.token == "loadtexture")
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to load texture...");
		if(param_count == 1)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;

			try
			{
				string s = osl_RemoveQuotesStr(second.token);
				char* n = new char[strlen(s.data())+1];
				sprintf(n, "%s\0", s.data());

				if(!LoadTexture(n))
				{
					osl_Error("LoadTexture() failed");
					_result = OSL_EXEC_FAIL;
				}
				else
					_result = OSL_EXEC_OK;
				delete[] n;
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadTexture");
				_result = OSL_EXEC_FAIL;
			}
			WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadTexture OK");
		}
		else
		{
			osl_Error("LoadTexture() error: function requires one parameter");
			_result = OSL_EXEC_FAIL;
		}
	}
	else if(first.token == "newguielem")
	{
		if(param_count == 2)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;
			vector<OSL_LEXEM>::iterator i_third = list->begin()+4;
			OSL_LEXEM third = *i_third;

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
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;
			vector<OSL_LEXEM>::iterator i_third = list->begin()+4;
			OSL_LEXEM third = *i_third;
			vector<OSL_LEXEM>::iterator i_fourth = list->begin()+6;
			OSL_LEXEM fourth = *i_fourth;

			SetGUIElem(osl_RemoveQuotesStr(second.token),
				osl_RemoveQuotesStr(third.token),
				osl_RemoveQuotesStr(fourth.token));
		}
		else if(param_count == 5)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;
			vector<OSL_LEXEM>::iterator i_third = list->begin()+4;
			OSL_LEXEM third = *i_third;
			vector<OSL_LEXEM>::iterator i_fourth = list->begin()+6;
			OSL_LEXEM fourth = *i_fourth;
			vector<OSL_LEXEM>::iterator i_fifth = list->begin()+8;
			OSL_LEXEM fifth = *i_fifth;
			vector<OSL_LEXEM>::iterator i_sixth = list->begin()+10;
			OSL_LEXEM sixth = *i_sixth;

			SetGUIElem(osl_RemoveQuotesStr(second.token),
				osl_RemoveQuotesStr(third.token),
				osl_RemoveQuotesStr(fourth.token),
				osl_RemoveQuotesStr(fifth.token),
				osl_RemoveQuotesStr(sixth.token));
		}
		else if(param_count == 6)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;
			vector<OSL_LEXEM>::iterator i_third = list->begin()+4;
			OSL_LEXEM third = *i_third;
			vector<OSL_LEXEM>::iterator i_fourth = list->begin()+6;
			OSL_LEXEM fourth = *i_fourth;
			vector<OSL_LEXEM>::iterator i_fifth = list->begin()+8;
			OSL_LEXEM fifth = *i_fifth;
			vector<OSL_LEXEM>::iterator i_sixth = list->begin()+10;
			OSL_LEXEM sixth = *i_sixth;
			vector<OSL_LEXEM>::iterator i_seventh = list->begin()+12;
			OSL_LEXEM seventh = *i_seventh;

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
	else if(first.token == "createplayer")
	{
		if(param_count == 28)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			vector<OSL_LEXEM>::iterator p4 = list->begin()+8;
			vector<OSL_LEXEM>::iterator p5 = list->begin()+10;
			vector<OSL_LEXEM>::iterator p6 = list->begin()+12;
			vector<OSL_LEXEM>::iterator p7 = list->begin()+14;
			vector<OSL_LEXEM>::iterator p8 = list->begin()+16;
			vector<OSL_LEXEM>::iterator p9 = list->begin()+18;
			vector<OSL_LEXEM>::iterator p10 = list->begin()+20;
			vector<OSL_LEXEM>::iterator p11 = list->begin()+22;
			vector<OSL_LEXEM>::iterator p12 = list->begin()+24;
			vector<OSL_LEXEM>::iterator p13 = list->begin()+26;
			vector<OSL_LEXEM>::iterator p14 = list->begin()+28;
			vector<OSL_LEXEM>::iterator p15 = list->begin()+30;
			vector<OSL_LEXEM>::iterator p16 = list->begin()+32;
			vector<OSL_LEXEM>::iterator p17 = list->begin()+34;
			vector<OSL_LEXEM>::iterator p18 = list->begin()+36;
			vector<OSL_LEXEM>::iterator p19 = list->begin()+38;
			vector<OSL_LEXEM>::iterator p20 = list->begin()+40;
			vector<OSL_LEXEM>::iterator p21 = list->begin()+42;
			vector<OSL_LEXEM>::iterator p22 = list->begin()+44;
			vector<OSL_LEXEM>::iterator p23 = list->begin()+46;
			vector<OSL_LEXEM>::iterator p24 = list->begin()+48;
			vector<OSL_LEXEM>::iterator p25 = list->begin()+50;
			vector<OSL_LEXEM>::iterator p26 = list->begin()+52;
			vector<OSL_LEXEM>::iterator p27 = list->begin()+54;
			vector<OSL_LEXEM>::iterator p28 = list->begin()+56;

			CreatePlayer((*p1).token, (*p2).token, (*p3).token, (*p4).token,
				(*p5).token, (*p6).token, (*p7).token, (*p8).token, (*p9).token,
				(*p10).token, (*p11).token, (*p12).token, (*p13).token, (*p14).token,
				(*p15).token, (*p16).token, (*p17).token, (*p18).token, (*p19).token,
				(*p20).token, (*p21).token, (*p22).token, (*p23).token, (*p24).token,
				(*p25).token, (*p26).token, (*p27).token, (*p28).token);


		}
		else
			osl_Error("CreatePlayer() error: incorrect number of parametres");
	}
	else if(first.token == "createpowerup")
	{
		if(param_count == 4)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			vector<OSL_LEXEM>::iterator p4 = list->begin()+8;

			CreatePowerup((*p1).token, (*p2).token, (*p3).token, (*p4).token);

		}
		else
			osl_Error("CreatePowerup() error: incorrect number of parametres");
	}
	else if(first.token == "createenemy")
	{
		if(param_count == 3)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;

			CreateEnemy((*p1).token, (*p2).token, (*p3).token);
		}
		else
			osl_Error("CreateEnemy() error: incorrect number of parametres");
	}
	else
		osl_Error("Command is not implemented yet");

	return _result;

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

bool DoLoad(string file)
{
	return
		osl_Load_v_04(file.c_str());
}

void DoLog(string file, string data);
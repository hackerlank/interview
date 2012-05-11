#include "stdafx.h"
#include "../draw/font.h"
#include "../draw/texture.h"
#include "../misc.h"
#include "../config.h"
#include "../draw/draw.h"
#include "../gui/gui.h"
#include "osl_types.h"
#include "osl_misc.h"
#include "osl_lexems.h"

#include "osl_makegui.h"
#include "osl_makeiichan.h"

#include "../scene.h"
#include "osl.h"

#include "../iichan/iichan_resource.h"
#include "../iichan/iichan_dialog.h"

#include "../sound/snd.h"


extern GUI* gui;
extern SND* snd;
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
		else if(second.token == "sound")
		{
			if(!snd)
			{
				snd = new SND();
				_result = OSL_EXEC_OK;
			}
			else
			{
				osl_Error("SND already initialized");
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
			string s = osl_RemoveQuotesStr(second.token);
			
			WriteToLog(DEFAULT_LOG_NAME, "Loading config: " + s);

			bool ok = LoadConfig(s.c_str(), &cfg);

			if(ok)
			{
				ApplyConfig(&cfg);
				_result = OSL_EXEC_OK;
				
			}
			else
			{
				_result = OSL_EXEC_FAIL;
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load config");
			}
		}
		else
			osl_Error("LoadConfig(): Invalid argument");
	}
	else if(first.token == "loadsound")
	{
		if(param_count == 1)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;

			if(osl_IsStrConstant(second.token))
			{
				try
				{
					string s = osl_RemoveQuotesStr(second.token);
					WriteToLog(DEFAULT_LOG_NAME, "Loading sound: " + s);
					if(snd)
					{		
						if(snd->LoadSound(s))
						{
							_result = OSL_EXEC_OK;
						}
						else
						{
							osl_Error("LoadSound() failed");
							WriteToLog(DEFAULT_LOG_NAME, "Failed to load sound");
							_result = OSL_EXEC_FAIL;
						}
					}
					else
					{
						osl_Error("Sound not initialized");
						WriteToLog(DEFAULT_LOG_NAME, "Failed to load config");
						_result = OSL_EXEC_FAIL;
					}
				}
				catch(...)
				{
					osl_Error("Exception in try block: LoadSound");
					_result = OSL_EXEC_FAIL;
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load config");
				}
			}
		}
		else
		{
			osl_Error("LoadSound() error: function requires one parameter");
			_result = OSL_EXEC_FAIL;
		}
	}
	else if(first.token == "loaddialog")
	{
		vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
		OSL_LEXEM second = *i_second;

		if(osl_IsStrConstant(second.token))
		{
			string s = osl_RemoveQuotesStr(second.token);

			WriteToLog(DEFAULT_LOG_NAME, "Loading dialog: " + s);

			bool ok = LoadDialog(s.c_str());

			if(ok)
			{
				ApplyConfig(&cfg);
				_result = OSL_EXEC_OK;
			}
			else
			{
				_result = OSL_EXEC_FAIL;
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load dialog");
			}
		}
		else
			osl_Error("LoadConfig(): Invalid argument");
	}
	else if(first.token == "loadfontfromfile")
	{
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
					string s = osl_RemoveQuotesStr(second.token);
					WriteToLog(DEFAULT_LOG_NAME, "Loading font from file: " + s);
					if(LoadFontFromFile((char*)s.c_str(),
						osl_StrToInt(third.token),
						osl_StrToInt(fourth.token)))
					{
						_result = OSL_EXEC_OK;
					}
					else
					{
						WriteToLog(DEFAULT_LOG_NAME, "Failed to load font from file");
						_result = OSL_EXEC_FAIL;
					}
				}
				catch(...)
				{
					osl_Error("Exception in try block: LoadFontFromFile");
					_result = OSL_EXEC_FAIL;
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load font from file");
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
				string s = osl_RemoveQuotesStr(second.token);

				WriteToLog(DEFAULT_LOG_NAME, "Loading font: " + s);
				bool ok =
					LoadFont((char*)s.c_str(),
						osl_StrToInt(third.token),
						osl_StrToInt(fourth.token),
						(char*)osl_RemoveQuotesStr(fifth.token).c_str());

				if(ok)
				{
					_result = OSL_EXEC_OK;
				}
				else
				{
					_result = OSL_EXEC_FAIL;
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load font");
				}
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadFont");
				_result = OSL_EXEC_FAIL;
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load font");
			}
		}
		else
		{
			osl_Error("LoadFont() error: function requires two parametres");
			_result = OSL_EXEC_FAIL;
		}
	}
	else if(first.token == "loadtexture")
	{
		if(param_count == 1)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;

			try
			{
				string s = osl_RemoveQuotesStr(second.token);

				WriteToLog(DEFAULT_LOG_NAME, "Loading texture: " + s);

				if(!LoadTexture(s.c_str()))
				{
					osl_Error("LoadTexture() failed");
					_result = OSL_EXEC_FAIL;
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load texture");
				}
				else
					_result = OSL_EXEC_OK;
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadTexture");
				_result = OSL_EXEC_FAIL;
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load texture");
			}
		}
		else
		{
			osl_Error("LoadTexture() error: function requires one parameter");
			_result = OSL_EXEC_FAIL;
		}
	}
	else if(first.token == "loadresource")
	{
		if(param_count == 1)
		{
			vector<OSL_LEXEM>::iterator i_second = list->begin()+2;
			OSL_LEXEM second = *i_second;

			try
			{
				string s = osl_RemoveQuotesStr(second.token);

				WriteToLog(DEFAULT_LOG_NAME, "Loading resource: " + s);

				if(!LoadResource(s.c_str()))
				{
					osl_Error("LoadResource() failed");
					_result = OSL_EXEC_FAIL;
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load resource");
				}
				else
					_result = OSL_EXEC_OK;
			}
			catch(...)
			{
				osl_Error("Exception in try block: LoadResource");
				_result = OSL_EXEC_FAIL;
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load resource");
			}
		}
		else
		{
			osl_Error("LoadResource() error: function requires one parameter");
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
		if(param_count == 6)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			vector<OSL_LEXEM>::iterator p4 = list->begin()+8;
			vector<OSL_LEXEM>::iterator p5 = list->begin()+10;
			vector<OSL_LEXEM>::iterator p6 = list->begin()+12;

			CreatePlayer((*p1).token, (*p2).token, (*p3).token, (*p4).token,
				(*p5).token, (*p6).token);
		}
		else
			osl_Error("CreatePlayer() error: incorrect number of parametres");
	}
	else if(first.token == "createpowerup")
	{
		if(param_count == 5)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			vector<OSL_LEXEM>::iterator p4 = list->begin()+8;
			vector<OSL_LEXEM>::iterator p5 = list->begin()+10;

			CreatePowerup((*p1).token, (*p2).token, (*p3).token, (*p4).token, (*p5).token);

		}
		else
			osl_Error("CreatePowerup() error: incorrect number of parametres");
	}
	else if(first.token == "createevent")
	{
		if(param_count == 5)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			vector<OSL_LEXEM>::iterator p4 = list->begin()+8;
			vector<OSL_LEXEM>::iterator p5 = list->begin()+10;

			CreateEvent((*p1).token, (*p2).token, (*p3).token, (*p4).token, (*p5).token);
		}
		else
			osl_Error("CreateEvent() error: incorrect number of parametres");
	}
	else if(first.token == "createenemy")
	{
		if(param_count == 4)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			vector<OSL_LEXEM>::iterator p4 = list->begin()+8;

			CreateEnemy((*p1).token, (*p2).token, (*p3).token, (*p4).token);
		}
		else
			osl_Error("CreateEnemy() error: incorrect number of parametres");
	}
	else if(first.token == "createsprite")
	{
		if(param_count == 3)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			vector<OSL_LEXEM>::iterator p2 = list->begin()+4;
			vector<OSL_LEXEM>::iterator p3 = list->begin()+6;
			CreateSprite((*p1).token, (*p2).token, (*p3).token);
		}
		else
			osl_Error("CreateSprite() error: incorrect number of parametres");
	}
	else if(first.token == "setworldmaxx")
	{
		if(param_count == 1)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			SetWorldMaxX((*p1).token);
		}
		else
			osl_Error("SetWorldMaxX() error: incorrect number of parametres");
	}
	else if(first.token == "setworldminx")
	{
		if(param_count == 1)
		{
			vector<OSL_LEXEM>::iterator p1 = list->begin()+2;
			SetWorldMinX((*p1).token);
		}
		else
			osl_Error("SetWorldMinX() error: incorrect number of parametres");
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
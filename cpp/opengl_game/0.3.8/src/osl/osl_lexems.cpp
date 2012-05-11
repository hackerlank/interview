#include "stdafx.h"
#include "../misc.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_lexems.h"
#include "../gui/gui_misc.h"
#include "../iichan/iichan_eventalloc.h"
#include "../iichan/iichan_small_types.h"
#include "../iichan/iichan_resource.h"
#include "../iichan/iichan_enemy.h"
#include "../console.h"
#include "../draw/particle.h"


vector<OSL_VAR_TYPE> var_types;
vector<OSL_OPERATOR> operators;
vector<OSL_FUNCTION> sys_functions;
vector<OSL_CONSTANT> sys_constants;
vector<OSL_KEYWORD> keywords;

extern unsigned int OSL_CURRENT_LINE;

void osl_AddVarType(string name, E_OSL_VAR_TYPE type)
{
	OSL_VAR_TYPE t;
	t.name = name;
	t.type = type;
	var_types.push_back(t);
}

void osl_AddOperator(string name, E_OSL_OPERATOR_TYPE type)
{
	OSL_OPERATOR p;
	p.name = name;
	p.type = type;
	operators.push_back(p);
}

void osl_AddSysFunction(string name, int param_count, bool system, E_OSL_VAR_TYPE return_type)
{
	OSL_FUNCTION f;
	f.name = name;
	f.param_count = param_count;
	f.system = system;
	f.return_type = return_type;
	sys_functions.push_back(f);
}

void osl_AddSysConstant(string name, int value)
{
	OSL_CONSTANT c;
	c.name = name;
	c.data = value;
	sys_constants.push_back(c);
}

void osl_AddKeyword(string name, E_OSL_KEYWORD_TYPE type)
{
	OSL_KEYWORD k;
	k.name = name;
	k.type = type;
	keywords.push_back(k);
}


void osl_InitLexemizer()
{
	osl_AddVarType("int", vtInt);
	osl_AddVarType("float", vtFloat);
	osl_AddVarType("string", vtStr);

	osl_AddOperator("+", optPlus);
	osl_AddOperator("-", optMinus);
	osl_AddOperator("*", optMult);
	osl_AddOperator("/", optDvd);
	osl_AddOperator(">", optGrt);
	osl_AddOperator(">=", optGrtEq);
	osl_AddOperator("==", optEq);
	osl_AddOperator("<", optLs);
	osl_AddOperator("<=", optLsEq);
	osl_AddOperator("=", optAssign);

	osl_AddSysFunction("echo", 1, true, vtNone);
	osl_AddSysFunction("exit", 0, true, vtNone);
	osl_AddSysFunction("init", 1, true, vtInt);
	osl_AddSysFunction("load", 1, true, vtInt);
	osl_AddSysFunction("loadfont", 4, true, vtInt);
	osl_AddSysFunction("loadfontfromfile", 3, true, vtInt);
	osl_AddSysFunction("loadtexture", 1, true, vtInt);
	osl_AddSysFunction("loadresource", 1, true, vtInt);
	osl_AddSysFunction("loadconfig", 1, true, vtInt);
	osl_AddSysFunction("loaddialog", 1, true, vtInt);
	osl_AddSysFunction("loadsound", 1, true, vtInt);
	osl_AddSysFunction("loadweather", 1, true, vtInt);
	osl_AddSysFunction("startbriefing", 8, true, vtInt);
	osl_AddSysFunction("newguielem", 2, true, vtNone);
	osl_AddSysFunction("setguielem", -1, true, vtNone); // -1 потому что несколько функций
	osl_AddSysFunction("createplayer", 6, true, vtNone);
	osl_AddSysFunction("createpowerup", 5, true, vtNone);
	osl_AddSysFunction("createenemy", 4, true, vtNone);
	osl_AddSysFunction("createevent", 5, true, vtNone);
	osl_AddSysFunction("createsprite", 3, true, vtNone);
	osl_AddSysFunction("setworldmaxx", 1, true, vtNone);
	osl_AddSysFunction("setworldminx", 1, true, vtNone);
	osl_AddSysFunction("loadlevel", 1, true, vtNone);  
	osl_AddSysFunction("freevar", 1, true, vtNone);
	osl_AddSysFunction("freeallvar", 0, true, vtNone);
	osl_AddSysFunction("terminategame", 0, true, vtNone);
	osl_AddSysFunction("showmenu", 1, true, vtNone);
	osl_AddSysFunction("startweather", 2, true, vtNone);
	

	// Сигналы для GUI
	osl_AddSysConstant("gui_sgn_open_group", GUI_SGN_OPEN_GROUP);
	osl_AddSysConstant("gui_sgn_close_program", GUI_SGN_CLOSE_PROGRAM);
	osl_AddSysConstant("gui_sgn_exit_game", GUI_SGN_EXIT_GAME);
	osl_AddSysConstant("gui_sgn_start_game", GUI_SGN_START_GAME);
	osl_AddSysConstant("gui_sgn_continue_game", GUI_SGN_CONTINUE_GAME);

	// Константы для CreateEvent
	osl_AddSysConstant("event_action_hero_say", EVENT_ACTION_HERO_SAY);
	osl_AddSysConstant("event_action_start_dialog", EVENT_ACTION_START_DIALOG);
	osl_AddSysConstant("event_action_set_ai", EVENT_ACTION_SET_AI);
	osl_AddSysConstant("event_action_set_pause", EVENT_ACTION_SET_PAUSE);
	osl_AddSysConstant("event_action_disable_keyboard", EVENT_ACTION_DISABLE_KEYBOARD);
	osl_AddSysConstant("event_action_hide_hud", EVENT_ACTION_HIDE_HUD);
	osl_AddSysConstant("event_action_disable_id_ai", EVENT_ACTION_DISABLE_ID_AI);
	osl_AddSysConstant("event_action_enable_id_ai", EVENT_ACTION_ENABLE_ID_AI);
	osl_AddSysConstant("event_action_move_id", EVENT_ACTION_MOVE_ID);
	osl_AddSysConstant("event_action_stop_id", EVENT_ACTION_STOP_ID);
	osl_AddSysConstant("event_action_sound_play", EVENT_ACTION_SOUND_PLAY);
	osl_AddSysConstant("event_action_sound_stop", EVENT_ACTION_SOUND_STOP);
	osl_AddSysConstant("event_action_music_play", EVENT_ACTION_MUSIC_PLAY);
	osl_AddSysConstant("event_action_music_stop", EVENT_ACTION_MUSIC_STOP);
	osl_AddSysConstant("event_action_all_sounds_stop", EVENT_ACTION_ALL_SOUNDS_STOP);
	osl_AddSysConstant("event_action_exec_osl_string", EVENT_ACTION_EXEC_OSL_STRING);
	osl_AddSysConstant("event_action_start_quake", EVENT_ACTION_START_QUAKE);
	osl_AddSysConstant("event_action_stop_quake", EVENT_ACTION_STOP_QUAKE);
	osl_AddSysConstant("event_condition_time_elapsed", EVENT_CONDITION_TIME_ELAPSED);
	osl_AddSysConstant("event_condition_hero_x_gr", EVENT_CONDITION_HERO_X_GR);
	osl_AddSysConstant("event_condition_id_x_gr", EVENT_CONDITION_ID_X_GR);
	osl_AddSysConstant("event_condition_id_x_eq", EVENT_CONDITION_ID_X_EQ);
	osl_AddSysConstant("event_condition_game_started", EVENT_CONDITION_GAME_STARTED);
	osl_AddSysConstant("event_condition_period", EVENT_CONDITION_PERIOD);

	// Константы для particles (enums)
	osl_AddSysConstant("rtscreentop", rtScreenTop);
	osl_AddSysConstant("rtscreenrandom", rtScreenRandom);
	osl_AddSysConstant("dclifezero", dcLifeZero);
	osl_AddSysConstant("dcoutofscreen", dcOutOfScreen);

	//osl_AddSysConstant("animstand", 4);							не использовалась нигде вроде бы

	// Типы поверапов
	osl_AddSysConstant("pwrhealth", pwrHealth);
	osl_AddSysConstant("pwrscore", pwrScore);
	osl_AddSysConstant("pwrammo", pwrAmmo);

	osl_AddSysConstant("none", 0);
	osl_AddSysConstant("player", resPlayer);
	osl_AddSysConstant("enemy", resEnemy);
	osl_AddSysConstant("powerup", resPowerup);
	osl_AddSysConstant("sprite", resSprite);
	osl_AddSysConstant("animatedsprite", resAnimSprite);

	// Типы врагов
	osl_AddSysConstant("enemyfighter", enemyFighter);
	osl_AddSysConstant("enemyshooter", enemyShooter);


	osl_AddSysConstant("sys_scr_width", INTERNAL_CONSTANT);
	osl_AddSysConstant("sys_scr_height", INTERNAL_CONSTANT);
	osl_AddSysConstant("menu_main", MENU_MAIN);
	osl_AddSysConstant("menu_character", MENU_CHARACTER);
	osl_AddSysConstant("menu_game", MENU_GAME);
	osl_AddSysConstant("menu_2bcontinued", MENU_2BCONTINUED);
	osl_AddSysConstant("menu_failure", MENU_FAILURE);
	osl_AddSysConstant("gui_console_group", GUI_CONSOLE_GROUP);


	osl_AddKeyword("function", ktFunction);
	osl_AddKeyword("if", ktIf);
	osl_AddKeyword("else", ktElse);
	osl_AddKeyword("while", ktWhile);
	osl_AddKeyword("do", ktDo);
	osl_AddKeyword("break", ktBreak);
	osl_AddKeyword("const", ktConst);
}




bool osl_InputFile(const char* file_name, char** dest)
{
	FILE* file;
	file = fopen(file_name, "rb");
	OSL_CURRENT_LINE = 0;
	if(!file)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Failed to open " + string(file_name));
		return false;
	}
	else
	{
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* buf = new char[length+1];
		int l = 0;

		while(feof(file) == 0)
		{
			fread(&buf[l], 1, 1, file);
			l++;			
		}
		buf[l-1] = '\0';
		fclose(file);
		*dest = new char[l+1];
		strncpy(*dest, buf, l);
		delete[] buf;
		return true;
	}
}

bool osl_InputString(const char* input, char** dest)
{
	UINT j = strlen(input);
	char* buf = new char[j+2];
	char tk;
	bool inside = false;
	bool inside2 = false;
	bool comment = false;
	bool escaped = false;
	int l=0;

	for (UINT i = 0; i < j; i++)
	{
		tk = *(input+i);
		if(tk == '"')
		{
			inside = !inside;
		}
		else if(tk == '\'')
		{
			inside2 = !inside2;
		}
		else if(tk == COMMENT_CHAR)
		{
			comment = true;
		}

		if(!inside)
			tk = tolower(tk);

		if(iswspace(tk) == 0 || inside || inside2)
		{
			if(!comment)
			{
				buf[l] = tk;
				l++;
			}
		}
		else if(tk == '\n')
		{
			buf[l] = SKIP_CHAR;
			comment = false;
			l++;
		}

	}
	if (buf[l-1]!=';')
	{
		buf[l] = ';';
		buf[l+1] = '\0';
	}
	else
		buf[l] = '\0';
	*dest = new char[l+2];
	strncpy(*dest, buf, l+2);
	delete[] buf;
	return true;
}

E_TOKEN_TYPE osl_GetToken(string s)
{
	if(osl_IsSemicolon(s))			return ttSemicolon;
	else if(osl_IsComma(s))			return ttComma;
	else if(osl_IsEqualcolon(s))	return ttEqualColon;
	else if(osl_IsLeftBracket(s))	return ttLeftBracket;
	else if(osl_IsRightBracket(s))	return ttRightBracket;
	else if(osl_IsLeftBrace(s))		return ttLeftBrace;
	else if(osl_IsRightBrace(s))	return ttRightBrace;
	else if(osl_IsOperator(s))		return ttOperator;
	else if(osl_IsVarType(s))		return ttVarType;
	else if(osl_IsStrConstant(s))	return ttStrConstant;
	else if(osl_IsIntConstant(s))	return ttIntConstant;
	else if(osl_IsFloatConstant(s))	return ttFloatConstant;
	else if(osl_IsSysFunction(s))	return ttSysFunction;
	else if(osl_IsSysConstant(s))	return ttSysConstant;
	else if(osl_IsKeyword(s))		return ttKeyword;
	else if(osl_IsValidName(s))		return ttValidName;
	else if(osl_IsSys(s))			return ttSys;
	else							return ttUnknown;
}

OSL_FUNCTION* osl_GetSysFunction(string name)
{
	for(vector<OSL_FUNCTION>::iterator it = sys_functions.begin();
		it != sys_functions.end();
		it++)
	{
		if((*it).name == name)
			return &(*it);
	}
	return NULL;
}


bool osl_IsEqualcolon(string s)		{ return (s == "="); }
bool osl_IsSemicolon(string s)		{ return (s == ";"); }
bool osl_IsComma(string s)			{ return (s == ","); }
bool osl_IsLeftBracket(string s)	{ return (s == "("); }
bool osl_IsRightBracket(string s)	{ return (s == ")"); }
bool osl_IsLeftBrace(string s)		{ return (s == "{"); }
bool osl_IsRightBrace(string s)		{ return (s == "}"); }
bool osl_IsStrConstant(string s)
{ 
	if(s.length() <= 1)
		return false;

	return((s[0] == '"' && s[s.length() - 1] == '"') ||
		(s[0] == '\'' && s[s.length() - 1] == '\''));

}
bool osl_IsIntConstant(string s)	{ return IsIntNumber(s); }
bool osl_IsFloatConstant(string s)	{ return IsFloatNumber(s); }
bool osl_IsSys(string s)			{ return (s.length() == 1 && s[0] == SKIP_CHAR); }

bool osl_IsConstant(string s)
{
	return (osl_IsIntConstant(s) || osl_IsFloatConstant(s) || osl_IsStrConstant(s));
}

bool osl_IsValidName(string s)
{
	static const string deprecated = "!@#$%^&+-*~`/|%=<>(){}[].,.;\'\"";
	return (s.find_first_of(deprecated) == s.npos && !isdigit(s[0]) && !osl_IsConstant(s));
}

bool osl_IsOperator(string s)
{
	for(vector<OSL_OPERATOR>::iterator it = operators.begin();
		it != operators.end();
		it++)
	{
		if((*it).name == s)
			return true;
	}
	return false;
}

E_OSL_OPERATOR_TYPE osl_GetOperatorType(string s)
{
	for(vector<OSL_OPERATOR>::iterator it = operators.begin();
		it != operators.end();
		it++)
	{
		if((*it).name == s)
			return (*it).type;
	}
	return optNone;
}

bool osl_IsVarType(string s)
{
	for(vector<OSL_VAR_TYPE>::iterator it = var_types.begin();
		it != var_types.end();
		it++)
	{
		if((*it).name == s)
			return true;
	}
	return false;
}

bool osl_IsSysFunction(string s)
{
	for(vector<OSL_FUNCTION>::iterator it = sys_functions.begin();
		it != sys_functions.end();
		it++)
	{
		if((*it).name == s)
			return true;
	}
	return false;
}

bool osl_IsSysConstant(string s)
{
	for(vector<OSL_CONSTANT>::iterator it = sys_constants.begin();
		it != sys_constants.end();
		it++)
	{
		if((*it).name == s)
			return true;
	}
	return false;
}

bool osl_IsKeyword(string s)
{
	for(vector<OSL_KEYWORD>::iterator it = keywords.begin();
		it != keywords.end();
		it++)
	{
		if((*it).name == s)
			return true;
	}
	return false;
}

								
int osl_Lexemize(char* char_stream, std::vector<OSL_LEXEM>* lexem_stream)
{
	static const string delim = "\"\'!+*=/%-,;(){}[]<>|";

	string src(char_stream);
	int lexem_count = 0;
	OSL_LEXEM lexem;
	OSL_LEXEM prev;

	int src_count = src.length();

	bool minus = false;
	OSL_LEXEM reserved;

	int i = 0;
	bool inside = false;
	bool was = false;
	string body;
	int delimpos = src.npos;

	while(i < src_count)
	{
		if(src[i] == '"')
		{
			int p = src.find_first_of('"', i+1);

			if(p == src.npos)
				WriteToLog(DEFAULT_OSL_LOG_NAME, "\" expected");
			else
			{
				body = src.substr(i, p - i + 1);
				i = p + 1;
				lexem_count++;
				lexem.type = osl_GetToken(body);
				lexem.token = body;					
				lexem_stream->push_back(lexem);
			}
			continue;
		}
		else if(src[i] == '\'')
		{
			int p = src.find_first_of('\'', i+1);

			if(p == src.npos)
				WriteToLog(DEFAULT_OSL_LOG_NAME, "\' expected");
			else
			{
				body = src.substr(i, p - i + 1);
				i = p + 1;
				lexem_count++;
				lexem.type = osl_GetToken(body);
				lexem.token = body;
				lexem_stream->push_back(lexem);
			}
			continue;
		}
		else
			delimpos = src.find_first_of(delim, i);
		
		if(delimpos == src.npos)
		{
			lexem_count++;
			body = src.substr(i);
			lexem.type = osl_GetToken(body);
			lexem.token = body;
			lexem_stream->push_back(lexem);
			i++;
		}
		else
		{
			lexem_count++;

			body = src.substr(i, max(delimpos - i, unsigned(1)));
			i = max(delimpos, i + 1);
			

			// костыль, но что поделать
			if(body != " ")
			{
				lexem.type = osl_GetToken(body);
				lexem.token = body;
				if(lexem.type == ttOperator)
				{
					if(lexem.token == "-")
					{				
						reserved = lexem;
						minus = true;
						continue;
					}
					else
					{					
						lexem_stream->push_back(lexem);
					}
				}
				else if(lexem.type == ttIntConstant || lexem.type == ttFloatConstant)
				{
					if(minus)
					{
						reserved.token = "+";
						if(osl_IsConstant(prev.token))
						{						
							lexem_stream->push_back(reserved);
						}
						if(osl_IsIntConstant(lexem.token))
						{
							lexem.token = osl_IntToStr(-osl_StrToInt(lexem.token));
							lexem_stream->push_back(lexem);
							minus = false;
						}
						else if(osl_IsFloatConstant(lexem.token))
						{
							lexem.token = osl_FloatToStr(-osl_StrToFloat(lexem.token));
							lexem_stream->push_back(lexem);
							minus = false;
						}
					}
					else
					{
						lexem_stream->push_back(lexem);
					}
				}
				else
				{
					if(minus)
					{
						minus = false;
						lexem_stream->push_back(reserved);
					}
					else
						lexem_stream->push_back(lexem);
				}
				prev = lexem;
			}

		}
	}
	return lexem_count;
}

#include "stdafx.h"
#include "../draw/font.h"
#include "../draw/texture.h"
#include "../misc.h"
#include "../config.h"
#include "../draw/draw.h"
#include "../gui/gui.h"
#include "../scene.h"
#include "../sound/snd.h"
#include "../console.h"
#include "../iichan/iichan_resource.h"
#include "../iichan/iichan_dialog.h"
#include "../iichan/iichan_weather.h"
#include "osl_types.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_makegui.h"
#include "osl_makeiichan.h"
#include "osl_variables.h"
#include "osl.h"

extern Console* console;
extern GUI* gui;
extern SND* snd;
extern config cfg;

extern unsigned int current_state;
extern string OSL_CURRENT_FILE;

extern vector<OSL_VAR> var_pool;

string OSL_CURRENT_PARAMS;
string OSL_CURRENT_FUNCTION;

#define OSL_EXEC_OK			"1"
#define OSL_EXEC_FAIL		"0"


// Каждая функция получает вектор параметров
typedef vector<string> param_list;

typedef string (*p_function)(param_list*);
map<string, p_function> function_map;


string _param_list_to_str(param_list* p)
{
	string res;
	if(p)
	{
		for(param_list::iterator it = p->begin();
			it != p->end();
			it++)
		{
			res += *it + ", ";
		}
		res.resize(res.length() - 2);
	}
	return res;
}


// Список системных функций
string osl_Echo(param_list* params);
string osl_Exit(param_list* params);
string osl_Load(param_list* params);
string osl_InitSubsystem(param_list* params);
string osl_LoadConfig(param_list* params);
string osl_LoadSound(param_list* params);
string osl_LoadDialog(param_list* params);
string osl_StartBriefing(param_list* params);
string osl_LoadFontFromFile(param_list* params);
string osl_LoadFont(param_list* params);
string osl_LoadTexture(param_list* params);
string osl_LoadResource(param_list* params);
string osl_LoadWeather(param_list* params);
string osl_NewGUIElem(param_list* params);
string osl_SetGUIElem(param_list* params);
string osl_CreatePlayer(param_list* params);
string osl_CreatePowerup(param_list* params);
string osl_CreateEvent(param_list* params);
string osl_CreateEnemy(param_list* params);
string osl_CreateSprite(param_list* params);
string osl_SetWorldMaxX(param_list* params);
string osl_SetWorldMinX(param_list* params);
string osl_LoadLevel(param_list* params);
string osl_TerminateGame(param_list* params);
string osl_ShowMenu(param_list* params);
string osl_StartWeather(param_list* params);


// Реализации
// NOTE:
// Проверка параметров ложится на каждую функцию в отдельности

string osl_Echo(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Warning("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;
			if(gui)
				gui->LogOnScreen(p);
			else
				osl_Error("gui not initialized");
		}
		_result = OSL_EXEC_OK;
	}
	return _result;
}

string osl_Exit(param_list* params)
{
	string _result;
	if(params != NULL)
	{
		osl_Warning("arguments unexpected, so ignored");
	}
	current_state = GAME_EXITING;
	_result = OSL_EXEC_OK;
	return _result;
}

string osl_Load(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		int success = 0;
		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;

			if(osl_IsStrConstant(p))
			{
				char* old_file = (char*)OSL_CURRENT_FILE.c_str();
				bool ok = osl_Load(osl_RemoveQuotesStr(p).c_str(), false);
				OSL_CURRENT_FILE = old_file;
				ok ? success++ : WriteToLog(DEFAULT_LOG_NAME, "Failed to load file: " + p);
			}
			else
			{
				osl_Error("wrong argument");
			}	
		}
		success == params->size() ? _result = OSL_EXEC_OK :	_result = OSL_EXEC_FAIL;
	}
	return _result;
}


// Эти инлайновые функции возвращают 1 или 0
// Используется для подсчета в osl_Init
// Допустим мы вызвали Init(gui, sound, console),
// и консоль не создалась то, суммарное число будет меньшее,
// чем число переданных функции параметров, значит мы должны
// вернуть OSL_EXEC_FAIL. В то же время, если консоль *уже*
// проинициализирована, то ничего страшного и возвращаем 1

inline int _init_gui()
{
	gui ? osl_Warning("GUI already initialized, skipping") : gui = new GUI();
	if(gui)
		return 1;
	else
		return 0;
}

inline int _init_sound()
{
	snd ? osl_Warning("Sound already initialized, skipping") : snd = new SND();
	if(snd)
		return 1;
	else
		return 0;
}

inline int _init_console()
{
	console ? osl_Warning("Console already initialized, skipping") : console = new Console();
	if(console)
		return 1;
	else
		return 0;
}

string osl_InitSubsystem(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		int success = 0;
		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;

			if(p == "gui")
				success += _init_gui();
			else if(p == "sound")
				success += _init_sound();
			else if(p == "console")
				success += _init_console();
			else
				osl_Warning("unknown argument: " + p);
		}
		
		success == params->size() ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_LoadConfig(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		int success = 0;
		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;

			if(osl_IsStrConstant(p))
			{
				WriteToLog(DEFAULT_LOG_NAME, "Loading config: " + p);
				bool ok = LoadConfig(osl_RemoveQuotesStr(p).c_str(), &cfg);

				if(ok)
				{
					scene::ApplyConfig(&cfg);
					success++;
				}
				else
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load config");
			}
			else
			{
				osl_Error("LoadConfig(): wrong argument");
			}
		}
		success == params->size() ? _result = OSL_EXEC_OK :	_result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_StartWeather(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 2)
	{
		osl_Error("Wrong number of arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);

		if(osl_IsStrConstant(p1) && osl_IsIntConstant(p2))
		{
			WriteToLog(DEFAULT_LOG_NAME, "Starting weather: " + p1);
			Weather* w = WeatherByName(osl_RemoveQuotesStr(p1).c_str());
			if(w)
			{
				StartWeather(w, osl_StrToInt(p2));
				_result = OSL_EXEC_OK;
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Weather " + p1 + " not found");
				_result = OSL_EXEC_FAIL;
			}

		}
		else
		{
			osl_Error("LoadConfig(): wrong arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

string osl_LoadSound(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(!snd)
	{
		osl_Error("sound not initialized");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		int success = 0;

		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;

			if(osl_IsStrConstant(p))
			{
				WriteToLog(DEFAULT_LOG_NAME, "Loading sound: " + p);

				bool ok = snd->LoadSound(osl_RemoveQuotesStr(p));
				if(ok)
					success++;
				else
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load sound");
			}
			else
			{
				osl_Error("wrong argument");
			}
		}
		success == params->size() ? _result = OSL_EXEC_OK :	_result = OSL_EXEC_FAIL;
	}
	return _result;
}


string osl_LoadDialog(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		int success = 0;

		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;

			if(osl_IsStrConstant(p))
			{
				WriteToLog(DEFAULT_LOG_NAME, "Loading dialog: " + p);

				bool ok = LoadDialog(osl_RemoveQuotesStr(p).c_str());
				if(ok)
					success++;
				else
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load dialog");
			}
			else
			{
				osl_Error("wrong argument");
			}
		}
		success == params->size() ? _result = OSL_EXEC_OK :	_result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_LoadWeather(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		int success = 0;

		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			string p = *it;

			if(osl_IsStrConstant(p))
			{
				WriteToLog(DEFAULT_LOG_NAME, "Loading weather: " + p);

				bool ok = LoadWeather(osl_RemoveQuotesStr(p).c_str());
				if(ok)
					success++;
				else
					WriteToLog(DEFAULT_LOG_NAME, "Failed to load weather");
			}
			else
			{
				osl_Error("wrong argument");
			}
		}
		success == params->size() ? _result = OSL_EXEC_OK :	_result = OSL_EXEC_FAIL;
	}
	return _result;
}

// TODO:
// Добавление брифингов в очередь

string osl_StartBriefing(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 8)
	{
		osl_Error("8 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		string p5 = *(params->begin()+4);
		string p6 = *(params->begin()+5);
		string p7 = *(params->begin()+6);
		string p8 = *(params->begin()+7);

		if(osl_IsStrConstant(p1) &&
			osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) &&
			osl_IsIntConstant(p4) &&
			osl_IsIntConstant(p5) &&
			osl_IsIntConstant(p6) &&
			osl_IsFloatConstant(p7) &&
			osl_IsIntConstant(p8))
		{
			string s = osl_RemoveQuotesStr(p1);

			WriteToLog(DEFAULT_LOG_NAME, "Starting briefing: " + s);
			StartBriefing(p1, p2, p3, p4, p5, p6, p7, p8);
			_result = OSL_EXEC_OK;
		}
		else
		{
			osl_Error("Invalid arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

string osl_LoadFontFromFile(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 3)
	{
		osl_Error("3 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);

		if(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) && osl_IsIntConstant(p3))
		{
			WriteToLog(DEFAULT_LOG_NAME, "Loading font from file: " + p1);
			if(LoadFontFromFile(osl_RemoveQuotesStr(p1).c_str(),
				osl_StrToInt(p2), osl_StrToInt(p3)))
			{
				_result = OSL_EXEC_OK;
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load font from file");
				_result = OSL_EXEC_FAIL;
			}
		}
		else
		{
			osl_Error("wrong arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

string osl_LoadFont(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 4)
	{
		osl_Error("4 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);

		if(osl_IsStrConstant(p1) && osl_IsIntConstant(p2) &&
			osl_IsIntConstant(p3) && osl_IsStrConstant(p4))
		{
			WriteToLog(DEFAULT_LOG_NAME, "Loading font: " + p1);

			if(LoadFont(
				osl_RemoveQuotesStr(p1).c_str(),
				osl_StrToInt(p2),
				osl_StrToInt(p3),
				osl_RemoveQuotesStr(p4).c_str()))
			{
				_result = OSL_EXEC_OK;
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load font");
				_result = OSL_EXEC_FAIL;
			}
		}
		else
		{
			osl_Error("wrong arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

string osl_LoadTexture(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());

		if(osl_IsStrConstant(p1))
		{
			WriteToLog(DEFAULT_LOG_NAME, "Loading texture: " + p1);

			if(LoadTexture(osl_RemoveQuotesStr(p1).c_str()))
			{
				_result = OSL_EXEC_OK;
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load texture");
				_result = OSL_EXEC_FAIL;
			}
		}
		else
		{
			osl_Error("wrong arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

string osl_LoadResource(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());

		if(osl_IsStrConstant(p1))
		{
			WriteToLog(DEFAULT_LOG_NAME, "Loading resource: " + p1);

			if(LoadResource(osl_RemoveQuotesStr(p1).c_str()))
			{
				_result = OSL_EXEC_OK;
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Failed to load resource");
				_result = OSL_EXEC_FAIL;
			}
		}
		else
		{
			osl_Error("wrong arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

string osl_NewGUIElem(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(!gui)
	{
		osl_Error("gui not initialized");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 2)
	{
		osl_Error("2 parametres expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);

		if(osl_IsStrConstant(p1) && osl_IsStrConstant(p2))
		{
			NewGUIElem(osl_RemoveQuotesStr(p1),
				osl_RemoveQuotesStr(p2));
			_result = OSL_EXEC_OK;
		}
		else
		{
			osl_Error("wrong arguments");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}

// Здесь проверка параметров на валидность не нужна,
// т.к параметры проверяются в osl_makegui.cpp

string osl_SetGUIElem(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(!gui)
	{
		osl_Error("gui not initialized");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() == 3)
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);

		SetGUIElem(osl_RemoveQuotesStr(p1),
			osl_RemoveQuotesStr(p2),
			osl_RemoveQuotesStr(p3));
		_result = OSL_EXEC_OK;

	}
	else if(params->size() == 5)
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		string p5 = *(params->begin()+4);

		SetGUIElem(osl_RemoveQuotesStr(p1),
			osl_RemoveQuotesStr(p2),
			osl_RemoveQuotesStr(p3),
			osl_RemoveQuotesStr(p4),
			osl_RemoveQuotesStr(p5));
		_result = OSL_EXEC_OK;

	}
	else if(params->size() == 6)
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		string p5 = *(params->begin()+4);
		string p6 = *(params->begin()+5);

		SetGUIElem(osl_RemoveQuotesStr(p1),
			osl_RemoveQuotesStr(p2),
			osl_RemoveQuotesStr(p3),
			osl_RemoveQuotesStr(p4),
			osl_RemoveQuotesStr(p5),
			osl_RemoveQuotesStr(p6));
		_result = OSL_EXEC_OK;
	}
	else
	{
		osl_Error("incorrect number of arguments");
		_result = OSL_EXEC_FAIL;
	}
	return _result;
}

// Здесь проверка параметров на валидность не нужна,
// т.к параметры проверяются в osl_makeiichan.cpp

string osl_CreatePlayer(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 6)
	{
		osl_Error("6 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		string p5 = *(params->begin()+4);
		string p6 = *(params->begin()+5);
		
		bool ok = CreatePlayer(p1, p2, p3, p4, p5, p6);
		ok ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_CreatePowerup(param_list* params)
{
 	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 4)
	{
		osl_Error("4 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		//string p5 = *(params->begin()+4);
		
 		bool ok = CreatePowerup(p1, p2, p3, p4);
		ok ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_CreateEvent(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 6)
	{
		osl_Error("6 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		string p5 = *(params->begin()+4);
		string p6 = *(params->begin()+5);
		
		bool ok = CreateEvent(p1, p2, p3, p4, p5, p6);

		ok ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_CreateEnemy(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 4)
	{
		osl_Error("4 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		string p4 = *(params->begin()+3);
		
		bool ok = CreateEnemy(p1, p2, p3, p4);
		ok ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}

	return _result;
}

string osl_CreateSprite(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 3)
	{
		osl_Error("3 arguments expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		string p2 = *(params->begin()+1);
		string p3 = *(params->begin()+2);
		
		bool ok = CreateSprite(p1, p2, p3);
		ok ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;
	}
	return _result;
}

string osl_SetWorldMaxX(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 argument expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());

		SetWorldMaxX(p1);
		_result = OSL_EXEC_OK;
	}
	return _result;
}

string osl_SetWorldMinX(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 argument expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());

		SetWorldMinX(p1);
		_result = OSL_EXEC_OK;
	}
	return _result;
}

string osl_LoadLevel(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 argument expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());
		bool ok = LoadLevel(osl_RemoveQuotesStr(p1));
		ok ? _result = OSL_EXEC_OK : _result = OSL_EXEC_FAIL;

	}
	return _result;
}

string osl_FreeVariable(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 argument expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		string p1 = *(params->begin());

		// TODO: fix bug, check type
		osl_UndeclareVar(osl_RemoveQuotesStr(p1), &var_pool);
		_result = OSL_EXEC_OK;

	}
	return _result;
}

string osl_FreeAllVariables(param_list* params)
{
	string _result;
	if(params != NULL)
	{
		osl_Warning("arguments unexpected, so ignored");
	}
	
	osl_FreeAllVar(&var_pool);
	_result = OSL_EXEC_OK;
	return _result;
}

string osl_TerminateGame(param_list* params)
{
	string _result;
	if(params != NULL)
	{
		osl_Warning("arguments unexpected, so ignored");
	}

	TerminateGame();
	_result = OSL_EXEC_OK;
	return _result;
}

string osl_ShowMenu(param_list* params)
{
	string _result;
	if(!params)
	{
		osl_Error("empty arguments");
		_result = OSL_EXEC_FAIL;
	}
	else if(params->size() != 1)
	{
		osl_Error("1 argument expected");
		_result = OSL_EXEC_FAIL;
	}
	else
	{
		if(gui)
		{
			string p1 = *(params->begin());

			if(osl_IsIntConstant(p1)) // TODO: вынести проверку?
			{
				gui->wrapperShowGroup(osl_StrToInt(p1));
				_result = OSL_EXEC_OK;
			}
			else
			{
				osl_Error("Incompatible parametres");
				_result = OSL_EXEC_FAIL;
			}
		}
		else
		{
			osl_Error("GUI not initialized");
			_result = OSL_EXEC_FAIL;
		}
	}
	return _result;
}


void osl_MapFunction(string name, p_function func)
{
	function_map[name] = func;
}

void osl_InitSystemFunctions()
{
	osl_MapFunction("echo", osl_Echo);
	osl_MapFunction("exit", osl_Exit);
	osl_MapFunction("load", osl_Load);
	osl_MapFunction("init", osl_InitSubsystem);
	osl_MapFunction("loadconfig", osl_LoadConfig);
	osl_MapFunction("loadsound", osl_LoadSound);
	osl_MapFunction("loaddialog", osl_LoadDialog);
	osl_MapFunction("loadweather", osl_LoadWeather);
	osl_MapFunction("startbriefing", osl_StartBriefing);
	osl_MapFunction("loadfontfromfile", osl_LoadFontFromFile);
	osl_MapFunction("loadfont", osl_LoadFont);
	osl_MapFunction("loadtexture", osl_LoadTexture);
	osl_MapFunction("loadresource", osl_LoadResource);
	osl_MapFunction("newguielem", osl_NewGUIElem);
	osl_MapFunction("setguielem", osl_SetGUIElem);
	osl_MapFunction("createplayer", osl_CreatePlayer);
	osl_MapFunction("createpowerup", osl_CreatePowerup);
	osl_MapFunction("createevent", osl_CreateEvent);
	osl_MapFunction("createenemy", osl_CreateEnemy);
	osl_MapFunction("createsprite", osl_CreateSprite);
	osl_MapFunction("setworldmaxx", osl_SetWorldMaxX);
	osl_MapFunction("setworldminx", osl_SetWorldMinX);
	osl_MapFunction("loadlevel", osl_LoadLevel);
	osl_MapFunction("freevar", osl_FreeVariable);
	osl_MapFunction("freeallvar", osl_FreeAllVariables);
	osl_MapFunction("terminategame", osl_TerminateGame);
	osl_MapFunction("showmenu", osl_ShowMenu);
	osl_MapFunction("startweather", osl_StartWeather);
}

void osl_FreeSystemFunctions()
{
	function_map.clear();
}

string osl_ExecuteSystemFunction(vector<OSL_LEXEM>* list)
{
	string _result;
	param_list* params = NULL;
	unsigned int param_count = 0;

	for(vector<OSL_LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		OSL_LEXEM lx = *it;

		if(osl_IsComma(lx.token) || osl_IsRightBracket(lx.token))
		{
			param_count++;

			if(param_count == 1)
			{
				params = new param_list();
			}
			params->push_back((*(it-1)).token);
		}
	}

	string function_name = (*(list->begin())).token;

	map<string, p_function>::const_iterator it = function_map.find(function_name);
	map<string, p_function>::iterator end = function_map.end();
	end--;
	bool found = false;

	if(it == end)
	{
		found = ((*it).first == function_name);
	}
	else
		found = true;

	if(found)
	{
		string log = "Calling " + function_name + "(";
		for(param_list::iterator it = params->begin();
			it != params->end();
			it++)
		{
			log += *it;
			if(it < params->end()-1)
			log += ", ";
		}

		log += ")";
		LogToFile(DEFAULT_OSL_LOG_NAME, log);

		OSL_CURRENT_FUNCTION = function_name;
		OSL_CURRENT_PARAMS = _param_list_to_str(params);

		_result = function_map[function_name](params);
		
		log = " Result: " + _result;
		LogToFile(DEFAULT_OSL_LOG_NAME, log);

		return _result;
	}
	else
	{
		osl_Warning("Unrecognized function (not implemented yet?)");
		return OSL_EXEC_FAIL;
	}
}
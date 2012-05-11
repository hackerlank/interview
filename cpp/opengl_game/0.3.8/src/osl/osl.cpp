#include "stdafx.h"
#include "../misc.h"
#include "../config.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_lexems.h"
#include "osl_parse.h"
#include "osl_debug.h"
#include "osl_execute.h"
#include "osl_variables.h"
#include "osl.h"

unsigned int	OSL_CURRENT_LINE	= 1;
string			OSL_CURRENT_FILE;

OSL_TEMPLATE_STORAGE storage;

// глобальный пул переменных
vector<OSL_VAR> var_pool;

// Инициализация шаблонов
// Вход:
//	template	- указатель на массив шаблонов инструкций языка OSL,
//					который инициализирует функция
// Выход:
//	void
// Примечания:
//	Вызывается один раз

void osl_InitTemplateStorage(OSL_TEMPLATE_STORAGE* storage)
{
	// %type(%name)=%math;
	vector<E_OSL_TEMPLATE_PATTERN> p1;
	p1.push_back(E_OSL_TYPE);
	p1.push_back(E_OSL_LEFT_BRACKET);
	p1.push_back(E_OSL_NAME);
	p1.push_back(E_OSL_RIGHT_BRACKET);
	p1.push_back(E_OSL_EQUALCOLON);
	p1.push_back(E_OSL_MATH);
	p1.push_back(E_OSL_SEMICOLON);

	// %type(%name);
	vector<E_OSL_TEMPLATE_PATTERN> p2;
	p2.push_back(E_OSL_TYPE);
	p2.push_back(E_OSL_LEFT_BRACKET);
	p2.push_back(E_OSL_NAME);
	p2.push_back(E_OSL_RIGHT_BRACKET);
	p2.push_back(E_OSL_SEMICOLON);

	// function %name(%param){ %code }
	vector<E_OSL_TEMPLATE_PATTERN> p3;
	p3.push_back(E_OSL_FUNCTION);
	p3.push_back(E_OSL_NAME);
	p3.push_back(E_OSL_LEFT_BRACKET);
	//p3.push_back(E_OSL_PARAM);
	p3.push_back(E_OSL_RIGHT_BRACKET);
	p3.push_back(E_OSL_LEFT_BRACE);
	p3.push_back(E_OSL_CODE);
	p3.push_back(E_OSL_RIGHT_BRACE);

	// %func(%param);
	vector<E_OSL_TEMPLATE_PATTERN> p4;
	p4.push_back(E_OSL_NAME);
	p4.push_back(E_OSL_LEFT_BRACKET);
	p4.push_back(E_OSL_PARAM);
	p4.push_back(E_OSL_RIGHT_BRACKET);
	p4.push_back(E_OSL_SEMICOLON);

	// if(%condition){ %code }
	vector<E_OSL_TEMPLATE_PATTERN> p5;
	p5.push_back(E_OSL_IF);
	p5.push_back(E_OSL_LEFT_BRACKET);
	p5.push_back(E_OSL_CONDITION);
	p5.push_back(E_OSL_RIGHT_BRACKET);
	p5.push_back(E_OSL_LEFT_BRACE);
	p5.push_back(E_OSL_CODE);
	p5.push_back(E_OSL_RIGHT_BRACE);

	// if(%condition){ %code} else { %code}
	vector<E_OSL_TEMPLATE_PATTERN> p6;
	p6.push_back(E_OSL_IF);
	p6.push_back(E_OSL_LEFT_BRACKET);
	p6.push_back(E_OSL_CONDITION);
	p6.push_back(E_OSL_RIGHT_BRACKET);
	p6.push_back(E_OSL_LEFT_BRACE);
	p6.push_back(E_OSL_CODE);
	p6.push_back(E_OSL_RIGHT_BRACE);
	p6.push_back(E_OSL_ELSE);
	p6.push_back(E_OSL_LEFT_BRACE);
	p6.push_back(E_OSL_CODE);
	p6.push_back(E_OSL_RIGHT_BRACE);

	// while(%condition){ %code }
	vector<E_OSL_TEMPLATE_PATTERN> p7;
	p7.push_back(E_OSL_WHILE);
	p7.push_back(E_OSL_LEFT_BRACKET);
	p7.push_back(E_OSL_CONDITION);
	p7.push_back(E_OSL_RIGHT_BRACKET);
	p7.push_back(E_OSL_LEFT_BRACE);
	p7.push_back(E_OSL_CODE);
	p7.push_back(E_OSL_RIGHT_BRACE);


	// do - while не работает
	// TODO: fix it
	// do { %code } while(%condition)
	vector<E_OSL_TEMPLATE_PATTERN> p8;
	p8.push_back(E_OSL_DO);
	p8.push_back(E_OSL_LEFT_BRACE);
	p8.push_back(E_OSL_CODE);
	p8.push_back(E_OSL_RIGHT_BRACE);
	p8.push_back(E_OSL_WHILE);
	p8.push_back(E_OSL_LEFT_BRACKET);
	p8.push_back(E_OSL_CONDITION);
	p8.push_back(E_OSL_RIGHT_BRACKET);
	p8.push_back(E_OSL_SEMICOLON);

	// %var=%math;
	vector<E_OSL_TEMPLATE_PATTERN> p9;
	p9.push_back(E_OSL_VARIABLE);
	p9.push_back(E_OSL_EQUALCOLON);
	p9.push_back(E_OSL_MATH);
	p9.push_back(E_OSL_SEMICOLON);


	storage->AddTemplate(E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE, p1);
	storage->AddTemplate(E_OSL_DECLARE_FUNCTION_TEMPLATE, p3);
	storage->AddTemplate(E_OSL_FUNCTION_CALL_TEMPLATE, p4);
	storage->AddTemplate(E_OSL_IF_TEMPLATE, p5);
	storage->AddTemplate(E_OSL_IF_ELSE_TEMPLATE, p6);
	storage->AddTemplate(E_OSL_WHILE_TEMPLATE, p7);
	storage->AddTemplate(E_OSL_DO_TEMPLATE, p8);
	storage->AddTemplate(E_OSL_ASSIGN_VAR_TEMPLATE, p9);
	storage->AddTemplate(E_OSL_DECLARE_VAR_TEMPLATE, p2);

	storage->init = true;
}



// Загрузка скрипта OSL
// Вход:
//	input -  имя файла или строка со скриптом
//	mode - 0 - чтение из файла скрипта, 1 - чтение из строки, содержащей скрипт
// Выход:
//	true/false - успешно ли выполнена функция
// Примечания:
//	

bool osl_Load(const char* input, bool mode)
{	
	if(!input)
		return false;

	if (!mode)
	{
		char* str = NULL;
		bool ok = false;

		// Игнорируем рекурсивную загрузку файла

		if(strcmp(OSL_CURRENT_FILE.c_str(), input) == 0)
		{
			osl_Error("Recursive loading " + string(input) + ", skipped");
			return false;
		}

		OSL_CURRENT_FILE = (char*)input;
		WriteToLog(DEFAULT_LOG_NAME, "Loading file: " + string(input));

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(input) + ": Input...");
#endif
		ok = osl_InputFile(input, &str);

		if(ok)
		{
			#ifdef _DEBUG_OSL
				WriteToLog(DEFAULT_OSL_LOG_NAME, string(input) + ": Input OK");
			#endif
		}
		else
		{
			WriteToLog(DEFAULT_LOG_NAME, string(input) + ": Input FAILED");
			return false;
		}

		ok = osl_Load(str,true);
		
		if (ok)
		{
	#ifdef _DEBUG_OSL
			WriteToLog(DEFAULT_OSL_LOG_NAME, string(input) + ": File successfully loaded");
	#endif
			return true;
		}
		else
		{
			WriteToLog(DEFAULT_LOG_NAME, string(input) + ": File failed to loaded");
			return false;		
		}
	}
	else
	{
		// разбиваем на строки (до точки с запятой)

		string s = string(input);
		osl_TrimComments(s);

		vector<UINT> semicolons;

		UINT l = s.length();
		bool in_quote = false;
		bool in_quote2 = false;
		for(UINT i = 0; i < l; i++)
		{
	
			if(s[i] == '\'')
				in_quote = !in_quote;
			else if(s[i] == '"')
				in_quote2 = !in_quote2;
			else if(s[i] == ';')
			{
				if(!in_quote && !in_quote2)
				{
					semicolons.push_back(i);
				}
			}
		}

		int prev_semicolon = -1;
		for(vector<UINT>::iterator it = semicolons.begin();
			it != semicolons.end();
			it++)
		{
			string line;
			UINT pos = *it;

			line = s.substr(prev_semicolon+1, pos - prev_semicolon);
			prev_semicolon = pos;

			//osl_TrimComments(line);
			osl_TrimSpaces(line);

			if(line.empty())
				continue;

			vector<OSL_LEXEM> lxm;
			char* str = NULL;
			bool ok = false;
			int lxm_count = 0;

			WriteToLog(DEFAULT_LOG_NAME, "Loading string: " + line);
			ok = osl_InputString(line.c_str(), &str);

		#ifdef _DEBUG_OSL
			WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize...");
		#endif
			lxm_count = osl_Lexemize(str, &lxm);
			lxm_count > 0 ? ok = true : ok = false;

			if(ok)
			{
		#ifdef _DEBUG_OSL
				char buffer[128];
				sprintf(buffer, "%s: Lexemize OK (%u lexems token)", input, lxm_count);
				WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
		#endif
				delete[] str;
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, string(str) + ": Lexemize FAILED");
				return false;
			}

		#ifdef _DEBUG_OSL
			osl_dDumpLexemStream(&lxm);
		#endif

		#ifdef _DEBUG_OSL
			WriteToLog(DEFAULT_OSL_LOG_NAME, "Interpret...");
		#endif

			ok = osl_Interpret(&lxm, &var_pool, &storage);
			//ok = osl_InterpretBeta(&lxm, &pool);

			if(ok)
			{
		#ifdef _DEBUG_OSL
				WriteToLog(DEFAULT_OSL_LOG_NAME, string(input) + ": Interpret OK");
		#endif
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, line + ": Interpret FAILED");
				return false;
			}
		#ifdef _DEBUG_OSL
			WriteToLog(DEFAULT_OSL_LOG_NAME, string(input) + ": String successfully loaded");
		#endif
		}
	}
	return true;
}



// Инициализация OSL

void osl_Init()
{
	// TODO: избавиться от storage (зачем он?)
	osl_InitTemplateStorage(&storage);
	osl_InitLexemizer();
	osl_InitSystemFunctions();
}

// Освобождение OSL

void osl_Free()
{
	storage.templates.clear();
	osl_FreeVarPool();
	osl_FreeSystemFunctions();
}

// Очищение переменных

void osl_FreeVarPool()
{
	osl_FreeAllVar(&var_pool);
}


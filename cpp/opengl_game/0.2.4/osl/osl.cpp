#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_variables.h"
#include "osl_templates.h"
#include "osl_parse.h"

#include "osl_debug.h"

#include "osl.h"




OSL_RETURN_DATA return_data;


// Инициализация
// Вход:
//	template	- указатель на массив шаблонов инструкций языка OSL,
//					который инициализирует функция
// Выход:
//	void
// Примечания:
//	Вызывается один раз

static void osl_InitTemplateStorage(OSL_TEMPLATE_STORAGE* storage)
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
	storage->AddTemplate(E_OSL_DECLARE_VAR_TEMPLATE, p2);
	storage->AddTemplate(E_OSL_DECLARE_FUNCTION_TEMPLATE, p3);
	storage->AddTemplate(E_OSL_FUNCTION_CALL_TEMPLATE, p4);
	storage->AddTemplate(E_OSL_IF_TEMPLATE, p5);
	storage->AddTemplate(E_OSL_IF_ELSE_TEMPLATE, p6);
	storage->AddTemplate(E_OSL_WHILE_TEMPLATE, p7);
	storage->AddTemplate(E_OSL_DO_TEMPLATE, p8);
	storage->AddTemplate(E_OSL_ASSIGN_VAR_TEMPLATE, p9);
}



// Загрузка скриптового файла OSL
// Вход:
//	file_name	- имя файла
// Выход:
//	true/false - успешно ли выполнена функция
// Примечания:
//	

bool osl_Load_v_04(const char* file_name)
{
	char* str = NULL;

	vector<LEXEM> lxm;
	OSL_TEMPLATE_STORAGE storage;
	vector<OSL_VAR> pool;

	bool ok = false;
	int lxm_count = 0;

	WriteToLog(DEFAULT_LOG_NAME, "Loading file: " + string(file_name));

	WriteToLog(DEFAULT_LOG_NAME, "Input...");
	ok = osl_Input(file_name, &str);

			if(ok)
				WriteToLog(DEFAULT_LOG_NAME, "Input OK");
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Input FAILED");
				return false;
			}

	WriteToLog(DEFAULT_LOG_NAME, "Lexemize...");
	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

			if(ok)
			{
				char buffer[50];

				sprintf(buffer, "Lexemize OK (%u lexems token)", lxm_count);
				WriteToLog(DEFAULT_LOG_NAME, buffer);
			}
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Lexemize FAILED");
				return false;
			}

	osl_dDumpLexemStream(&lxm);

	osl_InitTemplateStorage(&storage);

	WriteToLog(DEFAULT_LOG_NAME, "Compile...");
	ok = osl_Compile(&lxm, &pool, &storage);

		if(ok)
			WriteToLog(DEFAULT_LOG_NAME, "Compile OK");
		else
		{
			WriteToLog(DEFAULT_LOG_NAME, "Compile FAILED");
			return false;
		}

	WriteToLog(DEFAULT_LOG_NAME, "Interpret...");
	ok = osl_Interpret(&lxm, &pool, &storage);

			if(ok)
				WriteToLog(DEFAULT_LOG_NAME, "Interpret OK");
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Interpret FAILED");
				return false;
			}


	WriteToLog(DEFAULT_LOG_NAME, "File successfully loaded");
	return true;
}
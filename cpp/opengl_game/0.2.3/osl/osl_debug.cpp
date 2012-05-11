#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_variables.h"
#include "osl_templates.h"
#include "osl_functions.h"
#include "osl_maths.h"
#include "osl_parse.h"
#include "osl_debug.h"


// Проверка шаблона инструкций языка OSL
// Вход:
//	list	- поток лексем
// Выход:
//	true/false - успешно ли выполнена функция
// Примечания:
//	

void osl_dTestTemplate(vector<LEXEM>* list, OSL_TEMPLATE_STORAGE* storage)
{
	E_OSL_TEMPLATE t = storage->MatchLexems(list);
	
	switch(t)
	{
	case E_OSL_ASSIGN_VAR_TEMPLATE:	MessageBox(0, "E_OSL_ASSIGN_VAR_TEMPLATE", 0, 0); break;
	case E_OSL_DECLARE_VAR_TEMPLATE: MessageBox(0, "E_OSL_DECLARE_VAR_TEMPLATE", 0, 0);	break;
	case E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE:	MessageBox(0, "E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE", 0, 0); break;
	case E_OSL_DECLARE_FUNCTION_TEMPLATE: MessageBox(0, "E_OSL_DECLARE_FUNCTION_TEMPLATE", 0, 0); break;
	case E_OSL_FUNCTION_CALL_TEMPLATE: MessageBox(0, "E_OSL_FUNCTION_CALL_TEMPLATE", 0, 0); break;
	case E_OSL_IF_TEMPLATE:	MessageBox(0, "E_OSL_IF_TEMPLATE", 0, 0); break;
	case E_OSL_IF_ELSE_TEMPLATE: MessageBox(0, "E_OSL_IF_ELSE_TEMPLATE", 0, 0); break;
	case E_OSL_WHILE_TEMPLATE: MessageBox(0, "E_OSL_WHILE_TEMPLATE", 0, 0); break;
	case E_OSL_DO_TEMPLATE: MessageBox(0, "E_OSL_DO_TEMPLATE", 0, 0); break;
	case E_OSL_NO_TEMPLATE: MessageBox(0, "E_OSL_NO_TEMPLATE", 0, 0); break;
	default: break;
	}
}



// Дамп пула переменных
// Вход:
//	pool	- пул переменных OSL
// Выход:
//	void
// Примечания:
//	Дамп в DEFAULT_LOG_NAME	

void osl_dDumpVarPool(vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_LOG_NAME, "Dumping variable pool...");

	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		OSL_VAR var = *it;

		WriteToLog(DEFAULT_LOG_NAME, "\t name: \t\t" + var.name);
		WriteToLog(DEFAULT_LOG_NAME, "\t type: \t\t" + osl_VarTypeToString(var.type));
		WriteToLog(DEFAULT_LOG_NAME, "\t value: \t" + osl_VarToString(&var));
	}

	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}



// Дамп потока лексем
// Вход:
//	pool	- поток лексем OSL
// Выход:
//	void
// Примечания:
//	Дамп в DEFAULT_LOG_NAME

void osl_dDumpLexemStream(vector<LEXEM>* lexem_stream)
{
	WriteToLog(DEFAULT_LOG_NAME, "Dumping lexem stream...");

	//char type[50];
	string dump = "\t";
	for(vector<LEXEM>::iterator it = lexem_stream->begin();
		it != lexem_stream->end();
		it++)
	{
		LEXEM l = *it;
		dump += l.token += " ";
		
		//switch((it)->type)
		//{
		//case ttVarType: sprintf(type, "\tLexem type: ttVarType"); break;
		//case ttOperator: sprintf(type, "\tLexem type: ttOperator"); break;
		//case ttStrConstant: sprintf(type, "\tLexem type: ttStrConstant"); break;
		//case ttIntConstant: sprintf(type, "\tLexem type: ttIntConstant"); break;
		//case ttFloatConstant: sprintf(type, "\tLexem type: ttFloatConstant"); break;
		//case ttName: sprintf(type, "\tLexem type: ttIdentifier"); break;
		//case ttSysFunction: sprintf(type, "\tLexem type: ttSysFunction"); break;
		//case ttSysConstant: sprintf(type, "\tLexem type: ttSysConstant"); break;
		//case ttSemicolon: sprintf(type, "\tLexem type: ttSemicolon"); break;
		//case ttComma: sprintf(type, "\tLexem type: ttComma"); break;
		//case ttEqualcolon: sprintf(type, "\tLexem type: ttEqualcolon"); break;
		//case ttLeftBracket: sprintf(type, "\tLexem type: ttLeftBracket"); break;
		//case ttRightBracket: sprintf(type, "\tLexem type: ttRightBracket"); break;
		//case ttLeftBrace: sprintf(type, "\tLexem type: ttLeftBrace"); break;
		//case ttRightBrace: sprintf(type, "\tLexem type: ttRightBrace"); break;
		//case ttUnknown: sprintf(type, "\tLexem type: ttUnknown"); break;
		//case ttComparison: sprintf(type, "\tLexem type: ttComparison"); break;
		//case ttKeyword: sprintf(type, "\tLexem type: ttKeyword"); break;
		//case ttSys: sprintf(type, "\tLexem type: ttSys"); break;
		//default: break;
		//}
		//sprintf(type, "\tLexem token: %s", (it)->token.c_str());
		//WriteToLog(DEFAULT_LOG_NAME, type);
	}
	WriteToLog(DEFAULT_LOG_NAME, dump.c_str());
	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}



// Добавление в лог сообщения об ошибке
// Вход:
//	file		- имя файла, в котором произошла ошибка
//	function	- имя функции, в которой произошла ошибка
//	line		- номер строки, в которой произошла ошибка
// Выход:
//	void
// Примечания:
//	Ошибка пишется в DEFAULT_LOG_NAME
//	Используются макросы __FILE__, __FUNCTION__, __LINE__

void osl_dMacroError(char* file, char* function, int line)
{
	char buffer[192];

	sprintf_s(buffer, 192, "Error: file(%s); function(%s); line(%u)", file, function, line);
	WriteToLog(DEFAULT_LOG_NAME, string(buffer));
}



// Дамп массива строк
// Вход:
//	list	- массив строк
// Выход:
//	void
// Примечания:
//	Дамп в DEFAULT_LOG_NAME	

void osl_dDumpList(vector<string>* list)
{
	WriteToLog(DEFAULT_LOG_NAME, "Started dump");
	for(vector<string>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		WriteToLog(DEFAULT_LOG_NAME, *it);
	}
	WriteToLog(DEFAULT_LOG_NAME, "Finished dump");
}
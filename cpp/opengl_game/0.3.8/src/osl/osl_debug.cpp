#include "stdafx.h"
#include "../misc.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_variables.h"
#include "osl_debug.h"


// Проверка шаблона инструкций языка OSL
// Вход:
//	list	- поток лексем
// Выход:
//	true/false - успешно ли выполнена функция
// Примечания:
//	

void osl_dTestTemplate(vector<OSL_LEXEM>* list, OSL_TEMPLATE_STORAGE* storage)
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
//	Дамп в DEFAULT_OSL_LOG_NAME	

void osl_dDumpVarPool(vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Dumping variable pool...");

	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		OSL_VAR var = *it;

		WriteToLog(DEFAULT_OSL_LOG_NAME, "\t name: \t\t" + var.name);
		WriteToLog(DEFAULT_OSL_LOG_NAME, "\t type: \t\t" + osl_VarTypeToString(var.type));
		WriteToLog(DEFAULT_OSL_LOG_NAME, "\t value: \t" + osl_VarToString(&var));
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Finished dumping");
}



// Дамп потока лексем
// Вход:
//	pool	- поток лексем OSL
// Выход:
//	void
// Примечания:
//	Дамп в DEFAULT_OSL_LOG_NAME

void osl_dDumpLexemStream(vector<OSL_LEXEM>* lexem_stream)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Dumping lexem stream...");

	/* char str[32];
	//string dump = "\t";
	for(vector<OSL_LEXEM>::iterator it = lexem_stream->begin();
		it != lexem_stream->end();
		it++)
	{
		OSL_LEXEM l = *it;
		//dump += l.token += " ";
		
		switch(l.type)
		{
		case ttVarType: sprintf(str, "(%s) %s\0", "ttVarType", l.token.c_str()); break;
		case ttOperator: sprintf(str, "(%s) %s\0", "ttOperator", l.token.c_str()); break;
		case ttStrConstant: sprintf(str, "(%s) %s\0", "ttStrConstant", l.token.c_str()); break;
		case ttIntConstant: sprintf(str, "(%s) %s\0", "ttIntConstant", l.token.c_str()); break;
		case ttFloatConstant: sprintf(str, "(%s) %s\0", "ttFloatConstant", l.token.c_str()); break;
		case ttValidName: sprintf(str, "(%s) %s\0", "ttValidName", l.token.c_str()); break;
		case ttSysFunction: sprintf(str, "(%s) %s\0", "ttSysFunction", l.token.c_str()); break;
		case ttSysConstant: sprintf(str, "(%s) %s\0", "ttSysConstant", l.token.c_str()); break;
		case ttSemicolon: sprintf(str, "(%s) %s\0", "ttSemicolon", l.token.c_str()); break;
		case ttComma: sprintf(str, "(%s) %s\0", "ttComma", l.token.c_str()); break;
		case ttEqualColon: sprintf(str, "(%s) %s\0", "ttEqualcolon", l.token.c_str()); break;
		case ttLeftBracket: sprintf(str, "(%s) %s\0", "ttLeftBracket", l.token.c_str()); break;
		case ttRightBracket: sprintf(str, "(%s) %s\0", "ttRightBracket", l.token.c_str()); break;
		case ttLeftBrace: sprintf(str, "(%s) %s\0", "ttLeftBrace", l.token.c_str()); break;
		case ttRightBrace: sprintf(str, "(%s) %s\0", "ttRightBrace", l.token.c_str()); break;
		case ttUnknown: sprintf(str, "(%s) %s\0", "ttUnknown", l.token.c_str()); break;
		//case ttComparison: sprintf(type, "\tOSL_LEXEM type: ttComparison"); break;
		case ttKeyword: sprintf(str, "(%s) %s\0", "ttKeyword", l.token.c_str()); break;
		case ttSys: sprintf(str, "(%s) %s\0", "ttSys", l.token.c_str()); break;
		default: break;
		}
		WriteToLog(DEFAULT_OSL_LOG_NAME, str);
	}
	*/
	//WriteToLog(DEFAULT_OSL_LOG_NAME, dump.c_str());
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Finished dumping");
}



// Добавление в лог сообщения об ошибке
// Вход:
//	file		- имя файла, в котором произошла ошибка
//	function	- имя функции, в которой произошла ошибка
//	line		- номер строки, в которой произошла ошибка
// Выход:
//	void
// Примечания:
//	Ошибка пишется в DEFAULT_OSL_LOG_NAME
//	Используются макросы __FILE__, __FUNCTION__, __LINE__

void osl_dMacroError(char* file, char* function, int line)
{
	char buffer[192];

	sprintf_s(buffer, 192, "Error: file(%s); function(%s); line(%u)", file, function, line);
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(buffer));
}



// Дамп массива строк
// Вход:
//	list	- массив строк
// Выход:
//	void
// Примечания:
//	Дамп в DEFAULT_OSL_LOG_NAME	

void osl_dDumpList(vector<string>* list)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Started dump");
	for(vector<string>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, *it);
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Finished dump");
}
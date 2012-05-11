#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_variables.h"
#include "osl_debug.h"


// �������� ������� ���������� ����� OSL
// ����:
//	list	- ����� ������
// �����:
//	true/false - ������� �� ��������� �������
// ����������:
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



// ���� ���� ����������
// ����:
//	pool	- ��� ���������� OSL
// �����:
//	void
// ����������:
//	���� � DEFAULT_OSL_LOG_NAME	

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



// ���� ������ ������
// ����:
//	pool	- ����� ������ OSL
// �����:
//	void
// ����������:
//	���� � DEFAULT_OSL_LOG_NAME

void osl_dDumpLexemStream(vector<OSL_LEXEM>* lexem_stream)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Dumping lexem stream...");

	//char type[50];
	string dump = "\t";
	for(vector<OSL_LEXEM>::iterator it = lexem_stream->begin();
		it != lexem_stream->end();
		it++)
	{
		OSL_LEXEM l = *it;
		dump += l.token += " ";
		
		//switch((it)->type)
		//{
		//case ttVarType: sprintf(type, "\tOSL_LEXEM type: ttVarType"); break;
		//case ttOperator: sprintf(type, "\tOSL_LEXEM type: ttOperator"); break;
		//case ttStrConstant: sprintf(type, "\tOSL_LEXEM type: ttStrConstant"); break;
		//case ttIntConstant: sprintf(type, "\tOSL_LEXEM type: ttIntConstant"); break;
		//case ttFloatConstant: sprintf(type, "\tOSL_LEXEM type: ttFloatConstant"); break;
		//case ttName: sprintf(type, "\tOSL_LEXEM type: ttIdentifier"); break;
		//case ttSysFunction: sprintf(type, "\tOSL_LEXEM type: ttSysFunction"); break;
		//case ttSysConstant: sprintf(type, "\tOSL_LEXEM type: ttSysConstant"); break;
		//case ttSemicolon: sprintf(type, "\tOSL_LEXEM type: ttSemicolon"); break;
		//case ttComma: sprintf(type, "\tOSL_LEXEM type: ttComma"); break;
		//case ttEqualcolon: sprintf(type, "\tOSL_LEXEM type: ttEqualcolon"); break;
		//case ttLeftBracket: sprintf(type, "\tOSL_LEXEM type: ttLeftBracket"); break;
		//case ttRightBracket: sprintf(type, "\tOSL_LEXEM type: ttRightBracket"); break;
		//case ttLeftBrace: sprintf(type, "\tOSL_LEXEM type: ttLeftBrace"); break;
		//case ttRightBrace: sprintf(type, "\tOSL_LEXEM type: ttRightBrace"); break;
		//case ttUnknown: sprintf(type, "\tOSL_LEXEM type: ttUnknown"); break;
		//case ttComparison: sprintf(type, "\tOSL_LEXEM type: ttComparison"); break;
		//case ttKeyword: sprintf(type, "\tOSL_LEXEM type: ttKeyword"); break;
		//case ttSys: sprintf(type, "\tOSL_LEXEM type: ttSys"); break;
		//default: break;
		//}
		//sprintf(type, "\tOSL_LEXEM token: %s", (it)->token.c_str());
		//WriteToLog(DEFAULT_OSL_LOG_NAME, type);
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, dump.c_str());
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Finished dumping");
}



// ���������� � ��� ��������� �� ������
// ����:
//	file		- ��� �����, � ������� ��������� ������
//	function	- ��� �������, � ������� ��������� ������
//	line		- ����� ������, � ������� ��������� ������
// �����:
//	void
// ����������:
//	������ ������� � DEFAULT_OSL_LOG_NAME
//	������������ ������� __FILE__, __FUNCTION__, __LINE__

void osl_dMacroError(char* file, char* function, int line)
{
	char buffer[192];

	sprintf_s(buffer, 192, "Error: file(%s); function(%s); line(%u)", file, function, line);
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(buffer));
}



// ���� ������� �����
// ����:
//	list	- ������ �����
// �����:
//	void
// ����������:
//	���� � DEFAULT_OSL_LOG_NAME	

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
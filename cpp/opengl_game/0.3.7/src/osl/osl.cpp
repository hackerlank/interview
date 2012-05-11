#include "stdafx.h"
#include "../misc.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_lexems.h"
#include "osl_parse.h"

#include "osl_debug.h"

#include "../config.h"

#include "osl.h"

unsigned int	OSL_CURRENT_LINE	= 0;
char*			OSL_CURRENT_FILE	= NULL;




// �������������
// ����:
//	template	- ��������� �� ������ �������� ���������� ����� OSL,
//					������� �������������� �������
// �����:
//	void
// ����������:
//	���������� ���� ���

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


	// do - while �� ��������
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
}



// �������� ����������� ����� OSL
// ����:
//	file_name	- ��� �����
// �����:
//	true/false - ������� �� ��������� �������
// ����������:
//	

bool osl_Load_v_04(const char* file_name)
{

	OSL_CURRENT_FILE = (char*)file_name;

	char* str = NULL;

	vector<OSL_LEXEM> lxm;
	OSL_TEMPLATE_STORAGE storage;
	vector<OSL_VAR> pool;

	bool ok = false;
	int lxm_count = 0;

	WriteToLog(DEFAULT_LOG_NAME, "Loading file: " + string(file_name));

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(file_name) + ": Input...");
#endif
	ok = osl_Input(file_name, &str);

	if(ok)
	{
		#ifdef _DEBUG_OSL
			WriteToLog(DEFAULT_OSL_LOG_NAME, string(file_name) + ": Input OK");
		#endif
	}
	else
	{
		WriteToLog(DEFAULT_LOG_NAME, string(file_name) + ": Input FAILED");
		return false;
	}

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize...");
#endif
	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

	if(ok)
	{
#ifdef _DEBUG_OSL
		char buffer[128];
		sprintf(buffer, "%s: Lexemize OK (%u lexems token)", file_name, lxm_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
#endif
		delete[] str;
	}
	else
	{
		WriteToLog(DEFAULT_LOG_NAME, string(file_name) + ": Lexemize FAILED");
		return false;
	}

#ifdef _DEBUG_OSL
	osl_dDumpLexemStream(&lxm);
#endif

	osl_InitTemplateStorage(&storage);

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Interpret...");
#endif
	ok = osl_Interpret(&lxm, &pool, &storage);

	if(ok)
	{
#ifdef _DEBUG_OSL
		WriteToLog(DEFAULT_OSL_LOG_NAME, string(file_name) + ": Interpret OK");
#endif
	}
	else
	{
		WriteToLog(DEFAULT_LOG_NAME, string(file_name) + ": Interpret FAILED");
		return false;
	}
#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(file_name) + ": File successfully loaded");
#endif
	return true;
}
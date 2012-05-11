// osl.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "misc.h"
#include "parser.h"
#include "osl.h"


bool osl_Load_v_04(const char* file_name)
{
	char* str = NULL;
	std::vector<LEXEM> lxm;
	string s;
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

	//osl_DumpInputStream(str);

	WriteToLog(DEFAULT_LOG_NAME, "Lexemize...");

	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

			if(ok)
				WriteToLog(DEFAULT_LOG_NAME, "Lexemize OK");
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Lexemize FAILED");
				return false;
			}

	//osl_DumpLexemStream(&lxm);
	WriteToLog(DEFAULT_LOG_NAME, "Making list...");
	ok = osl_MakeList(&lxm, NULL);

			if(ok)
				WriteToLog(DEFAULT_LOG_NAME, "Making list OK");
			else
			{
				WriteToLog(DEFAULT_LOG_NAME, "Making list FAILED");
				return false;
			}

	//WriteToLog(DEFAULT_LOG_NAME, "Executing list");
	//ok = ExecuteList(...);

	//		if(ok)
	//			WriteToLog(DEFAULT_LOG_NAME, "Executing list OK");
	//		else
	//		{
	//			WriteToLog(DEFAULT_LOG_NAME, "Executing list FAILED");
	//			return false;
	//		}

	WriteToLog(DEFAULT_LOG_NAME, "File successfully loaded");
	return true;
}
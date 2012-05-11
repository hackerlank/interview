#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"

void osl_Error(string err_code)
{
	WriteToLog(DEFAULT_LOG_NAME, "Error: " + err_code);
}

void osl_Error(string err_code, string hint)
{
	WriteToLog(DEFAULT_LOG_NAME, "Error: " + err_code + " (" + hint + ")");
}

void osl_Warning(string warn_code)
{
	WriteToLog(DEFAULT_LOG_NAME, "Warning: " + warn_code);
}

void osl_DumpList(char* file_name, vector<string>* list)
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
#include "stdafx.h"
#include "../osl/osl_types.h"
#include "../osl/osl_lexems.h"
#include "../osl/osl_parse.h"
#include "../osl/osl_misc.h"
#include "../osl/osl.h"
#include "../misc.h"
#include "iichan_dialog.h"

extern unsigned int OSL_CURRENT_LINE;

vector<Dialog> dialogs;

bool LoadDialog(const char* file_name)
{
	char* str = NULL;
	vector<OSL_LEXEM> lxm;

	bool ok = false;
	int lxm_count = 0;

#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Loading Dialog file: " + string(file_name));
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Input...");
#endif
	if(osl_InputFile(file_name, &str))
		ok = osl_InputString(str, &str);

	if(ok)
	{
#ifdef _DEBUG_OSL
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input OK");
#endif
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input FAILED");
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
		char buffer[64];
		sprintf(buffer, "Lexemize OK (%u lexems token)", lxm_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
#endif
		delete[] str;
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize FAILED");
		return false;
	}


#ifdef _DEBUG_OSL
	osl_dDumpLexemStream(&lxm);
#endif
#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Building Dialog");
#endif

	Dialog Dialog;
	Dialog.SetName(NameFromPath(string(file_name)));

	vector<OSL_LEXEM> sqm;
	for(vector<OSL_LEXEM>::iterator it = lxm.begin();
		it != lxm.end();
		it++)
	{
		if(!osl_IsSys((*it).token))
			sqm.push_back((*it));
		else
			OSL_CURRENT_LINE++;

		if(osl_IsSemicolon((*it).token))
		{
			if(sqm.size() != 10)
			{
				WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadDialog() error: invalid line");
			}
			else
			{
#ifdef _DEBUG_OSL
				osl_dDumpLexemStream(&sqm);
#endif
				if(!(osl_IsStrConstant(sqm[0].token) && osl_IsIntConstant(sqm[2].token) &&
					osl_IsIntConstant(sqm[4].token) && osl_IsStrConstant(sqm[6].token) &&
					osl_IsStrConstant(sqm[8].token)))
				{
					osl_Error("LoadDialog() error: invalid tokens");
				}
				else
				{
					DIALOG_MESSAGE message;
					message.name = osl_RemoveQuotesStr(sqm[0].token);
					message.turn = osl_StrToInt(sqm[2].token);
					message.time = osl_StrToInt(sqm[4].token);
					message.resource = osl_RemoveQuotesStr(sqm[6].token);
					message.text = osl_RemoveQuotesStr(sqm[8].token);

					Dialog.AddMessage(message);
					sqm.clear();
				}		
			}
		}
	}
	dialogs.push_back(Dialog);
#ifdef _DEBUG_OSL
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Dialog successfully loaded");
#endif
	return true;
}

Dialog* DialogByName(const char* name)
{
	for(vector<Dialog>::iterator it = dialogs.begin();
		it != dialogs.end();
		it++)
	{
		Dialog* d = &(*it);
		if(strcmp(d->GetName().c_str(), name) == 0)
			return d;
	}
	return NULL;
}

void FreeDialogs()
{
	dialogs.clear();
}
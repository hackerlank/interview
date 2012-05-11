#include "stdafx.h"
#include "config.h"
#include "osl_lexems.h"
#include "osl_variables.h"
#include "osl_debug.h"
#include "osl.h"
#include "misc.h"

extern unsigned int OSL_CURRENT_LINE;



void SetConfigField(config* cfg, string field, string value)
{
	if(field == "scr_width")	cfg->scr_width	= atoi(value.c_str());
	if(field == "scr_height")	cfg->scr_height	= atoi(value.c_str());
	if(field == "bpp")			cfg->scr_bpp	= atoi(value.c_str());
	if(field == "fullscreen")	cfg->fullscreen	= atoi(value.c_str()) != 0;
	if(field == "vert_sync")	cfg->vert_sync	= atoi(value.c_str()) != 0;
	if(field == "debug")		cfg->debug		= atoi(value.c_str()) != 0;
}

bool LoadConfig(char* file_name, config* cfg)
{
	char* str = NULL;
	vector<LEXEM> lxm;

	bool ok = false;
	int lxm_count = 0;

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Loading config file: " + string(file_name));

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Input...");
	ok = osl_Input(file_name, &str);

	if(ok)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input OK");
	}
	else
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "Input FAILED");
		return false;
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Lexemize...");
	lxm_count = osl_Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

	if(ok)
	{
		char buffer[50];

		sprintf(buffer, "Lexemize OK (%u lexems token)", lxm_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
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

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Applying config...");

	vector<LEXEM> sqm;
	for(vector<LEXEM>::iterator it = lxm.begin();
		it != lxm.end();
		it++)
	{

		if(!osl_IsSys((*it).token))
			sqm.push_back((*it));
		else
			OSL_CURRENT_LINE++;

		if(osl_IsSemicolon((*it).token))
		{

			if(sqm.size() != 4)
			{
				WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadConfig() error: invalid line");
				return false;
			}
			else
			{
				if(!osl_IsEqualcolon(sqm[1].token) || !osl_IsIntConstant(sqm[2].token))
				{
					WriteToLog(DEFAULT_OSL_LOG_NAME, "LoadConfig() error: invalid line");
				}
				else
				{
					#ifdef _DEBUG_OSL
					osl_dDumpLexemStream(&sqm);
					#endif
					SetConfigField(cfg, sqm[0].token, sqm[2].token);
					sqm.clear();
				}
			}
		}
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, "Applying config OK");
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Config successfully loaded");
	
	return true;
}
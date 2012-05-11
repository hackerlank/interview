#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"

extern unsigned int OSL_CURRENT_LINE;

void osl_Error(string err_code)
{
	char buffer[192];

	sprintf_s(buffer, 192, "Error: %s ; line(%u)", err_code.c_str(), OSL_CURRENT_LINE);
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Error: " + err_code);
}

void osl_Error(string err_code, string hint)
{
	char buffer[192];

	sprintf_s(buffer, 192, "Error: %s (%s); line(%u)", err_code.c_str(), hint.c_str(), OSL_CURRENT_LINE);
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(buffer));
}




void osl_Warning(string warn_code)
{
	char buffer[192];

	sprintf_s(buffer, 192, "Warning: %s; (line %u)", warn_code.c_str(), OSL_CURRENT_LINE);
	WriteToLog(DEFAULT_OSL_LOG_NAME, string(buffer));
}



string osl_IntToStr(int n)
{
	char buffer[OSL_CONVERT_BUFFER_SIZE];
	
	try
	{
		if(_itoa_s(n, buffer, OSL_CONVERT_BUFFER_SIZE, 10) != 0)
			throw("_itoa_s() FAILED");
	}
	catch(char* str)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "osl_IntToStr() EXCEPTION: " + string(str));
		return "";
	}

	return string(buffer);
}

int osl_StrToInt(string s)
{
	int result = 0;

	try
	{
		result = atoi(s.c_str());
	}
	catch(char* str)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "osl_StrToInt() EXCEPTION: " + string(str));
		return 0;
	}
	return result;
}

string osl_FloatToStr(float n)
{
	char buffer[OSL_CONVERT_BUFFER_SIZE];

	try
	{
		if(_gcvt_s(buffer, OSL_CONVERT_BUFFER_SIZE, (float)n, OSL_CONVERT_DIGITS) != 0)
			throw("_gcvt_s() FAILED");
	}
	catch(char* str)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "osl_FloatToStr() EXCEPTION: " + string(str));
		return "";
	}

	return string(buffer);
}

float osl_StrToFloat(string s)
{
	float result = 0.0;

	try
	{
		result = (float)atof(s.c_str());
	}
	catch(char* str)
	{
		WriteToLog(DEFAULT_OSL_LOG_NAME, "osl_StrToFloat() EXCEPTION: " + string(str));
		return 0.0;
	}

	return result;
}

char* osl_RemoveQuotesChr(char* str)
{
	if(str)
		if(strlen(str) > 2)
			if(str[0] == '"' && str[strlen(str)-1] == '"')
			{
				UINT l = strlen(str)-1;
				char* new_str = new char[l];

				for(UINT i = 1; i<l; i++)
					new_str[i-1] = str[i];

				new_str[l-1] = '\0';

				delete[] str;

				return new_str;
			}
	return str;
}

string osl_RemoveQuotesStr(string str)
{
	if(str[0] == '"' && str[str.length()-1] == '"')
		return str.substr(1, str.length()-2);
	else
		return str;
}



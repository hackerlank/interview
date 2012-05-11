#include "stdafx.h"
#include "../misc.h"
#include "osl_misc.h"

extern unsigned int OSL_CURRENT_LINE;
extern string OSL_CURRENT_FILE;
extern string OSL_CURRENT_PARAMS;
extern string OSL_CURRENT_FUNCTION;

void osl_Error(string err_code)
{
	
	string buffer("Error (");
	buffer += string(OSL_CURRENT_FILE) + ", line " + IntToString(OSL_CURRENT_LINE) + 
		") in '" + OSL_CURRENT_FUNCTION + "(" + OSL_CURRENT_PARAMS + ")': " + err_code;
	WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
}

void osl_Error(string err_code, string hint)
{
	string buffer("Error (");
	buffer += string(OSL_CURRENT_FILE) + ", line " + IntToString(OSL_CURRENT_LINE) + 
		"): " + err_code + " (" + hint + ")";


	WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
}

void osl_Warning(string warn_code)
{
	string buffer("Warning (");
	buffer += string(OSL_CURRENT_FILE) + ", line " + IntToString(OSL_CURRENT_LINE) + 
		") in '" + OSL_CURRENT_FUNCTION + "(" + OSL_CURRENT_PARAMS + ")': " + warn_code;
	WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
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
	if((str[0] == '"' && str[str.length()-1] == '"') || (str[0] == '\'' && str[str.length()-1] == '\''))
		return str.substr(1, str.length()-2);
	else
		return str;
}

void osl_TrimSpaces(string& str)  
{  
	static const string deprecated = " \t\n\xA\xD";
     // Trim Both leading and trailing spaces  
     size_t startpos = str.find_first_not_of(deprecated); // Find the first character position after excluding leading blank spaces  
     size_t endpos = str.find_last_not_of(deprecated); // Find the first character position from reverse af  
   
     // if all spaces or empty return an empty string  
     if(( string::npos == startpos ) || ( string::npos == endpos))  
     {  
         str = "";  
     }  
     else  
         str = str.substr( startpos, endpos-startpos+1 ); 
}

void osl_TrimComments(string& str)
{
	static const string clrf = "\xD\xA";
	int startpos = -1;
	int clrf_pos = -1;
	int quote = -1;
	int quote2 = -1;
	for(;;)
	{
		startpos = str.find_first_of(COMMENT_CHAR, startpos + 1);
		clrf_pos = str.find_first_of(clrf, startpos + 1);

		if(clrf_pos == str.npos)
			clrf_pos = str.length()-1;

		if(startpos != str.npos)
		{
			str.erase(startpos, clrf_pos - startpos + 1);
		}
		else
			break;
	}


	//size_t startpos = str.find_first_of(COMMENT_CHAR);
	//size_t quote = str.find_first_of('"\'');
	//size_t quote2 = str.find_first_of('"\'', quote + 1);
	//static const string clrf = "\xD\n";
	//size_t clrf_pos = str.find_last_of(clrf, quote2 + 1);

	//if(startpos < quote || startpos > quote2)
	//	str = str.substr(0, startpos) + str.substr(clrf_pos+1, str.length() - clrf_pos + 1);
}
#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"

extern unsigned int OSL_CURRENT_LINE;

bool osl_Input(const char* file_name, char** dest)
{
	FILE *file;
	file = fopen(file_name, "rt");
	OSL_CURRENT_LINE = 0;
	if(!file)
	{
		WriteToLog(DEFAULT_LOG_NAME, "Failed to open " + string(file_name));
		return false;
	}
	else
	{
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);

		char* buf = new char[length+1];
		int l = 0;

		bool inside = false;
		bool comment = false;
		while(feof(file) == 0)
		{
			char tk;

			fread(&tk, 1, 1, file);

			if(tk == '"')
			{
				inside = !inside;
			}
			else if(tk == COMMENT_CHAR)
			{
				comment = true;
			}

			if(isspace(tk) == 0 || inside)
			{
				if(!comment)
				{
					buf[l] = tk;
					l++;
				}
			}
			else if(tk == '\n')
			{
				buf[l] = SKIP_CHAR;
				comment = false;
				l++;
			}
		}
		buf[l-1] = '\0';
		fclose(file);
		*dest = new char[l+1];
		strncpy(*dest, buf, l);
		delete[] buf;
		return true;
	}
}

E_TOKEN_TYPE osl_GetToken(string s)
{
	if(osl_IsSemicolon(s))			return ttSemicolon;
	else if(osl_IsComma(s))			return ttComma;
	else if(osl_IsComparison(s))	return ttComparison;
	else if(osl_IsEqualcolon(s))	return ttEqualcolon;
	else if(osl_IsLeftBracket(s))	return ttLeftBracket;
	else if(osl_IsRightBracket(s))	return ttRightBracket;
	else if(osl_IsLeftBrace(s))		return ttLeftBrace;
	else if(osl_IsRightBrace(s))	return ttRightBrace;
	else if(osl_IsOperator(s))		return ttOperator;
	else if(osl_IsVarType(s))		return ttVarType;
	else if(osl_IsStrConstant(s))	return ttStrConstant;
	else if(osl_IsIntConstant(s))	return ttIntConstant;
	else if(osl_IsFloatConstant(s))	return ttFloatConstant;
	else if(osl_IsSysFunction(s))	return ttSysFunction;
	else if(osl_IsSysConstant(s))	return ttSysConstant;
	else if(osl_IsKeyword(s))		return ttKeyword;
	else if(osl_IsValidName(s))		return ttName;
	else if(osl_IsSys(s))			return ttSys;
	else							return ttUnknown;
}

bool osl_IsSemicolon(string s)		{ if(s == ";") return true; else return false; }
bool osl_IsComma(string s)			{ if(s == ",") return true; else return false; }
bool osl_IsEqualcolon(string s)		{ if(s == "=") return true; else return false; }
bool osl_IsLeftBracket(string s)	{ if(s == "(") return true; else return false; }
bool osl_IsRightBracket(string s)	{ if(s == ")") return true; else return false; }
bool osl_IsLeftBrace(string s)		{ if(s == "{") return true; else return false; }
bool osl_IsRightBrace(string s)		{ if(s == "}") return true; else return false; }
bool osl_IsOperator(string s)		{ for(UINT i=0;i<l_operator;i++)if(s_operator[i]==s)return true;return false; }
bool osl_IsComparison(string s)		{ for(UINT i=0;i<l_comparison;i++)if(s_comparison[i]==s)return true;return false; }
bool osl_IsVarType(string s)		{ for(UINT i=0;i<l_OSL_VAR_TYPE;i++)if(s_OSL_VAR_TYPE[i]==s)return true;return false; }
bool osl_IsStrConstant(string s)	{ if(s.length()>1)if(s[0]=='"' && s[s.length()-1]=='"')return true; else return false; else return false; }
bool osl_IsIntConstant(string s)	{ if(IsIntNumber(s)) return true; else return false; }
bool osl_IsFloatConstant(string s)	{ if(IsFloatNumber(s)) return true; else return false; }
bool osl_IsSysFunction(string s)	{ for(UINT i=0;i<l_sysfunction;i++)if(s_sysfunction[i]==s)return true;return false; }
bool osl_IsSysConstant(string s)	{ for(UINT i=0;i<l_sysconstant;i++)if(s_sysconstant[i]==s)return true;return false; }
bool osl_IsKeyword(string s)		{ for(UINT i=0;i<l_keyword;i++)if(s_keyword[i]==s)return true;return false; }
bool osl_IsSys(string s)			{ for(UINT i=0;i<l_sys;i++)if(s_sys[i]==s)return true;return false; }
bool osl_IsConstant(string s)
{
	if(osl_IsIntConstant(s))
		return true;
	if(osl_IsFloatConstant(s))
		return true;
	if(osl_IsStrConstant(s))
		return true;
	if(osl_IsSysConstant(s))
		return true;
	return false;
}
bool osl_IsValidName(string s)
{
	static const string deprecated = "!@#$%^&+-*~`/|%=<>(){}[].,.;\'\"";
	if(s.find_first_of(deprecated) != s.npos)
		return false;

	if(isdigit(s[0]))
		return false;

	if(osl_IsConstant(s))
		return false;

	return true;	
}
								
int osl_Lexemize(char* char_stream, std::vector<LEXEM>* lexem_stream)
{
	static const string delim = "\"!+*=/%-,;(){}[]<>|";

	string src(char_stream);
	int lexem_count = 0;
	LEXEM lexem;

	int src_count = src.length();

	bool minus = false;
	LEXEM reserved;

	int i = 0;
	bool inside = false;
	bool was = false;
	string body;
	int delimpos = src.npos;
	if(src[src.length()-1] != ';' && src[src.length()-1] != '}')
	{
		osl_Error("';' expected");
		return -1;
	}
	while(i < src_count)
	{
		if(src[i] == '"')
		{
			int p = src.find_first_of('"', i+1);

			if(p == src.npos)
				WriteToLog(DEFAULT_LOG_NAME, "\" expected");
			else
			{
				body = src.substr(i, p - i + 1);
				i = p + 1;
				lexem_count++;
				lexem.type = osl_GetToken(body);
				lexem.token = body;					
				lexem_stream->push_back(lexem);

			}
			continue;
		}
		else
			delimpos = src.find_first_of(delim, i);
		
		if(delimpos == src.npos)
		{
			lexem_count++;
			body = src.substr(i);
			lexem.type = osl_GetToken(body);
			lexem.token = body;
			lexem_stream->push_back(lexem);
			//if(lexem.type == ttOperator)
			//{
			//	if(lexem.token == "-")
			//	{				
			//		reserved = lexem;
			//		minus = true;
			//	}
			//	else
			//	{
			//		
			//		lexem_stream->push_back(lexem);
			//	}
			//}
			//else if(lexem.type == ttIntConstant || lexem.type == ttFloatConstant)
			//{
			//	if(minus)
			//	{
			//		reserved.token = "+";
			//		lexem_stream->push_back(reserved);
			//		if(osl_IsIntConstant(lexem.token))
			//		{
			//			lexem.token = osl_IntToStr(-osl_StrToInt(lexem.token));

			//			lexem_stream->push_back(lexem);
			//			minus = false;
			//		}
			//		else if(osl_IsFloatConstant(lexem.token))
			//		{
			//			lexem.token = osl_FloatToStr(-osl_StrToFloat(lexem.token));
			//			lexem_stream->push_back(lexem);
			//			minus = false;
			//		}
			//	}
			//}
			//else
			//{
			//	if(minus)
			//	{
			//		minus = false;
			//		lexem_stream->push_back(reserved);
			//	}
			//}
		}
		else
		{
			lexem_count++;

			body = src.substr(i, max(delimpos - i, unsigned(1)));
			i = max(delimpos, i + 1);

			// костыль, но что поделать
			if(body != " ")
			{
				lexem.type = osl_GetToken(body);
				lexem.token = body;
				if(lexem.type == ttOperator)
				{
					if(lexem.token == "-")
					{				
						reserved = lexem;
						minus = true;
					}
					else
					{
						
						lexem_stream->push_back(lexem);
					}
				}
				else if(lexem.type == ttIntConstant || lexem.type == ttFloatConstant)
				{
					if(minus)
					{
						reserved.token = "+";
						lexem_stream->push_back(reserved);
						if(osl_IsIntConstant(lexem.token))
						{
							lexem.token = osl_IntToStr(-osl_StrToInt(lexem.token));

							lexem_stream->push_back(lexem);
							minus = false;
						}
						else if(osl_IsFloatConstant(lexem.token))
						{
							lexem.token = osl_FloatToStr(-osl_StrToFloat(lexem.token));
							lexem_stream->push_back(lexem);
							minus = false;
						}
					}
					else
					{
						lexem_stream->push_back(lexem);
					}
				}
				else
				{
					if(minus)
					{
						minus = false;
						lexem_stream->push_back(reserved);
					}
					else
						lexem_stream->push_back(lexem);
				}
			}

		}
	}
	return lexem_count;
}


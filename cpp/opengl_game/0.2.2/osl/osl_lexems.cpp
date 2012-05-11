#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"

bool osl_Input(const char* file_name, char** dest)
{
	FILE *file;
	file = fopen(file_name, "rt");
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
		while(feof(file) == 0)
		{
			char tk;

			fread(&tk, 1, 1, file);

			if(tk == '"')
				inside = !inside;


			if(isspace(tk) == 0 || inside)
			{
				buf[l] = tk;
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
bool osl_IsStrConstant(string s)	{ if(s.length()>1)if(s[0]=='"'&&s[s.length()-1]=='"')return true; else return false; }
bool osl_IsIntConstant(string s)	{ if(IsIntNumber(s)) return true; else return false; }
bool osl_IsFloatConstant(string s)	{ if(IsFloatNumber(s)) return true; else return false; }
bool osl_IsSysFunction(string s)	{ for(UINT i=0;i<l_sysfunction;i++)if(s_sysfunction[i]==s)return true;return false; }
bool osl_IsSysConstant(string s)	{ for(UINT i=0;i<l_sysconstant;i++)if(s_sysconstant[i]==s)return true;return false; }
bool osl_IsKeyword(string s)		{ for(UINT i=0;i<l_keyword;i++)if(s_keyword[i]==s)return true;return false; }
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
	static const string deprecated = "!@#$%^&+-*~`/%=<>(){}[].,.;\'\"";
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
	static const string delim = "\"!+*=/%-.,;(){}[]<> ";

	string src(char_stream);
	int lexem_count = 0;
	LEXEM lexem;

	int src_count = src.length();

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
				WriteToLog(DEFAULT_LOG_NAME, S1001);
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
				lexem_stream->push_back(lexem);
			}

		}
	}
	return lexem_count;
}

void osl_DumpLexemStream(vector<LEXEM>* lexem_stream)
{
	WriteToLog(DEFAULT_LOG_NAME, "Dumping lexem stream...");

	char type[50];
	for(vector<LEXEM>::iterator it = lexem_stream->begin();
		it != lexem_stream->end();
		it++)
	{
		switch((it)->type)
		{
		case ttVarType:
			sprintf(type, "\tLexem type: ttVarType");
			break;
		case ttOperator:
			sprintf(type, "\tLexem type: ttOperator");
			break;
		case ttStrConstant:
			sprintf(type, "\tLexem type: ttStrConstant");
			break;
		case ttIntConstant:
			sprintf(type, "\tLexem type: ttIntConstant");
			break;
		case ttFloatConstant:
			sprintf(type, "\tLexem type: ttFloatConstant");
			break;
		case ttName:
			sprintf(type, "\tLexem type: ttIdentifier");
			break;
		case ttSysFunction:
			sprintf(type, "\tLexem type: ttSysFunction");
			break;
		case ttSysConstant:
			sprintf(type, "\tLexem type: ttSysConstant");
			break;
		case ttSemicolon:
			sprintf(type, "\tLexem type: ttSemicolon");
			break;
		case ttComma:
			sprintf(type, "\tLexem type: ttComma");
			break;
		case ttEqualcolon:
			sprintf(type, "\tLexem type: ttEqualcolon");
			break;
		case ttLeftBracket:
			sprintf(type, "\tLexem type: ttLeftBracket");
			break;
		case ttRightBracket:
			sprintf(type, "\tLexem type: ttRightBracket");
			break;
		case ttLeftBrace:
			sprintf(type, "\tLexem type: ttLeftBrace");
			break;
		case ttRightBrace:
			sprintf(type, "\tLexem type: ttRightBrace");
			break;
		case ttUnknown:
			sprintf(type, "\tLexem type: ttUnknown");
			break;
		case ttComparison:
			sprintf(type, "\tLexem type: ttComparison");
			break;
		case ttKeyword:
			sprintf(type, "\tLexem type: ttKeyword");
			break;
		default:
			break;
		}

		WriteToLog(DEFAULT_LOG_NAME, type);

		sprintf(type, "\tLexem token: %s", (it)->token.c_str());
		WriteToLog(DEFAULT_LOG_NAME, type);

	}

	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}
#include "stdafx.h"
#include "data.h"
#include "misc.h"
#include "interpret.h"

enum I_TOKEN_TYPE { ttKeyword, ttOperator, ttStrConstant,
ttIntConstant, ttFloatConstant, ttVariable, ttFunction, ttDivider, ttSysConstant, ttUnknown };

bool Input(const char* file_name, char** dest)
{
	FILE *file;
	file = fopen(file_name, "rt");
	if(!file)
	{
		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Failed to open " + string(file_name));
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
		buf[l] = '\0';
		fclose(file);
		*dest = new char[l+1];
		strncpy(*dest, buf, l+1);
		delete[] buf;
		return true;
	}
}

void DumpInputStream(char* stream)
{
	string s(stream);
	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Dumping input stream...");
	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, s);
	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Finished dumping");
}

typedef struct tagLEXEM {
	I_TOKEN_TYPE type;
	char* token;

	tagLEXEM(I_TOKEN_TYPE t): type(t), token(NULL) {}
	tagLEXEM(): type(ttUnknown), token(NULL) {}
} LEXEM;

void ExpandLexemStream(LEXEM*& lexem_stream, int current_size)
{
	LEXEM* temp = new LEXEM[current_size+1];

	memcpy_s((LEXEM*)temp, (current_size+1), (LEXEM*)lexem_stream, current_size);

	if(lexem_stream) delete[] lexem_stream;

	lexem_stream = new LEXEM[current_size+1];

	memcpy_s((LEXEM*)lexem_stream, (current_size+1), (LEXEM*)temp, (current_size) /* * sizeof(LEXEM) */);

	delete[] temp;
}

void DumpLexemStream(std::vector<LEXEM>* lexem_stream)
{
	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Dumping lexem stream...");

	char type[50];
	for(std::vector<LEXEM>::iterator it = lexem_stream->begin();
		it != lexem_stream->end();
		it++)
	{
		switch((it)->type)
		{
		case ttKeyword:
			sprintf(type, "\tLexem type: ttKeyword");
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
		case ttVariable:
			sprintf(type, "\tLexem type: ttVariable");
			break;
		case ttFunction:
			sprintf(type, "\tLexem type: ttFunction");
			break;
		case ttDivider:
			sprintf(type, "\tLexem type: ttDivider");
			break;
		case ttUnknown:
			sprintf(type, "\tLexem type: ttUnknown");
			break;
		case ttSysConstant:
			sprintf(type, "\tLexem type: ttSysConstant");
			break;
		default:
			break;
		}

		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, type);

		if((it)->token)
		{
			sprintf(type, "\tLexem token: %s", (it)->token);
			WriteToLog(DEFAULT_INTERPRET_LOG_NAME, type);
		}

	}

	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Finished dumping");
}

const string a_dividers[] = { ",", ".", ";", "(", ")", "{", "}", "[", "]" }; 
const int a_dividers_l = 9;

const string a_operators[] = { "=", "+", "==", "!=", "-", "*", "/", "%", "++", "<", "<=", ">", ">=" };
const int a_operators_l = 12;

const string a_functions[] = { "print", "textout", "debug", "glEnable", "glDisable", "tostr" };
const int a_functions_l = 6;

const string a_keywords[] = { "goto", "int", "break", "for", "float", "string" };
const int a_keywords_l = 6;

const string a_sysconstants[] = { "GL_TEXTURE_2D", "GL_BLEND" };
const int a_sysconstants_l = 2;

const string a_deprecated[] = { "$", "@", "!", "&", "-", "+", "%", "*", "^", "=" };
const int a_deprecated_l = 10;

bool IsDivider(string s)
{
	for(int i = 0; i < a_dividers_l; i++)
		if(strcmp(s.c_str(), a_dividers[i].c_str()) == 0)
			return true;
	return false;
}

bool IsOperator(string s)
{
	for(int i = 0; i < a_operators_l; i++)
		if(strcmp(s.c_str(), a_operators[i].c_str()) == 0)
			return true;
	return false;
}

bool IsFunction(string s)
{
	for(int i = 0; i < a_functions_l; i++)
		if(strcmp(s.c_str(), a_functions[i].c_str()) == 0)
			return true;
	return false;
}

bool IsKeyword(string s)
{
	for(int i = 0; i < a_keywords_l; i++)
		if(strcmp(s.c_str(), a_keywords[i].c_str()) == 0)
			return true;
	return false;
}

bool IsSysConstant(string s)
{
	for(int i = 0; i < a_sysconstants_l; i++)
		if(strcmp(s.c_str(), a_sysconstants[i].c_str()) == 0)
			return true;
	return false;
}

bool IsStrConstant(string s)
{
	if(s.length() > 1)
		if(s[0] == '"')
			if(s[s.length()-1] == '"')
				return true;
			else
				return false;
		else
			return false;
	else
		return false;
}

bool IsVariable(string s)
{
	for(int i = 0; i < a_deprecated_l; i++)
		if(strcmp(s.c_str(), a_deprecated[i].c_str()) == 0)
			return false;
	return true;	
}

I_TOKEN_TYPE GetToken(string s)
{
	if(IsDivider(s))			return ttDivider;
	else if(IsOperator(s))		return ttOperator;
	else if(IsFunction(s))		return ttFunction;
	else if(IsKeyword(s))		return ttKeyword;
	else if(IsSysConstant(s))	return ttSysConstant;
	else if(IsIntNumber(s))		return ttIntConstant;
	else if(IsFloatNumber(s))	return ttFloatConstant;
	else if(IsStrConstant(s))	return ttStrConstant;
	else if(IsVariable(s))		return ttVariable;
	else						return ttUnknown;
}

int Lexemize(const char* char_stream, std::vector<LEXEM>* lexem_stream)
{
	static const string delim = "\"!+*=/%-.,;(){}[]<>";

	string src(char_stream);
	int lexem_count = 0;
	LEXEM lexem;

	int src_count = src.length();

	int i = 0;
	bool inside = false;
	bool was = false;
	string body;
	int delimpos = src.npos;
	while(i < src_count)
	{
		if(src[i] == '"')
		{
			int p = src.find_first_of('"', i+1);

			if(p == src.npos)
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Syntax error: \" expected");
			else
			{
				body = src.substr(i, p - i + 1);
				i = p + 1;
				lexem_count++;
				lexem.type = GetToken(body);
				lexem.token = new char[body.length()];
				strcpy(lexem.token, body.c_str());
				lexem_stream->push_back(lexem);
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "DEBUG (BODY): " + body);
			}
			continue;
		}
		else
			delimpos = src.find_first_of(delim, i);
		
		if(delimpos == src.npos)
		{
			lexem_count++;
			body = src.substr(i);
			lexem.type = GetToken(body);
			lexem.token = new char[body.length()];
			strcpy(lexem.token, body.c_str());
			lexem_stream->push_back(lexem);
		}
		else
		{
			lexem_count++;
			body = src.substr(i, max(delimpos - i, unsigned(1)));
			i = max(delimpos, i + 1);
			lexem.type = GetToken(body);
			lexem.token = new char[body.length()];
			strcpy(lexem.token, body.c_str());
			lexem_stream->push_back(lexem);
		}

		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "DEBUG (BODY): " + body);
	}
	return lexem_count;
}

typedef struct tagINT_VAR {
	string name;
	int value;
} INT_VAR;

typedef struct tagFLOAT_VAR {
	string name;
	float value;
} FLOAT_VAR;

typedef struct tagSTRING_VAR {
	string name;
	string value;
} STRING_VAR;



bool MakeList(std::vector<LEXEM>* lexem_stream, std::vector<string>* list)
{
	std::vector<INT_VAR> int_pool;
	std::vector<FLOAT_VAR> float_pool;
	std::vector<STRING_VAR> string_pool;


	for(std::vector<LEXEM>::iterator it = lexem_stream->begin();
		it != lexem_stream->end();
		it++)
	{
		LEXEM lexem = (*it);

		switch(lexem.type)
		{
		case ttKeyword:
			if(strcmp("int", lexem.token) == 0)
			{
				// INT
			}
			break;
		default: break;
		}

	}


}





bool Load_v_04(const char* file_name)
{
	char* str = NULL;
	std::vector<LEXEM> lxm;
	string s;
	bool ok = false;
	int lxm_count = 0;

	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Loading file: " + string(file_name));

	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Input...");
	ok = Input(file_name, &str);

			if(ok)
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Input OK");
			else
			{
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Input FAILED");
				return false;
			}

	DumpInputStream(str);

	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Lexemize...");

	lxm_count = Lexemize(str, &lxm);
	lxm_count > 0 ? ok = true : ok = false;

			if(ok)
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Lexemize OK");
			else
			{
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Lexemize FAILED");
				return false;
			}

	DumpLexemStream(&lxm);
	//WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Making list...");
	//ok = MakeList(...,...);

	//		if(ok)
	//			WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Making list OK");
	//		else
	//		{
	//			WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Making list FAILED");
	//			return false;
	//		}

	//WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Executing list");
	//ok = ExecuteList(...);

	//		if(ok)
	//			WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Executing list OK");
	//		else
	//		{
	//			WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "Executing list FAILED");
	//			return false;
	//		}

	WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "File successfully loaded");
	return true;
}




				














//const string keywords[] = { "if", "else", "for", "while", "do" };
//const string functions[] = { "TextOut", "LogToFile", "Load", "SearchFiles", "glEnable", "glClearColor" };
//const string constants[] = { "GL_TEXTURE_2D" };
//const string operators[] = { "=", "==", "+", "-", "*", "/", ">", "<", ">=", "<=", "%" };
//const string dividers[] = { ",", ".", ";", "{", "}", "(", ")", "[", "]" };

//typedef struct tagLEXEM_DATA {
//	vector<string> lexems;
//	int allCount;
//	int curCount;
//	string GetNext(void)
//	{
//		curCount++;
//		return lexems[curCount-1];
//	}
//
//	tagLEXEM_DATA()
//	{
//		lexems.resize(0);
//		allCount = 0;
//		curCount = 0;
//	}
//} LEXEM_DATA;
//
//void ClearSpacing(string& from)
//{
//	if(!from.empty())
//	{
//		int s_pos;
//		while((s_pos = from.find(' ')) != from.npos);
//		{
//			from.erase(s_pos, 1);
//		} 
//
//#ifdef _DEBUG_INTERPRET
//		//WriteToLog("Removed spaces: " + from);
//#endif
//	}
//}

//bool Load_v_03(const char* file_name)
//{
//	FILE* f = fopen(file_name, "rt");
//
//	fseek(f, 0, SEEK_END);
//	long len = ftell(f);
//	fseek(f, 0, SEEK_SET);
//
//	char* buf = new char[len];
//
//	string body;
//	fread(&buf[0], 1, len, f);
//
//	body = string(buf);
//	body.resize(len);
//
//	ClearSpacing(body);
//
//	WriteToLog("Body_v_03: " + body);
//
//	
//
//
//
//
//
//
//
//
//	fclose(f);
//	return 0;
//}


















































const LPSTR keywords[] = { "GUIDEFINITION", "RACEDEFINITION", "STARDEFINITION", "for", "while", "do", "if" };
const int k_l = 7;
// condition type, for parse record

extern GAMEDEFINITION gamedef;
extern GL_DEFINITION_STORAGE gl_def_storage;
extern KEYWORD_STORAGE key_storage;



#define _DEBUG_NEW_LOAD



// struct, which we will take as result of parsing
// it contains information, which we should use to build
// execution code
struct PARSE_RECORD {
	KEYWORD key;
	int min;
	CONDITION cnd;
	int max;
	vector<string> what_to_do;

	PARSE_RECORD()
	{
		key = kwNo;
		min = 0;
		max = 0;
		cnd = NOCOND;
		what_to_do.resize(0);
	}
};

void InitStorages()
{
	gl_def_storage.constants.fields.resize(0);
	gl_def_storage.functions.fields.resize(0);

	gl_def_storage.functions.Add("glEnable", GLFUNCTION(glEnable), 1);
	gl_def_storage.functions.Add("glBlendFunc", GLFUNCTION(glBlendFunc), 2);
	gl_def_storage.functions.Add("glAlphaFunc", GLFUNCTION(glAlphaFunc), 2);
	gl_def_storage.functions.Add("glClearColor", GLFUNCTION(glClearColor), 4);

	gl_def_storage.constants.Add("GL_TEXTURE_2D", GL_TEXTURE_2D);
	gl_def_storage.constants.Add("GL_BLEND", GL_BLEND);

	gl_def_storage.constants.Add("GL_ZERO", GL_ZERO);
	gl_def_storage.constants.Add("GL_ONE", GL_ONE);
	gl_def_storage.constants.Add("GL_SRC_COLOR", GL_SRC_COLOR);
	gl_def_storage.constants.Add("GL_ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR);
	gl_def_storage.constants.Add("GL_SRC_ALPHA", GL_SRC_ALPHA);
	gl_def_storage.constants.Add("GL_ONE_MINUS_SRC_ALPHA", GL_ONE_MINUS_SRC_ALPHA);
	gl_def_storage.constants.Add("GL_DST_ALPHA", GL_DST_ALPHA);
	gl_def_storage.constants.Add("GL_DST_COLOR", GL_DST_COLOR);
	gl_def_storage.constants.Add("GL_ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR);
	gl_def_storage.constants.Add("GL_SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE);
	gl_def_storage.constants.Add("GL_TRUE", GL_TRUE);
	gl_def_storage.constants.Add("GL_FALSE", GL_FALSE);
	gl_def_storage.constants.Add("GL_BYTE", GL_BYTE);
	gl_def_storage.constants.Add("GL_UNSIGNED_BYTE", GL_UNSIGNED_BYTE);
	gl_def_storage.constants.Add("GL_SHORT", GL_SHORT);
	gl_def_storage.constants.Add("GL_INT", GL_INT);
	gl_def_storage.constants.Add("GL_UNSIGNED_INT", GL_UNSIGNED_INT);
	gl_def_storage.constants.Add("GL_FLOAT", GL_FLOAT);
	gl_def_storage.constants.Add("GL_2_BYTES", GL_2_BYTES);
	gl_def_storage.constants.Add("GL_3_BYTES", GL_3_BYTES);
	gl_def_storage.constants.Add("GL_4_BYTES", GL_4_BYTES);
	gl_def_storage.constants.Add("GL_DOUBLE", GL_DOUBLE);

	gl_def_storage.constants.Add("GL_GREATER", GL_GREATER);

	key_storage.Add("for", kwFor);
	key_storage.Add("while", kwWhile);
	key_storage.Add("do", kwDo);
	key_storage.Add("if", kwIf);
	key_storage.Add("GUIDEFINITION", kwGuiDef);
	key_storage.Add("RACEDEFINITION", kwRaceDef);
	key_storage.Add("STARDEFINITION", kwStarDef);

}

void FillParseRecordToDoList(PARSE_RECORD* rec, string* body)
{
	do
	{
		int sc_pos = body->find(';');
		if(sc_pos == body->npos)
		{
			char output[50];
			sprintf(output, "Syntax error int line '%s': ';' expected", *body);
			WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
			break;
		}
		string op = body->substr(0, sc_pos);
		rec->what_to_do.push_back(op);

		// erasing this substring to take new one
		body->erase(0, sc_pos+1);
	}							
	while(body->length() > 0);
}

string GetFunctionName(string* from)
{
	int bracket_pos = from->find('(');
	if(bracket_pos == from->npos)
	{
		char output[50];
		sprintf(output, "Error in 'GetFunctionName(%s)'", *from);
		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
		return "";
	}
	else
		return from->substr(0, from->find('('));
}

int GetParamCount(string* from)
{
	int o_bracket_pos = from->find('(');
	int c_bracket_pos = from->find(')');
	if(o_bracket_pos == from->npos || c_bracket_pos == from->npos)
	{
		char output[50];
		sprintf(output, "Error in 'GetFunctionName(%s)'", *from);
		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
		return -1;
	}
	else
	{
		int c_count = 0;
		int c_pos = from->npos;
		do
		{
			c_pos = from->find(',', c_pos);
			if(c_pos != from->npos)
				c_count++;
		} while(c_pos != from->npos);

		if(c_count == 0)
			return 1;
		else
			return c_count;
	}
}


string GetNParam(string* from, int n)
{
	int o_bracket_pos = from->find('(');
	int c_bracket_pos = from->find(')');
	if(o_bracket_pos == from->npos || c_bracket_pos == from->npos)
	{
		char output[50];
		sprintf(output, "Error in 'GetFunctionName(%s)'", *from);
		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
		return "";
	}
	else
	{
		bool comma_exist = (from->find(',') != from->npos);
		if(n == 1)
		{
			if(!comma_exist)
				return from->substr(o_bracket_pos + 1, c_bracket_pos - o_bracket_pos - 1);
			else
			{
				int comma_pos = from->find(',');
				return from->substr(o_bracket_pos + 1, c_bracket_pos - comma_pos - 1);
			}
		}
		else
		{
			if(!comma_exist)
			{
				char output[50];
				sprintf(output, "Error in 'GetNParam(%s)'", *from);
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
				return "";
			}
			else
			{

				int comma_pos = from->npos;
				int comma_count = 0;
				int next_comma_pos = from->npos;
				do
				{
					comma_pos = from->find(',', comma_pos);
					comma_count++;
					next_comma_pos = from->find(',', comma_pos);
					if(next_comma_pos == from->npos)
						next_comma_pos = from->find(')', comma_pos);

				} while(comma_count != n - 1);
				if(comma_pos == from->npos || next_comma_pos == from->npos)
				{
					char output[50];
					sprintf(output, "Error in 'GetNParam(%s)'", *from);
					WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
					return "";
				}
				else
					return from->substr(comma_pos + 1, next_comma_pos - comma_pos - 1);
			}
		}
	}
}

bool FillParseHeaderByKey(PARSE_RECORD* rec, KEYWORD key, string* body)
{
	char output[50];
	int cnd;
	int cnd_pos;
	int cnd_pos1;
	switch(key)
	{
	case kwFor:
		{
			int eq_pos = body->find('=');
			if(eq_pos == body->npos)
			{
				sprintf(output, "Syntax error in line '%s': '=' expected", *body);
				WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
				return false;
			}
			else
			{
				int sc_pos = body->find(';');
				if(sc_pos == body->npos)
				{
					sprintf(output, "Syntax error in line '%s': ';' expected", *body);
					WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
					return false;
				}
				else
				{
					string min = body->substr(eq_pos + 1, body->length() - eq_pos  - (body->length() - sc_pos) - 1);
					rec->min = atoi(min.c_str());
					cnd_pos = body->npos;
					cnd_pos1 = body->npos;         
					cnd_pos = body->find('<');
					if(cnd_pos == body->npos)
					{
						cnd_pos = body->find('>');
						if(cnd_pos == body->npos)
						{
							sprintf(output, "Syntax error in line '%s': '>' or '<' expected", *body);
							WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
							return false;
						}
						cnd_pos1 = body->find('=', cnd_pos);
						if(cnd_pos1 != body->npos)
						{
							cnd = 3;
							rec->cnd = CONDITION(cnd);
						}
						else
						{
							cnd = 2;
							rec->cnd = CONDITION(cnd);
						}
					}
					else
					{
						cnd_pos1 = body->find('=', cnd_pos);
						if(cnd_pos1 != body->npos)
						{
							cnd = 1;
							rec->cnd = CONDITION(cnd);
						}
						else
						{
							cnd = 0;
							rec->cnd = CONDITION(cnd);
						}
					}
				}
				int sc_pos2 = body->find(';', cnd_pos);
				if(sc_pos2 == body->npos)
				{
					sprintf(output, "Syntax error in line '%s': ';' expected", *body);
					WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
					return false;
				}										
				else
				{
					int _dx;
					cnd_pos1 != body->npos ? _dx = 1 : _dx = 0;
					string max = body->substr(cnd_pos + _dx + 1, body->length() - (cnd_pos + _dx + 1)  - (body->length() - cnd_pos1) - 1);
					rec->max = atoi(max.c_str());
					return true;
				}
			}
			break;
		}
	default: return true;
	}
}

void Execute(string* s)
{
	string name = GetFunctionName(s);
	GLFUNCTION func = gl_def_storage.functions.ByName(name);
	int param_count = GetParamCount(s);

	float* params = new float[param_count];

	(*func)(params);

}























bool Load(char* file_name)
{
	ifstream f;
	f.open(file_name, ios::in);
	// if not opened
	if(!f.is_open())
	{
		// logging about fail
		char output[50];
		sprintf(output, "Failed to open file: %s", file_name);
		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
		// exit
		return false;
	}
	// if opened
	else
	{
		// logging success
		char output[50];
		sprintf(output, "Loading file: %s", file_name);
		WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
		int lines = 0;
		string body;
		bool opening_bracket = false;
		KEYWORD key = kwNo;
		PARSE_RECORD result;
		bool stopped = false;
		while(!f.eof())
		{
			string s;
			f >> s;
			if(s.empty())
				continue;
			if(s.find('#') != s.npos)
				continue;
			if(s.find('{') != s.npos)
			{
				opening_bracket = true;
				stopped = true;
			}
			if(s.find('}') != s.npos)
			{
				opening_bracket = false;
				stopped = true;
			}

			if(!stopped)
				body += s;
			else
	
				for(std::vector<KEYWORD_STORAGE_FIELD>::iterator it = key_storage.fields.begin();
					it != key_storage.fields.end();
					it++)
				{
					key = key_storage.ByName(s.substr(s.find((*it).name), strlen((*it).name.c_str())));
					if(key != kwNo)
						break;
				}

				switch(key)
				{
				case kwNo: 
					{
						FillParseRecordToDoList(&result, &body);
						break;
					}
				case kwGuiDef: return gamedef.gui.LoadFromFile(file_name);
				case kwRaceDef:	return gamedef.races.LoadFromFile(file_name);
				case kwStarDef: return gamedef.stars.LoadFromFile(file_name);
				case kwFor:
				case kwWhile:
					{
						if(!FillParseHeaderByKey(&result, key, &body))
							return false;
						else
							break;
					}
				default: break;
				}
				for(std::vector<string>::iterator it = result.what_to_do.begin();
					it != result.what_to_do.end();
					it++)
				{
					WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "FUNCTION NAME: " + GetFunctionName(&(*it)));
					WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "PARAM: " + GetNParam(&(*it), 1));
					if(GetFunctionName(&(*it)) == "Load")
					{
						string param = GetNParam(&body, 1);

						WriteToLog(DEFAULT_INTERPRET_LOG_NAME, "PARAM: " + param);

						Load((char*)param.c_str());
					}
					else
						Execute(&(*it));
				}
			
				if(stopped)
					body.clear();

		}
	}
}
















// LOADER V. 0.1
bool Load_v_01(char* file_name)
{
	// input stream
	ifstream f;
	// opening for input
	f.open(file_name, ios::in);

	// if not opened
	if(!f.is_open())
	{
		// logging about fail
		char output[50];
		sprintf(output, "Failed to open file: %s", file_name);
		LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
		// exit
		return false;
	}
	// if opened
	else
	{
		// logging success
		char output[50];
		sprintf(output, "Loading file: %s", file_name);
		LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);

		int lines = 0;
		// string, consists body operation of script/file
		// e.g. 'for(i=0;i<5;i++)' or 'Load(1.txt);Load(2.txt);Load(3.txt);'
		// i need this, cause 'f >> s' stops when get %20 (space) symbol, so
		// instead of 'for(i=0;i<5;i++)' we will get
		// for
		// (
		// i
		// =
		// 0
		// ..etc..
 		string body;
		// our result
		PARSE_RECORD result;
		// while not end of file
		while(!f.eof())
		{
			// reading string
			string s;
			f >> s;

			lines++;
			// if empty, skip iteration
			if(s.empty())
				continue;

			// concattenating 'body' of script/file
			if(s.find('{') == s.npos && s.find('}') == s.npos)
				body += s;
			else
			{
				// body ready
				//LogToFile("body: " + body);
				// now we need to recognize body
				// here we go

				// where we save current keyword/lexem/syntax block
				// default: statement, it case we won't find any cycles
				KEYWORD key = kwStatement;
				int key_pos = body.npos;
				int cnd_pos;
				int cnd_pos1;
				int cnd;

				// starting analyzing all 'body' string,
				// deleting already analyzed substrings
				int k;

					// first of all we find keyword
					for(k = 0; k < k_l; k++)
					{
						key_pos = body.find(keywords[k]);
						if(key_pos != body.npos)
						{
							// found key
							key = KEYWORD(k);
							// deleting substr with key from body
							string substr = body.substr(key_pos, strlen(keywords[k]));

							break;
						}
					}
					
					// if we find key
					if(key_pos != body.npos)
					{
						// writing it to result
						result.key = key;

						// what is the key?
						switch(key)
						{
						case kwFor:
						case kwWhile:
							{
									// in case of cycle we also need limits
									// e.g (for i = 0; i < 5; i++) limits are 0 and 5
									// 'min' is a number between '=' and ';'
									// 'max' is a number between colon of comparison and ';'

									// so, searching for min
									int eq_pos = body.find('=');
									// if '=' not found, error
									if(eq_pos == body.npos)
									{
										sprintf(output, "Syntax error (line %u): '=' expected", lines);
										LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
										// exiting
										return false;
									}
									// if '=' found
									else
									{
										// searching for first semicolon
										int sc_pos = body.find(';');
										// if ';' not found, error
										if(sc_pos == body.npos)
										{
											sprintf(output, "Syntax error (line %u): ';' expected", lines);
											LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
											// exiting
											return false;
										}
										// if ';' found
										else
										{
											// making 'min' substring
											string min = body.substr(eq_pos + 1, body.length() - eq_pos  - (body.length() - sc_pos) - 1);
											// writing result
											result.min = atoi(min.c_str());

											// okay, now we need 'max'
											// first, find condition
											cnd_pos = body.npos;
											cnd_pos1 = body.npos;
											
											cnd_pos = body.find('<');
											// if not found '<', searching '>'
											if(cnd_pos == body.npos)
											{
												cnd_pos = body.find('>');
												// if still not found, error
												if(cnd_pos == body.npos)
												{
													sprintf(output, "Syntax error (line %u): '>' or '<' expected", lines);
													LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
													// exiting
													return false;
												}
												// now check if there is +equal condition
												cnd_pos1 = body.find('=', cnd_pos);
												if(cnd_pos1 != body.npos)
												{
													//result.cnd = MOREEQUAL;
													cnd = 3;
													result.cnd = CONDITION(cnd);
												}
												else
												{
													cnd = 2;
													result.cnd = CONDITION(cnd);
												}
											}
											else
											{
												cnd_pos1 = body.find('=', cnd_pos);
												if(cnd_pos1 != body.npos)
												{
													cnd = 1;
													result.cnd = CONDITION(cnd);
												}
												else
												{
													cnd = 0;
													result.cnd = CONDITION(cnd);
												}
											}
										}
									
										// okay, now we know condition
										// search for max
										//    searching for second semicolon
										int sc_pos2 = body.find(';', cnd_pos);
										// if not found semicolon, error
										if(sc_pos2 == body.npos)
										{
											sprintf(output, "Syntax error (line %u): ';' expected", lines);
											LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
											// exiting
											return false;
										}										
										// if found semicolon
										else
										{
											int _dx;
											cnd_pos1 != body.npos ? _dx = 1 : _dx = 0;

											// making 'max' substring
											string max = body.substr(cnd_pos + _dx + 1, body.length() - (cnd_pos + _dx + 1)  - (body.length() - cnd_pos1) - 1);

											// writing result
											result.max = atoi(max.c_str());
										}
									}
									break;
								}
								case kwGuiDef:
									{
										gamedef.gui.LoadFromFile(file_name);
										return true;
									}
								case kwRaceDef:
									{
										gamedef.races.LoadFromFile(file_name);
										return true;
									}
								case kwStarDef:
									{
										gamedef.stars.LoadFromFile(file_name);
										return true;
									}
								default: break;
							}


							char buffer[100];
							sprintf(buffer, "PARSE RECORD::key = %u", k);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, buffer);
							sprintf(buffer, "PARSE RECORD::min = %u", result.min);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, buffer);
							sprintf(buffer, "PARSE RECORD::max = %u", result.max);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, buffer);
							sprintf(buffer, "PARSE RECORD::cnd = %u", result.cnd);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, buffer);

						}
						// if we not found key
						// this is a statement
						else
						{
							if(result.key == kwNo)
								result.key = kwStatement;

							// while body exists
							do
							{
								// searching for semicolon
								int sc_pos = body.find(';');

								if(sc_pos == body.npos)
								{
									sprintf(output, "Syntax error (line %u): ';' expected", lines);
									WriteToLog(DEFAULT_INTERPRET_LOG_NAME, output);
									break;
								}
								// making operation string
								string op = body.substr(0, sc_pos);
								// adding operation to what-to-do list
								result.what_to_do.push_back(op);

								// erasing this substring to take new one
								body.erase(0, sc_pos+1);
							}							
							while(body.length() > 0);

						}




								
				// zeroing both input string and body
				s.resize(0);
				body.resize(0);

			}
		}

		// is there a loop?
		if(result.key == kwFor)
		{
			for(int i = result.min; i < result.max; i++)
			{
				for(std::vector<string>::iterator it = result.what_to_do.begin();
					it!= result.what_to_do.end(); it++)
				{

					LogToFile(DEFAULT_INTERPRET_LOG_NAME, "PARSE RECORD::what-to-do = " + string((*it)));

					string s = (*it);
					// here will be command check
					if(s.find("Load") != s.npos)
					{
						// this is loading file
						// it maybe anything - gui description file, script, etc..
						int p1 = s.npos;
						int p2 = s.npos;

						p1 = s.find("(");
						p2 = s.find(")");

						if(p1 == s.npos || p2 == s.npos)
						{
							sprintf(output, "Syntax error in %s: '(' or ')' expected", s);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
						}
						else
						{
							string what_to_load;
							string final;
							// what to load, for example if string is
							// Load(1.txt); then what to load is
							// 1.txt
							what_to_load = s.substr(p1+1, p2-p1-1);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, "What-to-load = " + what_to_load);

							// there maybe Load(%i.txt) variant, so
							// we should search for '%'

							int pr = s.npos;

							pr = what_to_load.find('%');
							if(pr != what_to_load.npos)
							{
								final = what_to_load.substr(p1+2, pr-p1-1);

								char buf[100];
								sprintf(buf, "%u", i);
								final += string(buf);
								final += what_to_load.substr(pr+2, p2-pr);


								LogToFile(DEFAULT_INTERPRET_LOG_NAME, "Final = " + final);

								Load_v_01((char*)final.c_str());
							}
							else
							{
								Load_v_01((char*)what_to_load.c_str());
							}
						}
					}
				}
			}								
		}
		else
		{
				for(std::vector<string>::iterator it = result.what_to_do.begin();
					it!= result.what_to_do.end(); it++)
				{

					LogToFile(DEFAULT_INTERPRET_LOG_NAME, "PARSE RECORD::what-to-do = " + string((*it)));

					string s = (*it);
					// here will be command check
					if(s.find("Load") != s.npos)
					{
						// this is loading file
						// it maybe anything - gui description file, script, etc..
						int p1 = s.npos;
						int p2 = s.npos;

						p1 = s.find("(");
						p2 = s.find(")");

						if(p1 == s.npos || p2 == s.npos)
						{
							sprintf(output, "Syntax error in %s: '(' or ')' expected", s);
							LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
						}
						else
						{
							string what_to_load;
							string final;
							// what to load, for example if string is
							// Load(1.txt); then what to load is
							// 1.txt
							what_to_load = s.substr(p1+1, p2-p1-1);

							LogToFile(DEFAULT_INTERPRET_LOG_NAME, "What-to-load = " + what_to_load);

							// there maybe Load(%i.txt) variant, so
							// we should search for '%'

							int pr = s.npos;

							pr = what_to_load.find('%');
							if(pr != what_to_load.npos)
							{
								sprintf(output, "Syntax error in %s: '%' unexpected", what_to_load);
								LogToFile(DEFAULT_INTERPRET_LOG_NAME, output);
							}
							else
							{
								Load_v_01((char*)what_to_load.c_str());
							}
						}
					}
				}
		}
		return true;
	}
}

			

#include "stdafx.h"
#include "misc.h"
#include "parser.h"

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

void osl_DumpInputStream(char* stream)
{
	string s(stream);
	WriteToLog(DEFAULT_LOG_NAME, "Dumping input stream...");
	WriteToLog(DEFAULT_LOG_NAME, s);
	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}

void osl_DumpLexemStream(std::vector<LEXEM>* lexem_stream)
{
	WriteToLog(DEFAULT_LOG_NAME, "Dumping lexem stream...");

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

		WriteToLog(DEFAULT_LOG_NAME, type);

		if(!(it)->token.empty())
		{
			sprintf(type, "\tLexem token: %s", it->token.c_str());
			WriteToLog(DEFAULT_LOG_NAME, type);
		}

	}

	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}

bool osl_IsDivider(string s)
{
	const string dividers = "(){}[].,.;";
	if(s.find_first_of(dividers) == 0)
		return true;
	else
		return false;

	//for(int i = 0; i < a_dividers_l; i++)
	//	//if(strcmp(s.c_str(), a_dividers[i].c_str()) == 0)
	//	if(s.find_first_of(a_dividers[i]) == 0 == s.length() - 1)
	//		return true;
	//return false;
}

bool osl_IsOperator(string s)
{
	const string operators = "+-*\/%=<>";

	if(s.find_first_of(operators) == 0)
		return true;
	else
		return false;

	//for(int i = 0; i < a_operators_l; i++)
	//	/*if(strcmp(s.c_str(), a_operators[i].c_str()) == 0)*/
	//	if(s.find_first_of(a_operators[i]) == 0 == s.length() - 1)
	//		return true;
	//return false;
}

bool osl_IsFunction(string s)
{
	for(int i = 0; i < a_functions_l; i++)
		if(strcmp(s.c_str(), a_functions[i].c_str()) == 0)
			return true;
	return false;
}

bool osl_IsKeyword(string s)
{
	for(int i = 0; i < a_keywords_l; i++)
		if(strcmp(s.c_str(), a_keywords[i].c_str()) == 0)
			return true;
	return false;
}

bool osl_IsSysConstant(string s)
{
	for(int i = 0; i < a_sysconstants_l; i++)
		if(strcmp(s.c_str(), a_sysconstants[i].c_str()) == 0)
			return true;
	return false;
}

bool osl_IsStrConstant(string s)
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

bool osl_IsVariable(string s)
{
	const string deprecated = "+-*\/%=<>(){}[].,.;\'\"";

	if(s.find_first_of(deprecated) != s.npos)
		return false;
	//for(int i = 0; i < a_deprecated_l; i++)
	//	//if(strcmp(s.c_str(), a_deprecated[i].c_str()) == 0)
	//	if(s.find_first_of(a_deprecated[i]) != s.npos)
	//		return false;
	if(IsIntNumber(s))
		return false;
	if(IsFloatNumber(s))
		return false;
	if(osl_IsDivider(s))
		return false;
	if(osl_IsOperator(s))
		return false;
	return true;	
}

I_TOKEN_TYPE osl_GetToken(string s)
{
	if(osl_IsDivider(s))			return ttDivider;
	else if(osl_IsOperator(s))		return ttOperator;
	else if(osl_IsFunction(s))		return ttFunction;
	else if(osl_IsKeyword(s))		return ttKeyword;
	else if(osl_IsSysConstant(s))	return ttSysConstant;
	else if(IsIntNumber(s))			return ttIntConstant;
	else if(IsFloatNumber(s))		return ttFloatConstant;
	else if(osl_IsStrConstant(s))	return ttStrConstant;
	else if(osl_IsVariable(s))		return ttVariable;
	else							return ttUnknown;
}

inline void osl_Error(string err_code)
{
	WriteToLog(DEFAULT_LOG_NAME, err_code);
}

inline void osl_Warning(string warn_code)
{
	WriteToLog(DEFAULT_LOG_NAME, warn_code);
}

bool osl_IntVarDeclared(string var, vector<INT_VAR>* pool)
{
	for(std::vector<INT_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
		if((*it).name == var)
			return true;
	return false;
}

bool osl_FloatVarDeclared(string var, vector<FLOAT_VAR>* pool)
{
	for(std::vector<FLOAT_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
		if((*it).name == var)
			return true;
	return false;
}

bool osl_StrVarDeclared(string var, vector<STR_VAR>* pool)
{
	for(std::vector<STR_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
		if((*it).name == var)
			return true;
	return false;
}

void osl_DeclareInt(string var, vector<INT_VAR>* pool)
{
	INT_VAR v;
	v.name = var;
	v.init = false;

	WriteToLog(DEFAULT_LOG_NAME, "Declaring new INT variable: " + var);
	pool->push_back(v);
}

INT_VAR* osl_GetIntVar(string var, vector<INT_VAR>* pool)
{
	for(std::vector<INT_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == var)
			return &(*it);
	}
	osl_Error(P1001 + " (" + var + ")");
}

void osl_SetIntValue(int value, string var, vector<INT_VAR>* pool)
{
	INT_VAR* v = osl_GetIntVar(var, pool);

	char output[50];

	if(v)
	{
		sprintf(output, "%s = %u", var.c_str(), value);
		WriteToLog(DEFAULT_LOG_NAME, output);
		v->value = value;
		v->init = true;
	}
	else
	{
		sprintf(output, "Error, variable %s not found", var.c_str());
		WriteToLog(DEFAULT_LOG_NAME, output);
		return;
	}
}

void osl_DeclareFloat(string var, vector<FLOAT_VAR>* pool)
{
	FLOAT_VAR v;
	v.name = var;
	v.init = false;

	WriteToLog(DEFAULT_LOG_NAME, "Declaring new FLOAT variable: " + var);
	pool->push_back(v);
}

FLOAT_VAR* osl_GetFloatVar(string var, vector<FLOAT_VAR>* pool)
{
	for(std::vector<FLOAT_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == var)
			return &(*it);
	}
	osl_Error(P1001 + " (" + var + ")");
}

void osl_SetFloatValue(float value, string var, vector<FLOAT_VAR>* pool)
{
	FLOAT_VAR* v = osl_GetFloatVar(var, pool);
	char output[50];

	if(v)
	{
		sprintf(output, "%s = %u", var.c_str(), value);
		WriteToLog(DEFAULT_LOG_NAME, output);
		v->value = value;
		v->init = true;
	}
	else
	{
		sprintf(output, "Error, variable %s not found", var.c_str());
		WriteToLog(DEFAULT_LOG_NAME, output);
		return;
	}
}

void osl_DeclareStr(string var, vector<STR_VAR>* pool)
{
	STR_VAR v;
	v.name = var;
	v.init = false;

	WriteToLog(DEFAULT_LOG_NAME, "Declaring new STR variable: " + var);
	pool->push_back(v);
}

STR_VAR* osl_GetStrVar(string var, vector<STR_VAR>* pool)
{
	for(std::vector<STR_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == var)
			return &(*it);
	}
	osl_Error(P1001 + " (" + var + ")");
}

void osl_SetStrValue(string value, string var, vector<STR_VAR>* pool)
{
	STR_VAR* v = osl_GetStrVar(var, pool);
	char output[50];

	if(v)
	{
		sprintf(output, "%s = %u", var.c_str(), value);
		WriteToLog(DEFAULT_LOG_NAME, output);
		v->value = value;
		v->init = true;
	}
	else
	{
		sprintf(output, "Error, variable %s not found", var.c_str());
		WriteToLog(DEFAULT_LOG_NAME, output);
		return;
	}
}

int osl_GetInt(string operand_var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	int int_result = 0;

	if(IsIntNumber(operand_var))
	{
		int_result += atoi(operand_var.c_str());
	}
	else if(IsFloatNumber(operand_var))
	{
		int_result += (int)atof(operand_var.c_str());
	}
	else if(osl_IsVariable(operand_var))
	{
		VAR_TYPE this_var_type = osl_GetVarType(operand_var, int_pool, float_pool, str_pool);

		switch(this_var_type)
		{
		case vtInt:
			{
				INT_VAR* this_var = osl_GetIntVar(operand_var, int_pool);
				int_result = this_var->value;
				break;
			}
		case vtFloat:
			{
				FLOAT_VAR* this_var = osl_GetFloatVar(operand_var, float_pool);
				osl_Warning(W1003);
				int_result = (int)this_var->value;
				break;
			}
		case vtStr:
			{
				osl_Error(P1002);
				return 0;
			}
		case vtNone:
			{
				osl_Error(P1001);
				return 0;
			}
		default: break;
		}
	}

	return int_result;
}

float osl_GetFloat(string operand_var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	float float_result = 0.0;

	if(IsIntNumber(operand_var))
	{
		float_result += (float)atoi(operand_var.c_str());
	}
	else if(IsFloatNumber(operand_var))
	{
		float_result += atof(operand_var.c_str());
	}
	else if(osl_IsVariable(operand_var))
	{
		VAR_TYPE this_var_type = osl_GetVarType(operand_var, int_pool, float_pool, str_pool);

		switch(this_var_type)
		{
		case vtInt:
			{
				INT_VAR* this_var = osl_GetIntVar(operand_var, int_pool);
				osl_Warning(W1003);
				float_result = (float)this_var->value;
				break;
			}
		case vtFloat:
			{
				FLOAT_VAR* this_var = osl_GetFloatVar(operand_var, float_pool);
				float_result = this_var->value;
				break;
			}
		case vtStr:
			{
				osl_Error(P1002);
				return 0.0;
			}
		case vtNone:
			{
				osl_Error(P1001);
				return 0.0;
			}
		default: break;
		}
	}

	return float_result;
}

string osl_GetStr(string operand_var,  vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	string str_result;
	VAR_TYPE this_var_type = osl_GetVarType(operand_var, int_pool, float_pool, str_pool);

	switch(this_var_type)
	{
	case vtInt:
	case vtFloat:
		{
			osl_Error(P1002);
			return "";
		}
	case vtStr:
		{
			STR_VAR* this_var = osl_GetStrVar(operand_var, str_pool);
			str_result = this_var->value;
			break;
		}
	case vtNone:
		{
			osl_Error(P1001);
			return "";
		}
	default: break;
	}

	return str_result;
}

bool osl_DoAssign(string result_var, VAR_TYPE result_type, string operand_var,
				  vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	int int_result = 0;
	float float_result = 0.0;
	string str_result = "";

	switch(result_type)
	{
	case vtInt:
		{
			//int_result += osl_GetInt(operand_var, int_pool, float_pool, str_pool);
			osl_SetIntValue(osl_GetInt(operand_var, int_pool, float_pool, str_pool), result_var, int_pool);
			break;
		}
	case vtFloat:
		{
			//float_result += osl_GetFloat(operand_var, int_pool, float_pool, str_pool);
			osl_SetFloatValue(osl_GetFloat(operand_var, int_pool, float_pool, str_pool), result_var, float_pool);
			break;
		}
	case vtStr:
		{
			if(osl_IsVariable(operand_var))
				//str_result += osl_GetStr(operand_var, int_pool, float_pool, str_pool);
				osl_SetStrValue(osl_GetStr(operand_var, int_pool, float_pool, str_pool), result_var, str_pool);
			else
				osl_SetStrValue(operand_var, result_var, str_pool);
				//str_result += operand_var;
			break;
		}
	case vtNone:
		{
			osl_Error(P1008);
			break;
		}
	default: break;
	}
	return true;
}

bool osl_DoPlus(string result_var, VAR_TYPE result_type, string operand_var,
				vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	int int_result = 0;
	float float_result = 0.0;
	string str_result = "";

	switch(result_type)
	{
	case vtInt:
		{
			int_result += osl_GetInt(operand_var, int_pool, float_pool, str_pool);
			break;						
		}
	case vtFloat:
		{
			float_result += osl_GetFloat(operand_var, int_pool, float_pool, str_pool);
			break;
		}
	case vtStr:
		{
			str_result += osl_GetStr(operand_var, int_pool, float_pool, str_pool);
			break;	
		}
	default: break;
	}

	switch(result_type)
	{
	case vtInt:
		{
			osl_SetIntValue(osl_GetIntVar(result_var, int_pool)->value + int_result, result_var, int_pool);
			break;
		}
	case vtFloat:
		{
			osl_SetFloatValue(osl_GetFloatVar(result_var, float_pool)->value + float_result, result_var, float_pool);
			break;
		}
	case vtStr:
		{
			osl_SetStrValue(osl_GetStrVar(result_var, str_pool)->value + str_result, result_var, str_pool);
			break;
		}
	default: break;
	}

	return true;
}

bool osl_DoMultiply(string* result_v, string prev, string next)
{
	if(IsFloatNumber(prev))
	{
		float result;

		if(IsIntNumber(next))
		{
			char buffer[50];
			result = atof(prev.c_str()) * atoi(next.c_str());
			sprintf(buffer, "%d", result);
			*result_v = string(buffer);				
		}
		else if(IsFloatNumber(next))
		{
			char buffer[50];
			result = atof(prev.c_str()) * atof(next.c_str());
			sprintf(buffer, "%d", result);
			*result_v = string(buffer);
		}
	}
	else if(IsIntNumber(prev))
	{
		if(IsIntNumber(next))
		{
			char buffer[50];
			int result = atoi(prev.c_str()) * atoi(next.c_str());
			sprintf(buffer, "%u", result);
			*result_v = string(buffer);
		}
		else if(IsFloatNumber(next))
		{
			char buffer[50];
			float result = atoi(prev.c_str()) * atof(next.c_str());
			sprintf(buffer, "%u", result);
			*result_v = string(buffer);
		}
	}
	return true;
}

bool osl_DoDivide(string* result_v, string prev, string next)
{
	if(IsFloatNumber(prev))
	{
		float result;

		if(IsIntNumber(next))
		{
			char buffer[50];
			if(atoi(next.c_str()) == 0)
			{
				WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
				return false;
			}
			result = atof(prev.c_str()) / atoi(next.c_str());
			sprintf(buffer, "%d", result);
			*result_v = string(buffer);					
		}
		else if(IsFloatNumber(next))
		{
			char buffer[50];
			if(atoi(next.c_str()) == 0)
			{
				WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
				return false;
			}
			result = atof(prev.c_str()) / atof(next.c_str());
			sprintf(buffer, "%d", result);
			*result_v = string(buffer);
		}
	}
	else if(IsIntNumber(prev))
	{
		if(IsIntNumber(next))
		{
			char buffer[50];
			if(atoi(next.c_str()) == 0)
			{
				WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
				return false;
			}
			int result = atoi(prev.c_str()) / atoi(next.c_str());
			sprintf(buffer, "%u", result);
			*result_v = string(buffer);	
		}
		else if(IsFloatNumber(next))
		{
			char buffer[50];
			if(atoi(next.c_str()) == 0)
			{
				WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
				return false;
			}
			float result = atoi(prev.c_str()) / atof(next.c_str());
			sprintf(buffer, "%u", result);
			*result_v = string(buffer);	
		}
	}
	return true;
}

bool osl_VarInitted(string var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	VAR_TYPE vt = osl_GetVarType(var, int_pool, float_pool, str_pool);
	switch(vt)
	{
	case vtInt: if(osl_GetIntVar(var, int_pool)->init) return true; else return false;
	case vtFloat: if(osl_GetFloatVar(var, float_pool)->init) return true; else return false;
	case vtStr: if(osl_GetStrVar(var, str_pool)->init) return true; else return false;
	default: break;
	}
	return false;
}

void osl_TranslateVariables(vector<string>* source, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	for(std::vector<std::string>::iterator it = source->begin();
		it != source->end();
		it++)
	{
		if(osl_IsVariable(*it))
		{
			if(osl_VarInitted(*it, int_pool, float_pool, str_pool))
			{
				char buffer[50];
				VAR_TYPE vt = osl_GetVarType(*it, int_pool, float_pool, str_pool);

				switch(vt)
				{
				case vtInt:
					{
						sprintf(buffer, "%u", osl_GetIntVar(*it, int_pool)->value);
						*it = string(buffer);
						break;
					}
				case vtFloat:
					{
						sprintf(buffer, "%d", osl_GetFloatVar(*it, float_pool)->value);
						*it = string(buffer);
						break;
					}
				case vtStr:
					{
						*it = osl_GetStrVar(*it, str_pool)->value;
						break;
					}
				default: break;
				}
			}
		}
	}
}

void osl_SolveComplicated(string* result, vector<string>* source)
{

	//vector<string>::iterator last = source->end() - 1;
	//for(vector<string>::iterator it = source->begin();
	//	it != last;
	//	it++)
	//{
	vector<string>::iterator it = source->begin();
	int i = 0;
	while(i < source->size()-1)
	{
		i++;
		string s = *it;

		switch(s[0])
		{
		case '*':
			{
				osl_DoMultiply(result, *(it-1), *(it+1));
				break;
			}
		case '/':
			{
				osl_DoDivide(result, *(it-1), *(it+1));
				break;
			}
		default: break;
		}
		it++;
	}
}

void osl_SimplifyMath(vector<string>* source, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	vector<string> new_source;
	bool prev_op = false;

	vector<string> to_simplify;

	for(std::vector<string>::iterator it = source->begin();
		it != source->end();
		it++)
	{
		if(osl_IsVariable(*it))
		{
			new_source.push_back(*it);
			continue;
		}
		if(IsIntNumber(*it) || IsFloatNumber(*it))
		{
			if(!osl_IsOperator(*(it+1)))
				new_source.push_back(*it);
			continue;
		}
		if(osl_IsDivider(*it))
		{
			string result;
			if(prev_op)
				to_simplify.push_back(*(it-1));

			osl_SolveComplicated(&result, &to_simplify);
			to_simplify.resize(0);
			new_source.push_back(result);
			new_source.push_back(*it);

			continue;
		}
		if(osl_IsOperator(*it))
		{
			string op = *it;

			switch(op[0])
			{
			case '+':
			case '-':
				{
					if(prev_op)
						to_simplify.push_back(*(it-1));
					else
					{
						string result;
						osl_SolveComplicated(&result, &to_simplify);
						to_simplify.resize(0);
						new_source.push_back(result);
						new_source.push_back(*it);
					}

					
					prev_op = false;
					continue;
				}
			case '=':
				{
					new_source.push_back(*it);
					prev_op = false;
					continue;
				}
			case '*':
				{
					string r;

					osl_DoMultiply(&r, *(it-1), *(it+1));
					
					new_source.push_back(r);
					prev_op = true;

					to_simplify.push_back(*(it-1));
					to_simplify.push_back(*it);
							
					//if(osl_IsVariable(*(it+1)) || IsIntNumber(*(it+1)) || IsFloatNumber(*(it+1)))
					//	to_simplify.push_back(*(it+1));

					//to_simplify.push_back(*(it+1));

					*it = r;


					continue;
				}
			case '/':
				{
					string r;

					osl_DoDivide(&r, *(it-1), *(it+1));
					new_source.push_back(r);
					prev_op = true;

					to_simplify.push_back(*(it-1));
					to_simplify.push_back(*it);
					//to_simplify.push_back(*(it+1));

					continue;
				}
			default: continue;
			}
		}
	}
	WriteToLog(DEFAULT_LOG_NAME, "To simplify: ");
	DumpExecuteList(&to_simplify);
	source->assign(new_source.begin(), new_source.end());
}



	//int i = -1;
	//vector<string> new_source;
	//for(std::vector<std::string>::iterator it = source->begin();
	//	it != source->end();
	//	it++)
	//{
	//	i++;
	//	if(osl_IsVariable(*it))
	//		new_source.push_back(*it);
	//	if(osl_IsOperator(*it))
	//	{
	//		if(&*(it + 1) == NULL)
	//		{
	//			osl_Error(P1009);
	//			return;
	//		}
	//		switch((*it)[0])
	//		{
	//		case '=':
	//			{
	//				new_source.push_back(*it);
	//				break;
	//			}
	//		case '*':
	//			{
	//				string prev = *(it-1);
	//				string next = *(it+1);

	//				if(IsFloatNumber(prev))
	//				{
	//					float result;

	//					if(IsIntNumber(next))
	//					{
	//						char buffer[50];
	//						result = atof(prev.c_str()) * atoi(next.c_str());
	//						sprintf(buffer, "%d", result);
	//						new_source.push_back(string(buffer));				
	//					}
	//					else if(IsFloatNumber(next))
	//					{
	//						char buffer[50];
	//						result = atof(prev.c_str()) * atof(next.c_str());
	//						sprintf(buffer, "%d", result);
	//						new_source.push_back(string(buffer));
	//					}
	//				}
	//				else if(IsIntNumber(prev))
	//				{
	//					if(IsIntNumber(next))
	//					{
	//						char buffer[50];
	//						int result = atoi(prev.c_str()) * atoi(next.c_str());
	//						sprintf(buffer, "%u", result);
	//						new_source.push_back(string(buffer));
	//					}
	//					else if(IsFloatNumber(next))
	//					{
	//						char buffer[50];
	//						float result = atoi(prev.c_str()) * atof(next.c_str());
	//						sprintf(buffer, "%u", result);
	//						new_source.push_back(string(buffer));
	//					}
	//				}
	//				break;
	//			}

	//		case '/':
	//			{
	//				string prev = *(it-1);
	//				string next = *(it+1);

	//				if(IsFloatNumber(prev))
	//				{
	//					float result;

	//					if(IsIntNumber(next))
	//					{
	//						char buffer[50];
	//						if(atoi(next.c_str()) == 0)
	//						{
	//							WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
	//							return;
	//						}
	//						result = atof(prev.c_str()) / atoi(next.c_str());
	//						sprintf(buffer, "%d", result);
	//						new_source.push_back(string(buffer));					
	//					}
	//					else if(IsFloatNumber(next))
	//					{
	//						char buffer[50];
	//						if(atoi(next.c_str()) == 0)
	//						{
	//							WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
	//							return;
	//						}
	//						result = atof(prev.c_str()) / atof(next.c_str());
	//						sprintf(buffer, "%d", result);
	//						new_source.push_back(string(buffer));
	//					}
	//				}
	//				else if(IsIntNumber(prev))
	//				{
	//					if(IsIntNumber(next))
	//					{
	//						char buffer[50];
	//						if(atoi(next.c_str()) == 0)
	//						{
	//							WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
	//							return;
	//						}
	//						int result = atoi(prev.c_str()) / atoi(next.c_str());
	//						sprintf(buffer, "%u", result);
	//						new_source.push_back(string(buffer));	
	//					}
	//					else if(IsFloatNumber(next))
	//					{
	//						char buffer[50];
	//						if(atoi(next.c_str()) == 0)
	//						{
	//							WriteToLog(DEFAULT_LOG_NAME, "Division by zero");
	//							return;
	//						}
	//						float result = atoi(prev.c_str()) / atof(next.c_str());
	//						sprintf(buffer, "%u", result);
	//						new_source.push_back(string(buffer));	
	//					}
	//				}
	//				break;
	//			}
	//		default: 
	//			{
	//				new_source.push_back(*(it));
	//				new_source.push_back(*(it+1));
	//				break;
	//			}
	//		}
	//	}
	//	//else
	//	//{
	//	//	if(osl_IsOperator

	//	//		if(osl_IsOperator(*(it+1)))
	//	//		{
	//	//			string check = *(it+1);
	//	//			if(check[0] != '*' && check[0] != '/')
	//	//			{
	//	//				new_source.push_back(*it);
	//	//			}
	//	//		}
	//	//	}
	//	//	else
	//	//		new_source.push_back(*it);

	//	//}
	//}
	//source->assign(new_source.begin(), new_source.end());
//}

bool osl_ExecuteMath(vector<string>* source, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{

	// example: c = 2 + 6 * 4 - 2 + a - b;
	
	//osl_TranslateVariables(source, int_pool, float_pool, str_pool);
	//
	//vector<string>::iterator it = source->begin();
	//while(it != source->end())
	//{
	//	string s = *it;


	//	if(osl_IsVariable(s)
	//	{
	//		if(osl_VarInitted(s, int_pool, float_pool, str_pool))
	//		{
	//			osl_TranslateVariables(
	//			VAR_TYPE vt = osl_GetVarType(s, int_pool, float_pool, str_pool);

	//			switch(vt)
	//			{
	//			case vtInt:
	//				{
	//					new_source.push_back(


//	//string result_var;
//	VAR_TYPE result_var_type;
//	string temp;
//
//	VAR_TYPE current_token_type;
//
//
//	std::vector<string>::iterator first;
//	std::vector<string>::iterator last = source->end();
//	last--;
//
//	const string result_var = *(first = source->begin());
//	result_var_type = osl_GetVarType(result_var, int_pool, float_pool, str_pool);
//
//	//int int_result = 0;
//	//float float_result = -1.0;
//	//string str_result = "";
//
//	if(result_var_type == vtNone)
//	{
//		osl_Error(P1001);
//		return false;
//	}
//
//	if(*(first + 1) != "=")
//	{
//		osl_Error(P1003);
//		return false;
//	}
//
//	if(*(last) != ";")
//	{
//		osl_Error(P1004);
//		return false;
//	}
//	
//
//	int i = -1;
//	std::vector<string>::iterator prev;
//	std::vector<string>::iterator next;
//	std::vector<string>::iterator it = source->begin();
//	
//	while(it != last)
//	{
//		it++;
//		prev = it-1;
//
//		if(*it == "nop")
//			continue;
//
//
//		if(osl_IsOperator(*it))
//		{
//			switch((*it)[0])
//			{
//			case '=':
//				{
//					if(!osl_DoAssign(result_var, result_var_type, (*(it+1)), int_pool, float_pool, str_pool))
//					{
//						WriteToLog(DEFAULT_LOG_NAME, "Failed to do assign");
//						return false;
//					}
//					break;
//				
//				}
//
//			case '+':
//				{
//					if((*prev).empty())
//					{
//						osl_Error(P1005);
//						return false;
//					}				
//					if(!osl_DoPlus(result_var, result_var_type, *(it+1), int_pool, float_pool, str_pool))
//					{
//						WriteToLog(DEFAULT_LOG_NAME, "Failed to do plus");
//						return false;
//					}
//					break;
//				}
//
//			case '-':
//				{
//					if((*prev).empty())
//					{
//						osl_Error(P1005);
//						return false;
//					}				
//					if(!osl_DoPlus(result_var, result_var_type, *(it+1), int_pool, float_pool, str_pool))
//					{
//						WriteToLog(DEFAULT_LOG_NAME, "Failed to do plus");
//						return false;
//					}
//					break;
//				}
//
//			default: break;
//			}
//		}
//	}
//
//	return true;
//}

void DumpExecuteList(vector<string>* list)
{
	WriteToLog(DEFAULT_LOG_NAME, "Dumping execute list...");

	for(std::vector<std::string>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		WriteToLog(DEFAULT_LOG_NAME, "\t" + (*it));
	}

	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}

VAR_TYPE osl_GetVarType(string var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool)
{
	bool int_var = osl_IntVarDeclared(var, int_pool);
	bool float_var = osl_FloatVarDeclared(var, float_pool);
	bool str_var = osl_StrVarDeclared(var, str_pool);

	if(int_var)
		return vtInt;
	else if(float_var)
		return vtFloat;
	else if(str_var)
		return(vtStr);
	else
		return vtNone;
}

int osl_Lexemize(char* char_stream, std::vector<LEXEM>* lexem_stream)
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
			lexem.type = osl_GetToken(body);
			lexem.token = body;
			lexem_stream->push_back(lexem);
		}
	}
	return lexem_count;
}

bool osl_MakeList(std::vector<LEXEM>* lexem_stream, std::vector<string>* list)
{
	std::vector<INT_VAR> int_pool;
	std::vector<FLOAT_VAR> float_pool;
	std::vector<STR_VAR> str_pool;
	std::vector<string> to_execute;

	LEXEM_SWITCHER sw;
	string current_var = "";
	string left_var = "";
	VAR_TYPE left_var_type = vtNone;
	bool once_assigned = false;
	string current_op = "";
	VAR_TYPE current_var_type = vtNone;
	string current_constant = "";
	VAR_TYPE current_constant_type = vtNone;

	LEXEM* lexem = NULL;
	LEXEM* prev_lexem = NULL;


	std::vector<LEXEM>::iterator it = lexem_stream->begin();

	while(it != lexem_stream->end())
	{
		
		lexem = &(*it);

		switch(lexem->type)
		{
		case ttKeyword:
			{
				string key = lexem->token;

				if(key == "int")
				{
					if(sw.int_switch || sw.float_switch || sw.str_switch)
					{
						osl_Error(S1002);
						return false;
					}
					else
						sw.int_switch = true;
				}

				if(key == "float")
				{
					if(sw.int_switch || sw.float_switch || sw.str_switch)
					{
						osl_Error(S1002);
						return false;
					}
					else
						sw.float_switch = true;
				}

				if(key == "string")
				{
					if(sw.int_switch || sw.float_switch || sw.str_switch)
					{
						osl_Error(S1002);
						return false;
					}
					else
						sw.str_switch = true;
				}
				break;
			}

		case ttDivider:
			{
				

				switch(lexem->token[0])
				{
				case ';':
					{
						to_execute.push_back(lexem->token);
						osl_TranslateVariables(&to_execute, &int_pool, &float_pool, &str_pool);
						WriteToLog(DEFAULT_LOG_NAME, "Before simplification:");
						DumpExecuteList(&to_execute);
						osl_SimplifyMath(&to_execute, &int_pool, &float_pool, &str_pool);
						WriteToLog(DEFAULT_LOG_NAME, "After simplification:");
						DumpExecuteList(&to_execute);
						WriteToLog(DEFAULT_LOG_NAME, "Trying to execute");
						if(osl_ExecuteMath(&to_execute, &int_pool, &float_pool, &str_pool))
						{
							WriteToLog(DEFAULT_LOG_NAME, "OK");

							to_execute.resize(0);
						}
						else
						{
							WriteToLog(DEFAULT_LOG_NAME, "FAILED");
							return false;
						}

						break;
					}
				default: break;
				}
				break;
			}
		case ttStrConstant:
			{
				current_constant = lexem->token;
				current_constant_type = vtStr;

				to_execute.push_back(lexem->token);

				break;
			}


		case ttFloatConstant:
			{
				current_constant = lexem->token;
				current_constant_type = vtFloat;

				to_execute.push_back(lexem->token);

				break;
			}
					

		case ttIntConstant:
			{
				current_constant = lexem->token;
				current_constant_type = vtInt;

				to_execute.push_back(lexem->token);

				break;
			}


		case ttOperator:
			{
				string op = lexem->token;

				to_execute.push_back(lexem->token);

				break;
			}

		case ttVariable:
			{
				string var = lexem->token;
				current_var = var;

				to_execute.push_back(lexem->token);
				

				if(sw.int_switch)
				{
					sw.int_switch = false;
					sw.float_switch = false;
					sw.str_switch = false;
					if(!osl_FloatVarDeclared(var, &float_pool) &&
						!osl_StrVarDeclared(var, &str_pool) &&
						!osl_IntVarDeclared(var, &int_pool))
						osl_DeclareInt(var, &int_pool);
					else
						osl_Warning(W1001 + " (" + var + ")");						
				}
				else if(sw.float_switch)
				{
					sw.int_switch = false;
					sw.float_switch = false;
					sw.str_switch = false;
					if(!osl_IntVarDeclared(var, &int_pool) &&
						!osl_StrVarDeclared(var, &str_pool) &&
						!osl_FloatVarDeclared(var, &float_pool))
						osl_DeclareFloat(var, &float_pool);
					else
						osl_Warning(W1001 + " (" + var + ")");
				}
				else if(sw.str_switch)
				{
					sw.int_switch = false;
					sw.float_switch = false;
					sw.str_switch = false;
					if(!osl_IntVarDeclared(var, &int_pool) &&
						!osl_FloatVarDeclared(var, &float_pool) &&
						!osl_StrVarDeclared(var, &str_pool))
						osl_DeclareStr(var, &str_pool);
					else
						osl_Warning(W1001 + " (" + var + ")");
				}

				current_var_type = osl_GetVarType(var, &int_pool, &float_pool, &str_pool);

				break;
			}
		default: break;
		}
		it++;
	}
	return true;
}
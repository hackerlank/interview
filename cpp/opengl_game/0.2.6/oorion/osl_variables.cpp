#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_variables.h"


void osl_DeclareVar(OSL_VAR var, vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Declaring variable: " + var.name);
	pool->push_back(var);
}

void osl_UndeclareVar(string name, vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Undeclaring variable: " + name);
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
		{
			pool->erase(it);
			break;
		}
	}
}

bool osl_VarDeclared(string name, vector<OSL_VAR>* pool)
{
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
			return true;
	}
	return false;
}

OSL_VAR* osl_GetVar(string name, vector<OSL_VAR>* pool)
{
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
			return &*it;
	}
	return NULL;
}

bool osl_SetVar(string name, VAR_DATA data, vector<OSL_VAR>* pool)
{
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
		{
			it->data = data;
			it->init = true;
			return true;
		}
	}
	return false;
}

bool osl_VarInitted(string name, vector<OSL_VAR>* pool)
{
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
		{
			if(it->init)
				return true;
			else
				return false;
		}
	}
	return false;
}

VAR_DATA osl_GetVarData(string name, vector<OSL_VAR>* pool)
{
	VAR_DATA data;
	
	data.int_value = 0;

	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
		{
			if(it->init)
				return it->data;
			else
				return data;
		}
	}
	return data;
}

void osl_SetVarData(VAR_DATA* data, OSL_VAR_TYPE type, string what, bool add)
{
	switch(type)
	{
	case vtInt:
		{
			int d;
			if(osl_IsIntConstant(what))
			{
				d = osl_StrToInt(what);
			}
			else if(osl_IsFloatConstant(what))
			{
				d = (int)osl_StrToFloat(what);
			}
			if(add)
				data->int_value = data->int_value + d;
			else
				data->int_value = d;
			break;
		}
	case vtFloat:
		{
			if(add)
				data->float_value = data->float_value + osl_StrToFloat(what);
			else
				data->float_value = osl_StrToFloat(what);
			break;
		}
	case vtStr:
		{
			if(add)
			{
				if(data->str_value)
				{
					char* new_str = new char[strlen(data->str_value) + what.length() + 1];			
					sprintf(new_str, "%s%s\0", data->str_value, what.data());
					delete[] data->str_value;
					data->str_value = new_str;
				}
				else
					osl_Error("osl_SetVarData(): Unknown error");
			}
			else
			{
				if(data->str_value)
					delete[] data->str_value;
				
				char* new_str = new char[what.length()+1];
				sprintf(new_str, "%s\0", what.data());
				data->str_value = new_str;
			}
			break;
		}
	default: break;
	}
}

OSL_VAR_TYPE osl_GetVarType(string name, vector<OSL_VAR>* pool)
{
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
		{
			if(it->init)
				return it->type;
			else
				return vtNone;
		}
	}
	return vtNone;
}

bool osl_CompareConstants(OSL_VAR_TYPE t, E_TOKEN_TYPE c)
{
	if(t == vtInt && c == ttIntConstant)
		return true;
	if(t == vtFloat && c == ttFloatConstant)
		return true;
	if(t == vtStr && c == ttStrConstant)
		return true;
	return false;
}

string osl_VarTypeToString(OSL_VAR_TYPE type)
{
	if(type == vtInt)
		return "int";
	else if(type == vtFloat)
		return "float";
	else if(type == vtStr)
		return "string";
	else
		return "";
}


string osl_VarToString(OSL_VAR* var)
{
	if(var->type == vtInt)
	{
		return osl_IntToStr(var->data.int_value);
	}
	else if(var->type == vtFloat)
	{
		return osl_FloatToStr(var->data.float_value);
	}
	else if(var->type = vtStr)
	{
		return var->data.str_value;
	}
	else
		return "";
}

OSL_VAR_TYPE osl_StringToVarType(string s)
{
	if(s.find_first_of(S_OSL_VAR_TYPE) == s.npos)
		return vtNone;
	else
	{
		if(s == "int")
			return vtInt;
		else if(s == "float")
			return vtFloat;
		else if(s == "string")
			return vtStr;
	}
	return vtNone;
}























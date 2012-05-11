#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_variables.h"


void osl_DeclareVar(OSL_VAR var, vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_LOG_NAME, "Declaring variable: " + var.name);
	pool->push_back(var);
}

void osl_UndeclareVar(string name, vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_LOG_NAME, "Undeclaring variable: " + name);
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
			it->data.empty = false;
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
	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
		{
			if(it->init)
				return it->data;
			else
				return EMPTY_VAR_DATA;
		}
	}
	return EMPTY_VAR_DATA;
}

void osl_SetVarData(VAR_DATA* data, OSL_VAR_TYPE type, string what, bool add)
{
	switch(type)
	{
	case vtInt:
		{
			if(add)
				data->int_value = data->int_value + atoi(what.c_str());
			else
				data->int_value = atoi(what.c_str());
			break;
		}
	case vtFloat:
		{
			if(add)
				data->float_value = data->float_value + atof(what.c_str());
			else
				data->float_value = atof(what.c_str());
			break;
		}
	case vtStr:
		{
			if(add)
				data->str_value = data->str_value + what;
			else
				data->str_value = what;
			break;
		}
	default: break;
	}
	data->empty = what.empty();
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
}

string osl_VarToString(OSL_VAR* var)
{
	if(var->type == vtInt)
	{
		char buffer[16];
		itoa(var->data.int_value, buffer, 10);
		return string(buffer);
	}
	else if(var->type == vtFloat)
	{
		char buffer[16];
		fcvt(var->data.float_value, C_ACC, NULL, NULL);
		return string(buffer);
	}
	else if(var->type = vtStr)
	{
		return var->data.str_value;
	}
	else
		return "";
}




void osl_DumpVarPool(vector<OSL_VAR>* pool)
{
	WriteToLog(DEFAULT_LOG_NAME, "Dumping variable pool...");

	for(vector<OSL_VAR>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		OSL_VAR var = *it;

		WriteToLog(DEFAULT_LOG_NAME, "\t name: \t" + var.name);
		WriteToLog(DEFAULT_LOG_NAME, "\t type: \t" + osl_VarTypeToString(var.type));
		WriteToLog(DEFAULT_LOG_NAME, "\t value: \t" + osl_VarToString(&var));
	}

	WriteToLog(DEFAULT_LOG_NAME, "Finished dumping");
}
		


















#include "stdafx.h"
#include "../misc.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_functions.h"



void osl_DeclareFunction(OSL_FUNCTION func, vector<OSL_FUNCTION>* pool)
{
	pool->push_back(func);
}

void osl_UndeclareFunction(string name, vector<OSL_FUNCTION>* pool)
{
	for(vector<OSL_FUNCTION>::iterator it = pool->begin();
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

bool osl_FunctionDeclared(string name, vector<OSL_FUNCTION>* pool)
{
	for(vector<OSL_FUNCTION>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
			return true;
		
	}
	return false;
}

OSL_FUNCTION* osl_GetFunction(string name, vector<OSL_FUNCTION>* pool)
{
	for(vector<OSL_FUNCTION>::iterator it = pool->begin();
		it != pool->end();
		it++)
	{
		if(it->name == name)
			return &*it;
		
	}
	return NULL;
}
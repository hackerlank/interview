#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_variables.h"
#include "osl_lexems.h"
#include "osl_templates.h"
#include "osl_maths.h"
#include "osl_parse.h"

void TestTemplate(vector<LEXEM>* list, OSL_TEMPLATE_STORAGE* storage)
{
	E_OSL_TEMPLATE t = storage->MatchLexems(list);
	
	switch(t)
	{
	case E_OSL_ASSIGN_VAR_TEMPLATE:
		{
			MessageBox(0, "E_OSL_ASSIGN_VAR_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_DECLARE_VAR_TEMPLATE:
		{
			MessageBox(0, "E_OSL_DECLARE_VAR_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE:
		{
			MessageBox(0, "E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_DECLARE_FUNCTION_TEMPLATE:
		{
			MessageBox(0, "E_OSL_DECLARE_FUNCTION_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_FUNCTION_CALL_TEMPLATE:
		{
			MessageBox(0, "E_OSL_FUNCTION_CALL_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_IF_TEMPLATE:
		{
			MessageBox(0, "E_OSL_IF_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_IF_ELSE_TEMPLATE:
		{
			MessageBox(0, "E_OSL_IF_ELSE_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_WHILE_TEMPLATE:
		{
			MessageBox(0, "E_OSL_WHILE_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_DO_TEMPLATE:
		{
			MessageBox(0, "E_OSL_DO_TEMPLATE", 0, 0);
			break;
		}
	case E_OSL_NO_TEMPLATE:
		{
			MessageBox(0, "E_OSL_NO_TEMPLATE", 0, 0);
			break;
		}
	default: break;
	}
}

				
				//E_OSL_DECLARE_VAR_TEMPLATE,
				//E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE,
				//E_OSL_DECLARE_FUNCTION_TEMPLATE,
				//E_OSL_FUNCTION_CALL_TEMPLATE,
				//E_OSL_IF_TEMPLATE,
				//E_OSL_IF_ELSE_TEMPLATE,
				//E_OSL_WHILE_TEMPLATE,
				//E_OSL_DO_TEMPLATE,
				//E_OSL_NO_TEMPLATE

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


bool osl_CompileSeq(vector<LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{
	osl_DumpLexemStream(list);
	E_OSL_TEMPLATE t = storage->MatchLexems(list);

	if(t == E_OSL_NO_TEMPLATE)
		return false;
	else
	{
		switch(t)
		{
		case E_OSL_DECLARE_VAR_TEMPLATE:
			{
				// %type(%name);
				vector<LEXEM>::iterator i_first = list->begin();
				LEXEM first = *i_first;

				vector<LEXEM>::iterator i_second = list->begin() + 2;
				LEXEM second = *i_second;

				OSL_VAR_TYPE type = osl_StringToVarType(first.token);
				string name = second.token;

				if(osl_VarDeclared(name, pool))
				{
					// TODO: warning here?
					osl_Error("Variable redeclaration", name);
					return false;
				}
				
				OSL_VAR var;		
				var.name = name;
				var.type = type;

				osl_DeclareVar(var, pool);

				break;
			}
		case E_OSL_ASSIGN_VAR_TEMPLATE:
			{
				// %var=%math;

				vector<LEXEM> math;
				vector<LEXEM>::iterator i_first = list->begin();
				LEXEM first = *i_first;

				OSL_VAR* var;

				if(osl_VarDeclared(first.token, pool))
				{
					 var = osl_GetVar(first.token, pool);
				}
				else
				{
					osl_Error("Variable undeclared", first.token);
					return false;
				}

				
				for(vector<LEXEM>::iterator it = list->begin()+2;
					it!= list->end()-1;
					it++)
				{
					math.push_back(*it);
				}

				if(!osl_EvaluateMath(&math, var, pool))
					osl_Error("Compile error", "E_OSL_ASSIGN_VAR_TEMPLATE");;
				break;
			}
		case E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE:
			{
				// %type(%name)=%math;
				vector<LEXEM>::iterator i_first = list->begin();
				LEXEM first = *i_first;

				vector<LEXEM>::iterator i_second = list->begin() + 2;
				LEXEM second = *i_second;

				OSL_VAR_TYPE type = osl_StringToVarType(first.token);
				string name = second.token;

				if(osl_VarDeclared(name, pool))
				{
					// TODO: warning here?
					osl_Error("Variable redeclaration", name);
					return false;
				}
				
				OSL_VAR var;		
				var.name = name;
				var.type = type;

				osl_DeclareVar(var, pool);

				vector<LEXEM> math;
				OSL_VAR* d_var = osl_GetVar(name, pool);

				for(vector<LEXEM>::iterator it = list->begin()+5;
					it!= list->end()-1;
					it++)
				{
					math.push_back(*it);
				}

				if(!osl_EvaluateMath(&math, d_var, pool))
					osl_Error("Compile error", "E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE");

				break;
			}

		default: break;
		}
	}
	return true;
}



bool osl_Compile(vector<LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{
	vector<LEXEM> lxm_seq;
	bool braced = false;
	int brace_level = 0;

	for(vector<LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		lxm_seq.push_back(*it);

		LEXEM l = *it;

		if(osl_IsLeftBrace(l.token))
		{
			if(braced)
				brace_level++;
			braced = true;
		}

		if(osl_IsSemicolon(l.token))
		{
			if(!braced)
			{
				// here compile lxm_seq
				if(!osl_CompileSeq(&lxm_seq, pool, storage))
					return false;
				
				// zeroize lxm_seq
				lxm_seq.resize(0);
			}
		}

		if(osl_IsRightBrace(l.token))
		{
			if(brace_level == 0)
			{
				braced = false;

				// here compile lxm_seq
				if(!osl_CompileSeq(&lxm_seq, pool, storage))
					return false;
				// zeroize lxm_seq
				lxm_seq.resize(0);

			}
			else
				brace_level--;
		}
	}


	return true;
}


bool osl_Interpret(vector<LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{

	return true;
}


#include "stdafx.h"
#include "../misc.h"
#include "osl_types.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_maths.h"
#include "osl_parse.h"
#include "osl_variables.h"
#include "osl_debug.h"
#include "osl_execute.h"
#include "../config.h"

// ��� �����
// --------------------
#include <stack>
// --------------------

extern vector<OSL_CONSTANT> sys_constants;
extern config cfg;
extern unsigned int OSL_CURRENT_LINE;




// �������� ������
// ����:
//	list	- ����� ������
// �����:
//	true/false - ������� �� ��������� �������
// ����������:
//	

bool osl_CheckBrackets(vector<OSL_LEXEM>* list)
{
	int bracket_level = 0;
	bool in_bracket = false;

	int brace_level = 0;
	bool in_brace = false;

	for(vector<OSL_LEXEM>::iterator it = list->begin(); it != list->end(); it++)
	{
		OSL_LEXEM lx = *it;

		if(osl_IsLeftBracket(lx.token))
		{
			if(!in_bracket)
				in_bracket = true;
			
			bracket_level++;
		}
		else if(osl_IsRightBracket(lx.token))
		{
			if(!in_bracket)
			{
				osl_Error("Unexpected closing bracket");
				return false;
			}
			else
			{
				bracket_level--;
				if(bracket_level == 0)
					in_bracket = false;
			}
		}
		else if(osl_IsLeftBrace(lx.token))
		{
			if(!in_brace)
				in_brace = true;

			brace_level++;
		}
		else if(osl_IsRightBrace(lx.token))
		{
			if(!in_brace)
			{
				osl_Error("Unexpected closing brace");
				return false;
			}
			else
			{
				brace_level--;
				if(brace_level == 0)
					in_brace = false;
			}
		}
	}

	if(in_brace || in_bracket || brace_level > 0 || bracket_level > 0)
	{
		osl_Error("One or more brackets/braces unclosed");
		return false;
	}
	return true;
}


// ���������� �������
// ����:
//	list	- ����� ������, ������� ���������� ��������������
//	pool	- ������ ����������
//	storage - ������ �������� ����������� ����� OSL
// �����:
//	true/false - ������� �� ��������� ����������
// ����������:
//	����� ����� ���������� ���������� �� �� ��������
//	

bool osl_Compile(vector<OSL_LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{
	if(!osl_CheckBrackets(list))
		return false;

	UINT constant_count = 0;
	UINT compiled_vars_count = 0;
	UINT system_funcs_count = 0;

	BOOL IN_FUNC = FALSE;

	UINT i = 0;
	while(i < list->size())
	{

		OSL_LEXEM* lx = &list->at(i);
		if(lx->type == ttSysConstant)
		{		
			for(vector<OSL_CONSTANT>::iterator it = sys_constants.begin();
				it != sys_constants.end();
				it++)
			{
				OSL_CONSTANT c = *it;
				if(c.name == lx->token)
				{
					constant_count++;

					if(c.data == INTERNAL_CONSTANT)
					{
						string name = c.name;
						if(name == "sys_scr_width")
							lx->token = osl_IntToStr(cfg.scr_width);
						else if(name == "sys_scr_height")
							lx->token = osl_IntToStr(cfg.scr_height);
					}
					else
						lx->token = osl_IntToStr(c.data);


					lx->type = ttIntConstant;

					break;
				}
			}
		}

		if(lx->type == ttValidName)
		{
			if(osl_VarDeclared(lx->token, pool))
			{
				compiled_vars_count++;

				OSL_VAR* var = osl_GetVar(lx->token, pool);
				E_OSL_VAR_TYPE type = osl_GetVarType(lx->token, pool);
				string value = osl_VarToString(var);
				
				switch(type)
				{
				case vtInt:		lx->type = ttIntConstant; break;
				case vtFloat:	lx->type = ttFloatConstant; break;
				case vtStr:		lx->type = ttStrConstant; break;
				default:		lx->type = ttUnknown; break;
				}

				lx->token = value;
			}
		}
		else if(lx->type == ttSysFunction) // TODO: user-functions too
		{
			OSL_FUNCTION* func = osl_GetSysFunction(lx->token);

			/* ���� ������� ���������� */
			if(func)
			{

				/* ���� ��� ���������� �������� */
				if(func->return_type != vtNone)
				{
					system_funcs_count++;

					vector<OSL_LEXEM> full_list;
					vector<OSL_LEXEM> param_list;
					full_list.push_back(*lx);

					for(vector<OSL_LEXEM>::iterator it2 = list->begin() +1 +i;
						it2 != list->end();
						it2++)
					{
						OSL_LEXEM lx2 = *it2;

						UINT br_level = 0;
						bool f = false;
						if(osl_IsLeftBracket(lx2.token) ||
							osl_IsConstant(lx2.token) ||
							osl_IsValidName(lx2.token) ||
							osl_IsComma(lx2.token) ||
							osl_IsRightBracket(lx2.token))
						{
							if(br_level == 0 && f)
								break;

							if(osl_IsLeftBracket(lx2.token))
							{
								br_level++;
								IN_FUNC = TRUE;
							}
							else if(osl_IsRightBracket(lx2.token))
							{
								br_level--;
								if(IN_FUNC)
									IN_FUNC = FALSE;
							}

							full_list.push_back(lx2);
							param_list.push_back(lx2);
							f = true;
						}
					}
					bool ok = osl_Compile(&param_list, pool, storage);
					if(ok)
					{
						if(IN_FUNC)
						{
							// ����� ������� ������ ������� �� ���������
							// !!!!!!!!!
							// !!!!!!!!
						}
					}
				}
			}
		}
		i++;
	}
#ifdef _DEBUG_OSL
	if(constant_count > 0)
	{
		char buffer[64];
		sprintf(buffer, "%u constants recognized", constant_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
	}

	if(compiled_vars_count > 0)
	{
		char buffer[64];
		sprintf(buffer, "%u variables compiled", compiled_vars_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
	}

	if(system_funcs_count > 0)
	{
		char buffer[64];
		sprintf(buffer, "%u functions compiled", system_funcs_count);
		WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);
	}
#endif

	return true;
}



// �������� ������ ��������������� ���������
// ����:
//	list	- ����� �������������� ������
//	start	- ������ �������
//	end		- ��������� �������
// �����:
//	math	- ����� �������������� ������
// ����������:
//	�����, ����� ���� �������������� �������� ������ � �����
//	� $ � @ ��������������

void osl_FillMath(vector<OSL_LEXEM>* list,
				  vector<OSL_LEXEM>::iterator start,
				  vector<OSL_LEXEM>::iterator end,
				  vector<OSL_LEXEM>* math)
{
	bool unar = true;

	for(vector<OSL_LEXEM>::iterator it = start;
		it!= end;
		it++)
	{
		OSL_LEXEM l = *it;
		
		if(osl_GetStackPrior(&l) == 1)
		{
			// + - �������� �� �� ������� �������
			// @ $
			if(unar)
				l.token = osl_Unar(l.token[0]);
		}	
		math->push_back(l);

		// here was !osl_IsEqualcolon(l.token)
		if(!osl_IsEqualcolon(l.token) && !osl_IsLeftBracket(l.token))
			unar = false;
		else
			unar = true;
	}
}



// ������������� �������� �������
// ����:
//	list	- ����� ������, ������� ���������� ����������������
//	pool	- ������ ����������
//	storage - ������ �������� ����������� ����� OSL
// �����:
//	true/false - ������� �� ��������� �������������
// ����������:
//	���������������� ������ ���� ����������, �.�. ����� ������,
//	�������������� ; ��� } 

bool osl_InterpretSeq(vector<OSL_LEXEM>* list,
					  vector<OSL_VAR>* pool,
					  vector<OSL_FUNCTION>* func_pool,
					  OSL_TEMPLATE_STORAGE* storage)
{
	// ������ ����������, �������� � ���
	E_OSL_TEMPLATE t = storage->MatchLexems(list);



	if(t == E_OSL_NO_TEMPLATE)
	{
		osl_Warning("No template found");
		return false;
	}
	else
	{
		switch(t)
		{
		case E_OSL_DECLARE_VAR_TEMPLATE:
			{
				// %type(%name);
				vector<OSL_LEXEM>::iterator i_first = list->begin();
				OSL_LEXEM first = *i_first;

				vector<OSL_LEXEM>::iterator i_second = list->begin() + 2;
				OSL_LEXEM second = *i_second;

				E_OSL_VAR_TYPE type = osl_StringToVarType(first.token);
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

				// ����� ������, ���������� �������������� ���������
				vector<OSL_LEXEM> math;

				// ������ ������� ������
				vector<OSL_LEXEM>::iterator i_first = list->begin();
				OSL_LEXEM first = *i_first;

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

				osl_FillMath(list, list->begin()+2, list->end()-1, &math);		

				osl_Compile(&math, pool, storage);

				if(!osl_EvaluateMath(&math, var, pool))
				{
					osl_Error("Compile error", "E_OSL_ASSIGN_VAR_TEMPLATE");
					return false;
				}
				break;
			}
		case E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE:
			{
				// %type(%name)=%math;
				vector<OSL_LEXEM>::iterator i_first = list->begin();
				OSL_LEXEM first = *i_first;

				vector<OSL_LEXEM>::iterator i_second = list->begin() + 2;
				OSL_LEXEM second = *i_second;

				E_OSL_VAR_TYPE type = osl_StringToVarType(first.token);
				string name = second.token;

				osl_Compile(list, pool, storage);

				if(osl_VarDeclared(name, pool))
				{
					// TODO: warning here?
					osl_Error("Variable redeclaration", name);
					return false;
				}
				
				OSL_VAR var;		
				var.name = name;
				var.type = type;
				//memset((void*)var.data, '\0', sizeof(var.data));
				ZeroVarData(&var);
				

				osl_DeclareVar(var, pool);

				vector<OSL_LEXEM> math;
				OSL_VAR* d_var = osl_GetVar(name, pool);

				osl_FillMath(list, list->begin()+5, list->end()-1, &math);

				#ifdef _DEBUG_OSL
				osl_dDumpLexemStream(&math);
				#endif

				if(!osl_EvaluateMath(&math, d_var, pool))
				{
					osl_Error("Compile error", "E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE");
					return false;
				}

				break;
			}
		case E_OSL_FUNCTION_CALL_TEMPLATE:
			{
				// %func(%param);
				vector<OSL_LEXEM>::iterator i_first = list->begin();
				OSL_LEXEM first = *i_first;

				#ifdef _DEBUG_OSL
				osl_dDumpLexemStream(list);
				#endif

				osl_Compile(list, pool, storage);

				if(osl_IsSysFunction(first.token))
					osl_ExecuteSystemFunction(list);

				break;
			}

		default: break;
		}
	}
	return true;
}



// ������������� �������, ��������� ������������������ � �����������������
// ����:
//	list	- ����� ������, ������� ���������� ����������������
//	pool	- ����u�� ����������
//	storage - ������ �������� ����������� ����� OSL
// �����:
//	true/false - ������� �� ��������� �������������
// ����������:
//	������� ������� (�� ���������� ; ��� } �� ���������) � �������������� ���
//	����� osl_InterpretSeq 

bool osl_Interpret(vector<OSL_LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{

	vector<OSL_LEXEM> lxm_seq;
	bool braced = false;
	int brace_level = 0;

	if(!list || !pool || !storage)
	{
		osl_Error("osl_Interpret(): one or more arguments are invalid");
		return false;
	}

	for(vector<OSL_LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		OSL_LEXEM l = *it;

		if(!osl_IsSys(l.token))
			lxm_seq.push_back(l);
		else
			OSL_CURRENT_LINE++;


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
				if(!osl_InterpretSeq(&lxm_seq, pool, NULL, storage))
				{
					osl_Error("osl_InterpretSeq() FAILED");
					return false;
				}
				
				lxm_seq.resize(0);
			}
		}

		if(osl_IsRightBrace(l.token))
		{
			if(brace_level == 0)
			{
				braced = false;

				if(!osl_InterpretSeq(&lxm_seq, pool, NULL, storage))
				{
					osl_Error("osl_InterpretSeq() FAILED");
					return false;
				}

				lxm_seq.resize(0);
			}
			else
				brace_level--;
		}
	}
	#ifdef _DEBUG_OSL
	osl_dDumpVarPool(pool);
	#endif
	return true;
}




// ��� ������� ���� ���������� ������������� ��� �������� � ��������� ����� �����������
// ������ ���������� �� ��������

// � ����� ��� ���� ��������� ������������� �� ����� ���������
// �������� ����������_�������, ��������_�����, ���
// ������� ���� ����� ��������� � ����� �������, ����������� �� ���� ��������� �� ��� ���������
// �� ������� �����

// ��� ��������� �������������
enum E_OSL_INTERPRET_STATE { 
	isDeclareVar,
	isMath,
	isFor,
	isDo,
	isNo,
	isBracket,
	isBrace,
	isIterator,
	isFunctionCall,
	isIf
};

string osl_StateToString(E_OSL_INTERPRET_STATE state)
{
	switch(state)
	{
	case isDeclareVar:	return "isDeclareVar";
	case isMath:		return "isMath";
	case isFor:			return "isFor";
	case isDo:			return "isDo";
	case isNo:			return "isNo";
	case isBracket:		return "isBracket";
	case isBrace:		return "isBrace";
	case isIterator:	return "isIterator";
	case isFunctionCall: return "isFunctionCall";
	case isIf:			return "isIf";
	}
	return "";
}

struct OSL_INTERPRET_STATE {
	E_OSL_INTERPRET_STATE type; // ���
	int length; // ������������
};

bool osl_InterpretBeta(vector<OSL_LEXEM>* list, vector<OSL_VAR>* pool)
{
	vector<OSL_INTERPRET_STATE> state_vector;

	// ���������� ������ ���������
	for(vector<OSL_LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		OSL_LEXEM lx = *it;
		OSL_INTERPRET_STATE top_state;
		if(state_vector.size() > 0)
			top_state = state_vector.back();
		else
		{
			top_state.type = isNo;
			top_state.length = 0;
		}
		switch(lx.type)
		{
		case ttValidName:
			{
				if(top_state.type == isDeclareVar)
				{
					// ���������� ����������
					top_state.length++;
					state_vector.pop_back();
					state_vector.push_back(top_state);
				}
				else if(top_state.type == isMath)
				{
					// ����������
					top_state.length++;
					state_vector.pop_back();
					state_vector.push_back(top_state);
				}
				else
				{
					// ������������� �������
					// ����� ���������
					OSL_INTERPRET_STATE new_state;
					new_state.type = isFunctionCall;
					new_state.length = 1;
					state_vector.push_back(new_state);
				}
				break;
			}
		case ttVarType:
			{
				// ����� ���������
				OSL_INTERPRET_STATE new_state;
				new_state.type = isDeclareVar;
				new_state.length = 1;
				break;
			}

		case ttIntConstant:
		case ttFloatConstant:
		case ttStrConstant:
		case ttOperator:
			{
				if(top_state.type != isMath)
				{
					// ����� ���������
					OSL_INTERPRET_STATE new_state;
					new_state.type = isMath;
					new_state.length = 1;
				}
				else if(top_state.type == isFor)
				{
					// ����� ���������
					OSL_INTERPRET_STATE new_state;
					new_state.type = isIterator;
					new_state.length = 1;
				}
				else
				{
					// ����������� ������������ ����������� ���������
					top_state.length++;
					state_vector.pop_back();
					state_vector.push_back(top_state);
				}
				break;
			}

		case ttLeftBracket:
		case ttRightBracket:
			{
				if(top_state.type == isMath)
				{
					// ������ ��������������
					// ����������� ������������ ����������� ���������
					top_state.length++;
					state_vector.pop_back();
					state_vector.push_back(top_state);
				}
				else
				{
					// ������ ��������������
					// ����� ���������
					OSL_INTERPRET_STATE new_state;
					new_state.type = isBracket;
					new_state.length = 1;
					state_vector.push_back(new_state);
				}
				break;
			}

		case ttKeyword:
			{
				if(lx.token == "if")
				{
					// if
					// ����� ���������
					OSL_INTERPRET_STATE new_state;
					new_state.type = isIf;
					new_state.length = 1;
					state_vector.push_back(new_state);
				}
				break;
			}
		}
	}

	string d_string;
	for(vector<OSL_INTERPRET_STATE>::iterator it = state_vector.begin();
		it != state_vector.end();
		it++)
	{
		OSL_INTERPRET_STATE state = *it;
		d_string += osl_StateToString(state.type);
		d_string += " ";
		d_string += osl_IntToStr(state.length);
		d_string += " ";
	}

	WriteToLog(DEFAULT_OSL_LOG_NAME, d_string);






/*

	// ������� ����, � ���� ������ ��� ������� �� list
	stack<OSL_LEXEM> big_stack;

	// ���� ��������� �������������
	stack<OSL_INTERPRET_STATE> state_stack;


	for(vector<OSL_LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		OSL_LEXEM lx = *it;
		big_stack.push(lx);

		switch(lx.type)
		{
		case ttSemicolon:
		case ttRightBrace:
			{
				// ��������� ���� (���� ���������)
				stack<OSL_LEXEM> small_stack;
				// �������������� ������ ��� ���������� ����������
				vector<OSL_LEXEM> math_vector;

				// ������ ��� { ��� ; � ��������� ����
				OSL_LEXEM colon = big_stack.top();
				big_stack.pop();
				small_stack.push(colon);

				// ��������� ��������� ���� (�� ��������� { ��� ;, ��� ����� �����)		
				bool flag = false;
				while(!flag)
				{
					OSL_LEXEM slx = big_stack.top();
					big_stack.pop();
					small_stack.push(slx);		
					flag = (slx.type == ttLeftBrace ||
						slx.type == ttSemicolon ||
						big_stack.size() == 0);
				}

				// ������ ��������� ����
				flag = false;
				while(!flag)
				{
					OSL_LEXEM slx = small_stack.top();
					small_stack.pop();

					// �������������� �����
					if(osl_IsConstant(slx.token) ||
						osl_VarDeclared(slx.token, pool) ||
						osl_IsOperator(slx.token) ||
						osl_IsLeftBracket(slx.token) ||
						osl_IsRightBracket(slx.token))
					{
						if(osl_IsOperator(slx.token))
						{
							E_OSL_OPERATOR_TYPE type = osl_GetOperatorType(slx.token);

							if(type == optAssign)
							{
								OSL_VAR temp;
								if(osl_EvaluateMath(&math_vector, &temp, pool))
								{
									small_stack.push(slx); // ������ '='
									OSL_LEXEM var;
									var.token = osl_VarToString(&temp);
									switch(temp.type)
									{
									case vtInt: var.type = ttIntConstant; break;
									case vtFloat: var.type = ttFloatConstant; break;
									case vtStr: var.type = ttStrConstant; break;
									}
									small_stack.push(var); // ������ ����������
									math_vector.clear();							
								}
							}
							else
							{
								math_vector.push_back(slx);
							}
						}
						else
							math_vector.push_back(slx);
					}
					if(osl_IsLeftBracket(slx.token))
					{


				}

					



				break;
			}
		}
	}
   */
	return true;
}

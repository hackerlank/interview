#include "stdafx.h"
#include "../misc.h"
#include "osl_misc.h"
#include "osl_types.h"
#include "osl_lexems.h"
#include "osl_variables.h"
#include "osl_maths.h"


// debug
#include "osl_templates.h"
#include "osl_debug.h"
//

int osl_GetStackPrior(OSL_LEXEM* lx)
{
	for(unsigned int i = 0; i < L_S_PRIOR; i++)
	{
		if(lx->token.find_first_of(S_PRIOR[i]) != lx->token.npos)
			return i;
	}
	return -1;
}



// Унарный аналог символа
// Вход:
//	c	- символ
// Выход:
//	char - унарный аналог c, или сам c, если нет унарного аналога
// Примечания:
//	У унарных операций самый высокий приоритет. Значения прописаны жестко:
//	- ($) + (@)

char osl_Unar(char c)
{
	if(c == '+')
		return '@';
	else if(c == '-')
		return '$';
	else
		return c;
}



bool osl_IsUnar(char c)
{
	if(c == '$' || c == '@')
		return true;
	else
		return false;
}



bool osl_StackIsLowerPrior(vector<OSL_LEXEM>* stack, int prior)
{
	if(!stack)
	{
		osl_Error("osl_StackIsLowerPrior(): argument 'stack' is invalid");
		return false;
	}

	for(vector<OSL_LEXEM>::iterator it = stack->begin();
		it != stack->end();
		it++)
	{
		if(osl_GetStackPrior(&(*it)) > prior)
			return false;
	}
	return true;
}

bool osl_MathToReversePolishNotation(vector<OSL_LEXEM>* math,
									 vector<OSL_LEXEM>* polish_notation,
									 OSL_VAR* var,
									 vector<OSL_VAR>* pool)
{
	if(!math || !polish_notation || !var || !pool)
	{
		osl_Error("osl_MathToReversePolishNotation(): one or more agruments are incorrect");
		return false;
	}

	OSL_VAR_TYPE type = var->type;
	vector<OSL_LEXEM> stack;

	for(vector<OSL_LEXEM>::iterator it = math->begin();
		it != math->end();
		it++)
	{

		OSL_LEXEM lx = *it;
		if(osl_IsValidName(lx.token))
		{
			if(!osl_VarDeclared(lx.token, pool))
			{
				osl_Error("Variable undeclared", lx.token);
				return false;
			}
			else
			{
				if(type == vtStr && osl_GetVarType(lx.token, pool) != vtStr)
				{
					osl_Error("Incompatible types");
					return false;
				}
				else
				{
					OSL_VAR* temp_var = osl_GetVar(lx.token, pool);
					OSL_LEXEM constant;

					switch(temp_var->type)
					{
					case vtInt:
						{
							constant.token = osl_IntToStr(temp_var->data.int_value);
							constant.type = ttIntConstant;
							break;
						}
					case vtFloat:
						{
							constant.type = ttFloatConstant;
							constant.token = osl_FloatToStr(temp_var->data.float_value);
							break;
						}
					case vtStr:
						{
							constant.type = ttStrConstant;
							constant.token = temp_var->data.str_value;
							break;
						}
					default: break;
					}
					polish_notation->push_back(constant);	
				}
			}
		}
		else if(osl_IsConstant(lx.token))
		{
			if(osl_IsIntConstant(lx.token) || osl_IsFloatConstant(lx.token))
			{
				if(type == vtStr)
				{
					osl_Error("Incompatible types");
					return false;
				}
				else
				{
					polish_notation->push_back(lx);
				}
			}
			else
				// adding string
				polish_notation->push_back(lx);
		}
		else if(osl_IsOperator(lx.token) || osl_IsUnar(lx.token[0]))
		{
			int prior = osl_GetStackPrior(&lx);

			if(stack.empty() || osl_StackIsLowerPrior(&stack, prior))
				stack.push_back(lx);

			else if(!osl_StackIsLowerPrior(&stack, prior))
			{
				try
				{
					do
					{
						vector<OSL_LEXEM>::iterator i_last = stack.end()-1;
						OSL_LEXEM last = *i_last;
						
						polish_notation->push_back(last);
						stack.pop_back();

					} while(!osl_StackIsLowerPrior(&stack, prior));
					stack.push_back(lx);
				}
				catch(...)
				{
					osl_dMacroError(__FILE__, __FUNCTION__, __LINE__);
					return false;
				}
			}
		}
		else if(osl_IsLeftBracket(lx.token))
		{
			stack.push_back(lx);
		}
		else if(osl_IsRightBracket(lx.token))
		{
			try
			{
				do
				{
					vector<OSL_LEXEM>::iterator i_last = stack.end()-1;
					OSL_LEXEM last = *i_last;
					polish_notation->push_back(last);
					stack.pop_back();

				} while(!osl_IsLeftBracket(stack[stack.size()-1].token));
				stack.pop_back();
			}
			catch(...)
			{
				osl_dMacroError(__FILE__, __FUNCTION__, __LINE__);
				return false;
			}
		}
	}

	if(!stack.empty())
	{
		do
		{
			vector<OSL_LEXEM>::iterator i_last = stack.end()-1;
			OSL_LEXEM last = *i_last;
			polish_notation->push_back(last);
			stack.pop_back();

		} 
		while(!stack.empty());
	}

	return true;
}

OSL_VAR_TYPE osl_GetConstantType(OSL_LEXEM* lxm)
{
	if(osl_IsIntConstant(lxm->token))
		return vtInt;
	else if(osl_IsFloatConstant(lxm->token))
		return vtFloat;
	else if(osl_IsStrConstant(lxm->token))
		return vtStr;
	else
		return vtNone;
	return vtNone;
}

bool osl_Aoperand(OSL_LEXEM A, OSL_LEXEM op, OSL_LEXEM& result)
{
	char operand = op.token[0];
#ifdef _DEBUG_OSL_MATH
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to math: " + A.token + op.token);
#endif
	switch(operand)
	{
	case '@':
		{
			// унарный плюс
			if(osl_IsIntConstant(A.token))
			{
				int aa = osl_StrToInt(A.token);
				int rr = +aa;
				result.type = ttIntConstant;
				result.token = osl_IntToStr(rr);
			}
			else if(osl_IsFloatConstant(A.token))
			{
				float aa = osl_StrToFloat(A.token);
				float rr = +aa;
				result.type = ttFloatConstant;
				result.token = osl_FloatToStr(rr);
			}
			else
			{
				osl_Error("Wrong operation for string", "function: osl_Aoperand");
				return false;
			}
			break;
		}
	case '$':
		{
			// унарный минус
			if(osl_IsIntConstant(A.token))
			{
				int aa = osl_StrToInt(A.token);
				int rr = -aa;
				result.type = ttIntConstant;
				result.token = osl_IntToStr(rr);
			}
			else if(osl_IsFloatConstant(A.token))
			{
				float aa = osl_StrToFloat(A.token);
				float rr = -aa;
				result.type = ttFloatConstant;
				result.token = osl_FloatToStr(rr);
			}
			else
			{
				osl_Error("Wrong operation for string", "function: osl_Aoperand");
				return false;
			}
			break;
		}
	default: osl_Error("Unknown error", "function: osl_Aoperand"); return false;
	}
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Result: " + result.token);
	return true;
}
				


bool osl_ABoperand(OSL_LEXEM A, OSL_LEXEM B, OSL_LEXEM op, OSL_LEXEM& result)
{
	//WriteToLog(DEFAULT_OSL_LOG_NAME, "start osl_ABoperand()");
	char operand = op.token[0];
#ifdef _DEBUG_OSL_MATH
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Trying to math: " + A.token + op.token + B.token);
#endif
	try
	{
		switch(operand)
		{
		case '+':
			{
				if(osl_IsIntConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						int bb = osl_StrToInt(B.token);
						int rr = aa + bb;													
						result.type = ttIntConstant;
						result.token = osl_IntToStr(rr);
						//return true;
					}
					else if(osl_IsFloatConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						float bb = osl_StrToFloat(B.token);
						float rr = (float)aa + bb;
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and int)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsFloatConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						int bb = osl_StrToInt(B.token);
						float rr = aa + (float)bb;
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsFloatConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						float bb = osl_StrToFloat(B.token);
						float rr = aa + bb;
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and float)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsStrConstant(A.token))
				{
					if(osl_IsStrConstant(B.token))
					{
						// убираем последнюю кавычку в первой строке
						A.token.resize(A.token.length()-1);

						// убираем первую кавычку во второй строке
						B.token = B.token.substr(1,B.token.length()-1);

						string rr = A.token + B.token;
						result.type = ttStrConstant;
						result.token = rr;
					}
					else
					{
						osl_Error("Incompatible types (string and non-string)", "function: osl_ABoperand");
						return false;
					}
				}
				break;
			}
		case '-':
			{
				if(osl_IsIntConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						int bb = osl_StrToInt(B.token);
						int rr = aa - bb;													
						result.type = ttIntConstant;
						result.token = osl_IntToStr(rr);
						//return true;
					}
					else if(osl_IsFloatConstant(B.token))
					{
						int aa = osl_StrToInt(A.token.c_str());
						float bb = osl_StrToFloat(B.token.c_str());
						float rr = (float)aa - bb;					
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and int)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsFloatConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						int bb = osl_StrToInt(B.token);
						float rr = aa - (float)bb;
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsFloatConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						float bb = osl_StrToFloat(B.token);
						float rr = aa - bb;					
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and float)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsStrConstant(A.token))
				{
					osl_Error("Incompatible operation", "function: osl_ABoperand");
					return false;
				}
				break;
			}
		case '*':
			{
				if(osl_IsIntConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						int bb = osl_StrToInt(B.token);
						int rr = aa * bb;
						result.type = ttIntConstant;
						result.token = osl_IntToStr(rr);
						//return true;
					}
					else if(osl_IsFloatConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						float bb = osl_StrToFloat(B.token);
						float rr = (float)aa * bb;					
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and int)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsFloatConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						int bb = osl_StrToInt(B.token);
						float rr = aa * (float)bb;					
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						return true;
					}
					else if(osl_IsFloatConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						float bb = osl_StrToFloat(B.token);
						float rr = aa * bb;				
						result.type = ttFloatConstant;
						result.token = osl_FloatToStr(rr);
						//return true;
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and float)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsStrConstant(A.token))
				{
					osl_Error("Incompatible operation", "function: osl_ABoperand");
					return false;
				}
				break;
			}
		case '/':
			{
				if(osl_IsIntConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						int bb = osl_StrToInt(B.token);
						if(bb != 0)
						{
							float rr = (float)aa / (float)bb;			
							result.type = ttFloatConstant;
							result.token = osl_FloatToStr(rr);
							//return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsFloatConstant(B.token))
					{
						int aa = osl_StrToInt(A.token);
						float bb = osl_StrToFloat(B.token);
						if(bb != 0)
						{
							float rr = (float)aa / bb;						
							result.type = ttFloatConstant;
							result.token = osl_FloatToStr(rr);
							//return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and int)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsFloatConstant(A.token))
				{
					if(osl_IsIntConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						int bb = osl_StrToInt(B.token);
						if(bb != 0)
						{
							float rr = aa / (float)bb;						
							result.type = ttFloatConstant;
							result.token = osl_FloatToStr(rr);
							//return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsFloatConstant(B.token))
					{
						float aa = osl_StrToFloat(A.token);
						float bb = osl_StrToFloat(B.token);
						if(bb != 0)
						{
							float rr = aa / bb;						
							result.type = ttFloatConstant;
							result.token = osl_FloatToStr(rr);
							//return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsStrConstant(B.token))
					{
						osl_Error("Incompatible types (string and float)", "function: osl_ABoperand");
						return false;
					}
					else
					{
						osl_Error("Unknown error");
						return false;
					}
				}
				else if(osl_IsStrConstant(A.token))
				{

					osl_Error("Incompatible operation", "function: osl_ABoperand");
					return false;

				}
				break;
			}
		default:
			{
				osl_Error("Unsupported operation");
				return false;
			}
		}
	}
	catch(...)
	{
		osl_Error("Unknown exception", "function: ABoperand");
		return false;
	}
#ifdef _DEBUG_OSL_MATH
	WriteToLog(DEFAULT_OSL_LOG_NAME, "Result: " + result.token);
#endif
	return true; // not sure
}	

bool osl_EvaluateReversePolishNotation(vector<OSL_LEXEM>* polish_notation, OSL_VAR* var, vector<OSL_VAR>* pool)
{
	vector<OSL_LEXEM> stack;

	for(vector<OSL_LEXEM>::iterator it = polish_notation->begin();
		it != polish_notation->end();
		it++)
	{
		OSL_LEXEM lx = *it;

		if(osl_IsConstant(lx.token))
		{
			stack.push_back(lx);
		}
		else if(osl_IsOperator(lx.token) || osl_IsUnar(lx.token[0]))
		{
			OSL_LEXEM A;
			OSL_LEXEM B;
			OSL_LEXEM R;

			if(osl_IsUnar(lx.token[0]))
			{
				vector<OSL_LEXEM>::iterator aa = stack.end()-1;

				A = *aa;

				if(!osl_Aoperand(A, lx, R))
				{
					osl_Error("Error in osl_Aoperand()");
					return false;
				}
				else
				{
					stack.pop_back();
					stack.push_back(R);
				}
			}
			else
			{
				try
				{
					vector<OSL_LEXEM>::iterator bb = stack.end()-1;
					vector<OSL_LEXEM>::iterator aa = stack.end()-2;

					A = *aa;
					B = *bb;


					if(!osl_ABoperand(A, B, lx, R))
						throw "ABOPERAND_FAIL";
					stack.pop_back();
					stack.pop_back();
					stack.push_back(R);
				}
				catch(...)
				{
					osl_Error("Unknown exception", "function: osl_EvaluateReversePolishNotation");
					return false;
				}
			}
		}
	}

	if(stack.size() > 1)
	{
		osl_Error("Stack size > 1", "function: osl_EvaluateReversePolishNotation");
		return false;
	}
	else if(stack.empty())
	{
		osl_Error("Stack empty", "function: osl_EvaluateReversePolishNotation");
		return false;
	}
	else
	{
		vector<OSL_LEXEM>::iterator first = stack.begin();
		OSL_LEXEM result = *first;

		if(osl_IsIntConstant(result.token) && var->type == vtInt)
		{
			osl_SetVarData(&var->data, vtInt, result.token, false);
		}
		else if(osl_IsIntConstant(result.token) && var->type == vtFloat)
		{
			osl_SetVarData(&var->data, vtFloat, result.token, false);
		}
		else if(osl_IsFloatConstant(result.token) && var->type == vtInt)
		{
			osl_Warning("Float to int convertion");
			osl_SetVarData(&var->data, vtInt, result.token, false);
		}
		else if(osl_IsFloatConstant(result.token) && var->type == vtFloat)
		{
			osl_SetVarData(&var->data, vtFloat, result.token, false);
		}
		else if(osl_IsStrConstant(result.token) && var->type == vtStr)
		{
			osl_SetVarData(&var->data, vtStr, result.token, false);
		}
		else
		{
			osl_Error("Incompatible types", "function: osl_EvaluateReversePolishNotation");
			return false;
		}
	}
	return true;
}	

bool osl_EvaluateMath(vector<OSL_LEXEM>* math, OSL_VAR* var, vector<OSL_VAR>* pool)
{
	vector<OSL_LEXEM> reverse;
	bool ok = false;

	ok = osl_MathToReversePolishNotation(math, &reverse, var,  pool);

	if(ok)
	{
		ok = osl_EvaluateReversePolishNotation(&reverse, var, pool);
		
		if(!ok)
		{
			osl_Error("Failed to evaluate reverse polish notation");
			ok = false;
		}
	}
	else
	{
		osl_Error("Failed to converth math to reverse polish notation");
		ok = false;
	}

	return true;
}
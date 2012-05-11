#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_variables.h"
#include "osl_maths.h"


int GetStackPrior(LEXEM* lx)
{
	int prior;

	if(lx->token.find_first_of(S_PRIOR_3) != lx->token.npos)
		prior = 3;
	else if(lx->token.find_first_of(S_PRIOR_2) != lx->token.npos)
		prior = 2;
	else if(lx->token.find_first_of(S_PRIOR_1) != lx->token.npos)
		prior = 1;
	else if(lx->token.find_first_of(S_PRIOR_0) != lx->token.npos)
		prior = 0;
	else
		prior = -1;

	return prior;
}

bool StackIsLowerPrior(vector<LEXEM>* stack, int prior)
{
	for(vector<LEXEM>::iterator it = stack->begin();
		it != stack->end();
		it++)
	{
		if(GetStackPrior(&(*it)) > prior)
			return false;
	}
	return true;
}

bool osl_MathToReversePolishNotation(vector<LEXEM>* math,
									 vector<LEXEM>* polish_notation,
									 OSL_VAR* var,
									 vector<OSL_VAR>* pool)
{
	OSL_VAR_TYPE type = var->type;
	//VAR_DATA data = var->data;

	vector<LEXEM> stack; // стек для операций же


	for(vector<LEXEM>::iterator it = math->begin();
		it != math->end();
		it++)
	{
		LEXEM lx = *it;
		if(osl_IsValidName(lx.token))
		{
			if(!osl_VarDeclared(lx.token, pool))
			{
				osl_Error("Variable undeclared", lx.token);
				return false;
			}
			else
			{
				if(osl_GetVarType(lx.token, pool) != type)
				{
					osl_Error("Incompatible types");
					return false;
				}
				else
				{
					polish_notation->push_back(lx);				
				}
			}
		}
		else if(osl_IsConstant(lx.token))
		{
			if(osl_CompareConstants(type, lx.type))
			{
				polish_notation->push_back(lx);
			}
			else
			{
				osl_Error("Incompatible types");
				return false;
			}
		}
		else if(osl_IsOperator(lx.token) || osl_IsLeftBracket(lx.token))
		{
			int prior = GetStackPrior(&lx);

			if(stack.empty() || StackIsLowerPrior(&stack, prior))
				stack.push_back(lx);

			if(!StackIsLowerPrior(&stack, prior))
			{
				do
				{
					vector<LEXEM>::iterator i_last = stack.end()-1;
					LEXEM last = *i_last;
					stack.pop_back();
					polish_notation->push_back(last);

				} while(!StackIsLowerPrior(&stack, prior));
				stack.push_back(lx);
			}

			if(osl_IsLeftBracket(lx.token))
				stack.push_back(lx);
		}
		else if(osl_IsRightBracket(lx.token))
		{
			do
			{
				vector<LEXEM>::iterator i_last = stack.end()-1;
				LEXEM last = *i_last;
				polish_notation->push_back(last);
				stack.pop_back();

			} while(!osl_IsLeftBracket(stack[stack.size()-1].token));
			stack.pop_back();
		}
	}

	if(!stack.empty())
	{

		do
		{
			vector<LEXEM>::iterator i_last = stack.end()-1;
			LEXEM last = *i_last;
			polish_notation->push_back(last);
			stack.pop_back();

		} while(!stack.empty());
	}
	return true;
}

OSL_VAR_TYPE osl_GetConstantType(LEXEM* lxm)
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

bool osl_ABoperand(LEXEM* A, LEXEM* B, LEXEM* op, LEXEM& result)
{
	char operand = op->token[0];

	if(!A || !B || !op)
	{
		osl_Error("Unknown error", "function: osl_ABoperand");
		return false;
	}

	try
	{
		switch(operand)
		{
		case '+':
			{
				if(osl_IsIntConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						int bb = atoi(B->token.c_str());
						int rr = aa + bb;													
						itoa(rr, output, 10);
						result.type = ttIntConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						float bb = atof(B->token.c_str());
						float rr = aa + bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsFloatConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						int bb = atoi(B->token.c_str());
						float rr = aa + bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						float bb = atof(B->token.c_str());
						float rr = aa + bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsStrConstant(A->token))
				{
					if(osl_IsStrConstant(B->token))
					{
						string rr = A->token + B->token;
						result.type = ttStrConstant;
						result.token = rr;
						return true;
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
				if(osl_IsIntConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						int bb = atoi(B->token.c_str());
						int rr = aa - bb;													
						itoa(rr, output, 10);
						result.type = ttIntConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						float bb = atof(B->token.c_str());
						float rr = aa - bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsFloatConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						int bb = atoi(B->token.c_str());
						float rr = aa - bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						float bb = atof(B->token.c_str());
						float rr = aa - bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsStrConstant(A->token))
				{
					osl_Error("Incompatible operation", "function: osl_ABoperand");
					return false;
				}
				break;
			}
		case '*':
			{
				if(osl_IsIntConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						int bb = atoi(B->token.c_str());
						int rr = aa * bb;													
						itoa(rr, output, 10);
						result.type = ttIntConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						float bb = atof(B->token.c_str());
						float rr = aa * bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsFloatConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						int bb = atoi(B->token.c_str());
						float rr = aa * bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						float bb = atof(B->token.c_str());
						float rr = aa * bb;
						sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
						result.type = ttFloatConstant;
						result.token = string(output);
						return true;
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsStrConstant(A->token))
				{
					osl_Error("Incompatible operation", "function: osl_ABoperand");
					return false;
				}
				break;
			}
		case '/':
			{
				if(osl_IsIntConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						int bb = atoi(B->token.c_str());
						if(bb != 0)
						{
							int rr = aa / bb;													
							itoa(rr, output, 10);
							result.type = ttIntConstant;
							result.token = string(output);
							return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						int aa = atoi(A->token.c_str());
						float bb = atof(B->token.c_str());
						if(bb != 0)
						{
							float rr = aa / bb;
							sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
							result.type = ttFloatConstant;
							result.token = string(output);
							return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsFloatConstant(A->token))
				{
					if(osl_IsIntConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						int bb = atoi(B->token.c_str());
						if(bb != 0)
						{
							float rr = aa / bb;
							sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
							result.type = ttFloatConstant;
							result.token = string(output);
							return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsFloatConstant(B->token))
					{
						char output[16];
						float aa = atof(A->token.c_str());
						float bb = atof(B->token.c_str());
						if(bb != 0)
						{
							float rr = aa / bb;
							sprintf(output, fcvt(rr, C_ACC, NULL, NULL));
							result.type = ttFloatConstant;
							result.token = string(output);
							return true;
						}
						else
						{
							osl_Error("Division by zero");
							return false;
						}
					}
					else if(osl_IsStrConstant(B->token))
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
				else if(osl_IsStrConstant(A->token))
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
}
						








	
	



bool osl_EvaluateReversePolishNotation(vector<LEXEM>* polish_notation, OSL_VAR* var, vector<OSL_VAR>* pool)
{
	vector<LEXEM> stack;

	for(vector<LEXEM>::iterator it = polish_notation->begin();
		it != polish_notation->end();
		it++)
	{
		LEXEM lx = *it;

		if(osl_IsConstant(lx.token))
		{
			stack.push_back(lx);
		}
		else if(osl_IsOperator(lx.token))
		{
			LEXEM A;
			LEXEM B;
			LEXEM R;
			try
			{
				vector<LEXEM>::iterator aa = stack.end()-1;
				vector<LEXEM>::iterator bb = stack.end()-2;
				A = *aa;
				B = *bb;
				
				if(!osl_ABoperand(&A, &B, &lx, R))
					throw "ABOPERAND_FAIL";
				stack.pop_back();
				stack.pop_back();
				stack.push_back(R);
			}
			//catch("ABOPERAND_FAIL")
			//{
			//	osl_Error("ABOPERAND_FAIL");
			//	return false;
			//}
			catch(...)
			{
				osl_Error("Unknown exception", "function: osl_EvaluateReversePolishNotation");
				return false;
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
		vector<LEXEM>::iterator first = stack.begin();
		LEXEM result = *first;
		//VAR_DATA new_data;

		if(osl_IsIntConstant(result.token) && var->type == vtInt)
		{
			//new_data.int_value = atoi(result.token.c_str());
			osl_SetVarData(&var->data, vtInt, result.token, false);

			//var->data.int_value = atoi(result.token.c_str());
			//var->data.empty = false;
			//var->init = true;
			return true;
		}
		else if(osl_IsIntConstant(result.token) && var->type == vtFloat)
		{		
			var->data.float_value = (float)atoi(result.token.c_str());
			var->data.empty = false;
			var->init = true;
			return true;
		}
		else if(osl_IsFloatConstant(result.token) && var->type == vtInt)
		{
			osl_Warning("Float to int convertion");
			var->data.int_value = (int)atof(result.token.c_str());
			var->data.empty = false;
			var->init = true;
			return true;
		}
		else if(osl_IsFloatConstant(result.token) && var->type == vtFloat)
		{
			var->data.float_value = atof(result.token.c_str());
			var->data.empty = false;
			var->init = true;
			return true;
		}
		else if(osl_IsStrConstant(result.token) && var->type == vtStr)
		{
			var->data.str_value = result.token;
			var->data.empty = false;
			var->init = true;
			return true;
		}
		else
		{
			osl_Error("Incompatible types", "function: osl_EvaluateReversePolishNotation");
			return false;
		}
	}

	return false;
}
		

				








bool osl_EvaluateMath(vector<LEXEM>* math, OSL_VAR* var, vector<OSL_VAR>* pool)
{
	vector<LEXEM> reverse;

	bool ok = osl_MathToReversePolishNotation(math, &reverse, var,  pool);

	if(ok)
	{
		osl_DumpLexemStream(&reverse);

		ok = osl_EvaluateReversePolishNotation(&reverse, var, pool);
		
		if(ok)
			osl_DumpVarPool(pool);
		else
		{
			osl_Error("Failed to evaluate reverse polish notation");
			return false;
		}
	}
	else
	{
		osl_Error("Failed to converth math to reverse polish notation");
		return false;
	}

	return true;
}






	//OSL_VAR temp;

	//vector<LEXEM> new_math;
	//vector<LEXEM> brackets;

	////a = 2 * ( (a + 5) * 4) - 3 * 2 * 3 + 1

	//// brackets cycle
	//bool bracked = false;
	//int bracket_level = 0;
	//for(vector<LEXEM>::iterator it = math->begin();
	//	it != math->end();
	//	it++)
	//{
	//	LEXEM* lx = &(*it);
	//	LEXEM* next = &(*(it+1));
	//	
	//	if(osl_IsLeftBracket(lx->token))
	//	{
	//		brackets.resize(0);

	//		if(bracked)
	//			bracket_level++;
	//		bracked = true;
	//	}

	//	if(bracked)
	//	{
	//		brackets.push_back(*it);
	//	}

	//	if(osl_IsRightBracket(lx->token))
	//	{

	//		if(bracket_level == 0)
	//			bracked = false;
	//		else
	//			bracked_level--;






	//	}

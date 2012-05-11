#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_variables.h"
#include "osl_lexems.h"
#include "osl_templates.h"
#include "osl_maths.h"
#include "osl_parse.h"
#include "osl_functions.h"
#include "osl_debug.h"

#include "osl_execute.h"

extern unsigned int OSL_CURRENT_LINE;




// Проверка скобок
// Вход:
//	list	- поток лексем
// Выход:
//	true/false - успешно ли выполнена функция
// Примечания:
//	

bool osl_CheckBrackets(vector<LEXEM>* list)
{
	int bracket_level = 0;
	bool in_bracket = false;

	int brace_level = 0;
	bool in_brace = false;

	for(vector<LEXEM>::iterator it = list->begin(); it != list->end(); it++)
	{
		LEXEM lx = *it;

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



// Компиляция скрипта
// Вход:
//	list	- поток лексем, которые необходимо откомилировать
//	pool	- массив переменных
//	storage - массив шаблонов конструкция языка OSL
// Выход:
//	true/false - успешно ли выполнена компиляция
// Примечания:
//	Я не уверен, нужен ли здесь pool и storage, т.к. проверка так же выполняется и при
//	интерпретации скрипта

bool osl_Compile(vector<LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{
	if(!osl_CheckBrackets(list))
		return false;

	UINT constant_count = 0;
	for(vector<LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		LEXEM* lx = &(*it);
		if(lx->type == ttSysConstant)
		{		
			for(UINT i = 0; i < l_sysconstant; i++)
			{
				if(SYS_CONSTANTS[i].constant == lx->token)
				{
					constant_count++;
					lx->token = osl_IntToStr(SYS_CONSTANTS[i].data);
					lx->type = ttIntConstant;
					break;
				}
			}
		}
	}

	char buffer[300];

	sprintf(buffer, "%u constants recognized", constant_count);

	WriteToLog(DEFAULT_OSL_LOG_NAME, buffer);

	return true;
}



// Создание потока математического выражения
// Вход:
//	list	- поток математических лексем
//	start	- первая лексема
//	end		- последняя лексема
// Выход:
//	math	- поток математических лексем
// Примечания:
//	Также, здесь идет преобразование унарного минуса и плюса
//	в $ и @ соответственно

void osl_FillMath(vector<LEXEM>* list,
				  vector<LEXEM>::iterator start,
				  vector<LEXEM>::iterator end,
				  vector<LEXEM>* math)
{
	bool unar = true;

	for(vector<LEXEM>::iterator it = start;
		it!= end;
		it++)
	{
		LEXEM l = *it;
		
		if(osl_GetStackPrior(&l) == 1)
		{
			// + - заменяем на их унарные аналоги
			// @ $
			if(unar)
				l.token = osl_Unar(l.token[0]);
		}	
		math->push_back(l);

		if(!osl_IsEqualcolon(l.token) && !osl_IsLeftBracket(l.token))
			unar = false;
		else
			unar = true;
	}
}



// Интерпретация сегмента скрипта
// Вход:
//	list	- поток лексем, которые необходимо интерпретировать
//	pool	- массив переменных
//	storage - массив шаблонов конструкций языка OSL
// Выход:
//	true/false - успешна ли выполнена интерпретация
// Примечания:
//	Интерпретируется только одна инструкция, т.е. поток лексем,
//	оканчивающийся ; или } 

bool osl_InterpretSeq(vector<LEXEM>* list,
					  vector<OSL_VAR>* pool,
					  vector<OSL_FUNCTION>* func_pool,
					  OSL_TEMPLATE_STORAGE* storage)
{
	// шаблон инструкции, работаем с ним
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

				// поток лексем, образующих математическое выражение
				vector<LEXEM> math;

				// первый элемент потока
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

				osl_FillMath(list, list->begin()+2, list->end()-1, &math);		

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
				vector<LEXEM>::iterator i_first = list->begin();
				LEXEM first = *i_first;

				#ifdef _DEBUG_OSL
				osl_dDumpLexemStream(list);
				#endif
				


				if(osl_IsSysFunction(first.token))
					osl_ExecuteSystemFunction(list);

				break;
			}

		default: break;
		}
	}
	return true;
}



// Интерпретация скрипта, правильно лексемизированного и откомилированного
// Вход:
//	list	- поток лексем, которые необходимо интерпретировать
//	pool	- массив переменных
//	storage - массив шаблонов конструкций языка OSL
// Выход:
//	true/false - успешна ли выполнена интерпретация
// Примечания:
//	Находим сегмент (от предыдущей ; или } до следующей) и интерпретируем его
//	через osl_InterpretSeq 

bool osl_Interpret(vector<LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{

	vector<LEXEM> lxm_seq;
	bool braced = false;
	int brace_level = 0;

	if(!list || !pool || !storage)
	{
		osl_Error("osl_Compile(): one or more arguments are invalid");
		return false;
	}

	for(vector<LEXEM>::iterator it = list->begin();
		it != list->end();
		it++)
	{
		LEXEM l = *it;

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
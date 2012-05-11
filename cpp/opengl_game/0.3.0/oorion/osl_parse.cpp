#include "stdafx.h"
#include "misc.h"
#include "osl_types.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_maths.h"
#include "osl_parse.h"
#include "osl_variables.h"



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


//bool osl_ParseParams(vector<OSL_LEXEM>* list, vector<OSL_VAR>* pool)
//{
//	//for each(OSL_LEXEM lx in list)
//	for(vector<OSL_LEXEM>::iterator it = list->begin();
//		it != list->end();
//		it++)
//	{
//		OSL_LEXEM* lx = *(it);
//		if(lx->type == ttSysFunction)
//		{
//
//			osl_ParseParams(





// Компиляция скрипта
// Вход:
//	list	- поток лексем, которые необходимо откомилировать
//	pool	- массив переменных
//	storage - массив шаблонов конструкция языка OSL
// Выход:
//	true/false - успешно ли выполнена компиляция
// Примечания:
//	Здесь также заменяются переменные на их значения
//	

bool osl_Compile(vector<OSL_LEXEM>* list, vector<OSL_VAR>* pool, OSL_TEMPLATE_STORAGE* storage)
{
	if(!osl_CheckBrackets(list))
		return false;

	UINT constant_count = 0;
	UINT compiled_vars_count = 0;
	UINT system_funcs_count = 0;

	BOOL IN_FUNC = FALSE;

	//for(vector<OSL_LEXEM>::iterator it = list->begin();
	//	it != list->end();
	//	it++)
	//{
	UINT i = 0;
	while(i < list->size())
	{

		//OSL_LEXEM* lx = &(*it);
		//OSL_LEXEM lx = list[i];
		//vector<OSL_LEXEM>::iterator it = list
		//OSL_LEXEM* lx = &list[i];
		OSL_LEXEM* lx = &list->at(i);
		if(lx->type == ttSysConstant)
		{		
			for(UINT i = 0; i < l_sysconstant; i++)
			{
				if(s_sysconstant[i].name == lx->token)
				{
					constant_count++;

					lx->token = osl_IntToStr(s_sysconstant[i].data);
					lx->type = ttIntConstant;

					break;
				}
			}
		}

		if(lx->type == ttName)
		{
			if(osl_VarDeclared(lx->token, pool))
			{
				compiled_vars_count++;

				OSL_VAR* var = osl_GetVar(lx->token, pool);
				OSL_VAR_TYPE type = osl_GetVarType(lx->token, pool);
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

			/* если функция существует */
			if(func)
			{
				//func_level++;

				/* если она возвращает значение */
				if(func->return_type != vtNone)
				{
					system_funcs_count++;

					vector<OSL_LEXEM> full_list;
					vector<OSL_LEXEM> param_list;
					full_list.push_back(*lx);

					for(vector<OSL_LEXEM>::iterator it2 = list->begin() +1 +i; // + 1 было
						it2 != list->end();
						it2++)
					{
						OSL_LEXEM lx2 = *it2;

//SetGUIElem("btnHomeStarNameNext", "AddSignal", GUI_SGN_SET_VAR, 260, GetGUIElem("btnHomeStarName", "Text"));
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
							//string result = osl_ExecuteSystemFunction(&full_list);
						//if(func_level > 1)
						//{
							// ЗДЕСЬ СДЕЛАТЬ ЗАМЕНУ ФУНКЦИИ НА РЕЗУЛЬТАТ
							// !!!!!!!!!
							// !!!!!!!!
							osl_dDumpLexemStream(&full_list);
							//for(UINT j = 1; j < full_list.size(); j++)
							//{
						}


						//}

					}
				}
			}
		}
		i++;
	}

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

bool osl_InterpretSeq(vector<OSL_LEXEM>* list,
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
				vector<OSL_LEXEM>::iterator i_first = list->begin();
				OSL_LEXEM first = *i_first;

				vector<OSL_LEXEM>::iterator i_second = list->begin() + 2;
				OSL_LEXEM second = *i_second;

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
				vector<OSL_LEXEM> math;

				// первый элемент потока
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

				OSL_VAR_TYPE type = osl_StringToVarType(first.token);
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



// Интерпретация скрипта, правильно лексемизированного и откомилированного
// Вход:
//	list	- поток лексем, которые необходимо интерпретировать
//	pool	- массuив переменных
//	storage - массив шаблонов конструкций языка OSL
// Выход:
//	true/false - успешна ли выполнена интерпретация
// Примечания:
//	Находим сегмент (от предыдущей ; или } до следующей) и интерпретируем его
//	через osl_InterpretSeq 

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
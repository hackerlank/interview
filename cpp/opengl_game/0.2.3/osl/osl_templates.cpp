#include "stdafx.h"
#include "misc.h"
#include "osl_misc.h"
#include "osl_lexems.h"
#include "osl_variables.h"
#include "osl_templates.h"

OSL_TEMPLATE::tagOSL_TEMPLATE()
{
	tmplt.resize(0);
	type = E_OSL_NO_TEMPLATE;
}

bool MatchTempPatternLexem(LEXEM l, E_OSL_TEMPLATE_PATTERN p)
{
	switch(p)
	{
	case E_OSL_TYPE:
		{
			if(!osl_IsVarType(l.token))
				return false;
			break;
		}
	case E_OSL_NAME:
		{
			if(!osl_IsValidName(l.token))
				return false;
			break;
		}
	case E_OSL_MATH:
		{
			if(!osl_IsValidName(l.token) && !osl_IsConstant(l.token) && !osl_IsOperator(l.token)
				&& !osl_IsLeftBracket(l.token) && !osl_IsRightBracket(l.token))
				return false;
			break;
		}
	case E_OSL_PARAM:
		{
			if(!osl_IsValidName(l.token) && !osl_IsConstant(l.token) && !osl_IsComma(l.token))
				return false;
			break;
		}
	case E_OSL_CONDITION:
		{
			if(!osl_IsValidName(l.token) && !osl_IsConstant(l.token) && !osl_IsComparison(l.token))
				return false;
			break;
		}
	case E_OSL_CODE:
		{
			// ???
			break;
		}
	case E_OSL_FUNC:
		{
			if(!osl_IsValidName(l.token))
				return false;
			break;
		}
	case E_OSL_IF:
		{
			if(l.token != "if")
				return false;
			break;
		}
	case E_OSL_WHILE:
		{
			if(l.token != "while")
				return false;
			break;
		}
	case E_OSL_ELSE:
		{
			if(l.token != "else")
				return false;
			break;
		}
	case E_OSL_FUNCTION:
		{
			if(l.token != "function")
				return false;
			break;
		}
	case E_OSL_VARIABLE:
		{
			if(!osl_IsValidName(l.token))
				return false;
			break;
		}
	case E_OSL_LEFT_BRACKET:
		{
			if(l.token != "(")
				return false;
			break;
		}
	case E_OSL_RIGHT_BRACKET:
		{
			if(l.token != ")")
				return false;
			break;
		}
	case E_OSL_LEFT_BRACE:
		{
			if(l.token != "{")
				return false;
			break;
		}
	case E_OSL_RIGHT_BRACE:
		{
			if(l.token != "}")
				return false;
			break;
		}
	case E_OSL_SEMICOLON:
		{
			if(l.token != ";")
				return false;
			break;
		}
	case E_OSL_EQUALCOLON:
		{
			if(l.token != "=")
				return false;
			break;
		}
	default: return false;
	}
	return true;
}

bool OSL_TEMPLATE::Match(vector<LEXEM>* lexems)
{
	if(lexems->size() < tmplt.size())
		return false;

	vector<LEXEM>::iterator m = lexems->begin();

	unsigned int lxm_c = 0;

	if(!MatchTempPatternLexem(*m, *tmplt.begin()))
		return false;


	

	for(vector<E_OSL_TEMPLATE_PATTERN>::iterator p = tmplt.begin();
		p != tmplt.end();
		p++)
	{
		

		LEXEM lx = *m;

		switch(*p)
		{
		case E_OSL_TYPE:
			{
				if(!osl_IsVarType(lx.token))
				{
					osl_Error("Wrong type specifier");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_NAME:
			{
				if(!osl_IsValidName(lx.token))
				{
					osl_Error("Wrong identifier");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_MATH:
			{
				LEXEM* p_lx = &(*m);
				int iter = 0;

				while(osl_IsConstant(p_lx->token) || osl_IsValidName(p_lx->token) || osl_IsOperator(p_lx->token)
					|| osl_IsLeftBracket(p_lx->token) || osl_IsRightBracket(p_lx->token))
				{
					m++;
					lxm_c++;
					p_lx = &(*m);

				}
				continue;
			}
		case E_OSL_PARAM:
			{
				LEXEM* p_lx = &(*m);
				int iter = 0;

				while(osl_IsConstant(p_lx->token) || osl_IsValidName(p_lx->token) || osl_IsComma(p_lx->token))
				{
					iter++;		
					
					
					if(iter % 2 == 1)
					{
						if(osl_IsComma(p_lx->token))
						{
							osl_Error("Variable or constant expected");
							return false;
						}
					}
					else
					{
						if(!osl_IsComma(p_lx->token))
						{
							osl_Error("Comma expected");
							return false;
						}
						LEXEM* next = &(*(m+1));
						if(osl_IsRightBracket(next->token))
						{
							osl_Error("Unexpected token: " + next->token);
							return false;
						}
					}

					m++;
					lxm_c++;
					p_lx = &(*m);

				}
				continue;
			}
		case E_OSL_CONDITION:
			{
				LEXEM* p_lx = &(*m);
				int iter = 0;

				while(osl_IsConstant(p_lx->token) || osl_IsValidName(p_lx->token) ||
					osl_IsComparison(p_lx->token) || osl_IsEqualcolon(p_lx->token))
				{
					iter++;

					if(iter % 2 == 1)
					{
						if(!osl_IsConstant(p_lx->token) && !osl_IsValidName(p_lx->token))
						{
							osl_Error("Variable or constant expected");
							return false;
						}
					}
					else
					{
						if(osl_IsConstant(p_lx->token) || osl_IsValidName(p_lx->token))
						{
							osl_Error("Condition expected");
							return false;
						}
					}

					m++;
					lxm_c++;
					p_lx = &(*m);
				}	

				continue;

			}
		case E_OSL_CODE:
			{
				LEXEM* p_lx = &(*m);
				int iter = 0;

				while(!osl_IsRightBrace(p_lx->token))
				{
					m++;
					lxm_c++;
					p_lx = &(*m);
					
				}
				continue;
			}
		case E_OSL_FUNC:
			{
				LEXEM* p_lx = &(*m);

				if(p_lx->token != "function")
				{
					osl_Error("'function' expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_IF:
			{
				LEXEM* p_lx = &(*m);

				if(p_lx->token != "if")
				{
					osl_Error("'if' expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_WHILE:
			{
				LEXEM* p_lx = &(*m);

				if(p_lx->token != "while")
				{
					osl_Error("'while' expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_ELSE:
			{
				LEXEM* p_lx = &(*m);

				if(p_lx->token != "else")
				{
					osl_Error("'else expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_FUNCTION:
			{
				LEXEM* p_lx = &(*m);

				if(!osl_IsValidName(p_lx->token))
				{
					osl_Error("Wrong identifier");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_VARIABLE:
			{
				LEXEM* p_lx = &(*m);

				if(!osl_IsValidName(p_lx->token))
				{
					osl_Error("Wrong identifier");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_LEFT_BRACKET:
			{
				if(!osl_IsLeftBracket(lx.token))
				{
					osl_Error("Left bracket expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_RIGHT_BRACKET:
			{
				if(!osl_IsRightBracket(lx.token))
				{
					osl_Error("Right bracket expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_LEFT_BRACE:
			{
				if(!osl_IsLeftBrace(lx.token))
				{
					osl_Error("Left brace expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_RIGHT_BRACE:
			{
				if(!osl_IsRightBrace(lx.token))
				{
					osl_Error("Right brace expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_SEMICOLON:
			{
				if(!osl_IsSemicolon(lx.token))
				{
					osl_Error("Semicolon expected");
					return false;
				}
				lxm_c++;
				break;
			}
		case E_OSL_EQUALCOLON:
			{
				if(!osl_IsEqualcolon(lx.token))
				{
					osl_Error("Equalcolon expected");
					return false;
				}
				lxm_c++;
				break;
			}
		default: break;
		}
		m++;

	}
	if(lxm_c < lexems->size())
		return false;
	return true;
}

OSL_TEMPLATE_STORAGE::tagOSL_TEMPLATE_STORAGE()
{
	templates.resize(0);
}

void OSL_TEMPLATE_STORAGE::AddTemplate(E_OSL_TEMPLATE type, vector<E_OSL_TEMPLATE_PATTERN> tmplt)
{
	OSL_TEMPLATE temp;
	temp.tmplt = tmplt;
	temp.type = type;

	templates.push_back(temp);
}

E_OSL_TEMPLATE OSL_TEMPLATE_STORAGE::MatchLexems(vector<LEXEM>* lexems)
{
	for(vector<OSL_TEMPLATE>::iterator it = templates.begin();
		it != templates.end();
		it++)
	{
		OSL_TEMPLATE t = *it;
		if(t.Match(lexems))
			return t.type;
	}
	return E_OSL_NO_TEMPLATE;
}
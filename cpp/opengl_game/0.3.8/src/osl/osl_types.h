// Тип переменной - osl_variables.cpp
// --------------------------------------------------------------------------
enum E_OSL_VAR_TYPE { vtInt, vtFloat, vtStr, vtNone };
struct OSL_VAR_TYPE {
	E_OSL_VAR_TYPE type;
	string name;
};
// --------------------------------------------------------------------------



// Тип оператора - osl_lexems.cpp
// --------------------------------------------------------------------------
enum E_OSL_OPERATOR_TYPE { optPlus, optMinus, optMult, optDvd,
	optGrt, optGrtEq, optEq, optLs, optLsEq, optAssign, optNone };
struct OSL_OPERATOR {
	E_OSL_OPERATOR_TYPE type;
	string name;
};
// --------------------------------------------------------------------------


// Тип синтаксической лексемы
// --------------------------------------------------------------------------
enum E_OSL_KEYWORD_TYPE { ktFor, ktWhile, ktIf, ktElse, ktDo, ktBreak, ktConst, ktFunction };
struct OSL_KEYWORD {
	E_OSL_KEYWORD_TYPE type;
	string name;
};
// --------------------------------------------------------------------------



// Тип шаблона - osl_templates.cpp
// --------------------------------------------------------------------------
enum E_OSL_TEMPLATE { 
				E_OSL_DECLARE_VAR_TEMPLATE,
				E_OSL_ASSIGN_VAR_TEMPLATE,
				E_OSL_DECLARE_ASSIGN_VAR_TEMPLATE,
				E_OSL_DECLARE_FUNCTION_TEMPLATE,
				E_OSL_FUNCTION_CALL_TEMPLATE,
				E_OSL_IF_TEMPLATE,
				E_OSL_IF_ELSE_TEMPLATE,
				E_OSL_WHILE_TEMPLATE,
				E_OSL_DO_TEMPLATE,
				E_OSL_NO_TEMPLATE
};
// --------------------------------------------------------------------------



// Тип паттерна шаблона - osl_templates.cpp
// --------------------------------------------------------------------------
enum E_OSL_TEMPLATE_PATTERN {
				E_OSL_TYPE,
				E_OSL_NAME,
				E_OSL_MATH,
				E_OSL_PARAM,
				E_OSL_CONDITION,
				E_OSL_CODE,
				E_OSL_FUNC,
				E_OSL_IF,
				E_OSL_WHILE,
				E_OSL_ELSE,
				E_OSL_DO,
				E_OSL_FUNCTION,
				E_OSL_VARIABLE,
				E_OSL_LEFT_BRACKET,
				E_OSL_RIGHT_BRACKET,
				E_OSL_LEFT_BRACE,
				E_OSL_RIGHT_BRACE,
				E_OSL_SEMICOLON,
				E_OSL_EQUALCOLON,
				E_OSL_NO_PATTERN
};
// --------------------------------------------------------------------------



// Тип элемента GUI - osl_makegui.cpp
// --------------------------------------------------------------------------
enum GUI_ELEMENT_TYPE { 
	guiBaseControl,
	guiVidget,
	guiLabel,
	guiIcon,
	guiContainer,
	guiPanel,
	guiProgressBar,
	guiControl,
	guiButton,
	guiCheckBox,
	guiRadioButton,
	guiMemo,
	guiTextBox,
	guiNone
};
// --------------------------------------------------------------------------



// Тип токена - osl_lexems.cpp
// --------------------------------------------------------------------------
enum E_TOKEN_TYPE {
	ttVarType,
	ttOperator,
	ttSemicolon,
	ttStrConstant,
	ttIntConstant,
	ttFloatConstant,
	ttValidName,
	ttSysFunction,
	ttComma,
	ttEqualColon,
	ttLeftBracket,
	ttRightBracket,
	ttLeftBrace,
	ttRightBrace,
	ttSysConstant,
	ttKeyword,
	ttSys,
	ttUnknown
};
// --------------------------------------------------------------------------



// Целочисленная переменная - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_INT_VAR {
	string name;
	int value;
	bool init;
} OSL_INT_VAR;
// --------------------------------------------------------------------------



// Дробная переменная - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_FLOAT_VAR {
	string name;
	float value;
	bool init;
} OSL_FLOAT_VAR;
// --------------------------------------------------------------------------



// Строковая переменная - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_STR_VAR {
	string name;
	string value;
	bool init;
} OSL_STR_VAR;
// --------------------------------------------------------------------------



// Значение переменной - osl_variables.cpp
// --------------------------------------------------------------------------
union OSL_VARDATA {
	char* str_value;
	float float_value;
	int int_value;
};
// --------------------------------------------------------------------------



// Тип переменной - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagVAR {
	string name;
	bool init;
	OSL_VARDATA data;
	E_OSL_VAR_TYPE type;
} OSL_VAR;
// --------------------------------------------------------------------------



// Тип лексемы - osl_lexems.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_LEXEM {
	E_TOKEN_TYPE type;
	string token;

	tagOSL_LEXEM(E_TOKEN_TYPE t): type(t) { token.resize(0); }
	tagOSL_LEXEM(): type(ttUnknown) { token.resize(0); }
} OSL_LEXEM;
// --------------------------------------------------------------------------



// Тип шаблона - osl_templates.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_TEMPLATE {

	vector<E_OSL_TEMPLATE_PATTERN> tmplt;
	E_OSL_TEMPLATE type;

	tagOSL_TEMPLATE();

	bool Match(vector<OSL_LEXEM>* lexems);

} OSL_TEMPLATE;
// --------------------------------------------------------------------------



// Массив шаблонов - osl.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_TEMPLATE_STORAGE {

	vector<OSL_TEMPLATE> templates;

	tagOSL_TEMPLATE_STORAGE();

	void AddTemplate(E_OSL_TEMPLATE type, vector<E_OSL_TEMPLATE_PATTERN> tmplt);
	E_OSL_TEMPLATE MatchLexems(vector<OSL_LEXEM>* OSL_LEXEMs);

	bool init;

} OSL_TEMPLATE_STORAGE;
// --------------------------------------------------------------------------


// Тип константы
// --------------------------------------------------------------------------
typedef struct _OSL_CONSTANT {
	string name;
	int data;
} OSL_CONSTANT;
// --------------------------------------------------------------------------



// Тип функции
// --------------------------------------------------------------------------
typedef struct tagOSL_FUNCTION {
	string name;
	int param_count; // -1 - неограничено
	bool system;
	E_OSL_VAR_TYPE return_type;
} OSL_FUNCTION;
// --------------------------------------------------------------------------

// ��� ���������� - osl_variables.cpp
// --------------------------------------------------------------------------
enum OSL_VAR_TYPE { vtInt, vtFloat, vtStr, vtNone };
// --------------------------------------------------------------------------



// ��� ������� - osl_templates.cpp
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



// ��� �������� ������� - osl_templates.cpp
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



// ��� �������� GUI - osl_makegui.cpp
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



// ��� ������ - osl_lexems.cpp
// --------------------------------------------------------------------------
enum E_TOKEN_TYPE {
	ttVarType,
	ttOperator,
	ttSemicolon,
	ttEqualcolon,
	ttStrConstant,
	ttIntConstant,
	ttFloatConstant,
	ttName,
	ttSysFunction,
	ttComma,
	ttLeftBracket,
	ttRightBracket,
	ttLeftBrace,
	ttRightBrace,
	ttComparison,
	ttSysConstant,
	ttKeyword,
	ttSys,
	ttUnknown
};
// --------------------------------------------------------------------------



// ������������� ���������� - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_INT_VAR {
	string name;
	int value;
	bool init;
} OSL_INT_VAR;
// --------------------------------------------------------------------------



// ������� ���������� - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_FLOAT_VAR {
	string name;
	float value;
	bool init;
} OSL_FLOAT_VAR;
// --------------------------------------------------------------------------



// ��������� ���������� - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_STR_VAR {
	string name;
	string value;
	bool init;
} OSL_STR_VAR;
// --------------------------------------------------------------------------



// �������� ���������� - osl_variables.cpp
// --------------------------------------------------------------------------
union OSL_VARDATA {
	char* str_value;
	float float_value;
	int int_value;
};
// --------------------------------------------------------------------------



// ��� ���������� - osl_variables.cpp
// --------------------------------------------------------------------------
typedef struct tagVAR {
	string name;
	OSL_VARDATA data;
	bool init;
	OSL_VAR_TYPE type;
} OSL_VAR;
// --------------------------------------------------------------------------



// ��� ������� - osl_lexems.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_LEXEM {
	E_TOKEN_TYPE type;
	string token;

	tagOSL_LEXEM(E_TOKEN_TYPE t): type(t) { token.resize(0); }
	tagOSL_LEXEM(): type(ttUnknown) { token.resize(0); }
} OSL_LEXEM;
// --------------------------------------------------------------------------



// ��� ������� - osl_templates.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_TEMPLATE {

	vector<E_OSL_TEMPLATE_PATTERN> tmplt;
	E_OSL_TEMPLATE type;

	tagOSL_TEMPLATE();

	bool Match(vector<OSL_LEXEM>* lexems);

} OSL_TEMPLATE;
// --------------------------------------------------------------------------



// ������ �������� - osl.cpp
// --------------------------------------------------------------------------
typedef struct tagOSL_TEMPLATE_STORAGE {

	vector<OSL_TEMPLATE> templates;

	tagOSL_TEMPLATE_STORAGE();

	void AddTemplate(E_OSL_TEMPLATE type, vector<E_OSL_TEMPLATE_PATTERN> tmplt);
	E_OSL_TEMPLATE MatchLexems(vector<OSL_LEXEM>* OSL_LEXEMs);

} OSL_TEMPLATE_STORAGE;
// --------------------------------------------------------------------------



//// ��������� ���������
//// --------------------------------------------------------------------------
//typedef struct _OSL_SYSCONSTANT {
//	string constant;
//	//union OSL_VARDATA data;
//	int data;
//} OSL_SYSCONSTANT;
//// --------------------------------------------------------------------------



// ��� ���������
// --------------------------------------------------------------------------
typedef struct _OSL_CONSTANT {
	string name;
	//union OSL_VARDATA data;
	int data;
} OSL_CONSTANT;
// --------------------------------------------------------------------------



// ��� �������
// --------------------------------------------------------------------------
typedef struct tagOSL_FUNCTION {
	string name;
	int param_count; // -1 - ������������
	bool system;
	OSL_VAR_TYPE return_type;
} OSL_FUNCTION;
// --------------------------------------------------------------------------
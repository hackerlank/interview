enum I_TOKEN_TYPE { ttKeyword, ttOperator, ttStrConstant,
ttIntConstant, ttFloatConstant, ttVariable, ttFunction, ttDivider, ttSysConstant, ttUnknown };

typedef struct tagLEXEM {
	I_TOKEN_TYPE type;
	string token;

	tagLEXEM(I_TOKEN_TYPE t): type(t) { token.resize(0); }
	tagLEXEM(): type(ttUnknown) { token.resize(0); }
} LEXEM;

typedef struct tagINT_VAR {
	string name;
	int value;
} INT_VAR;

typedef struct tagFLOAT_VAR {
	string name;
	float value;
} FLOAT_VAR;

typedef struct tagSTR_VAR {
	string name;
	string value;
} STR_VAR;

const string a_dividers[] = { ",", ".", ";", "(", ")", "{", "}", "[", "]" }; 
const int a_dividers_l = 9;

const string a_operators[] = {"==", "<=", ">=", "++", "--", "=", "+", "!", "-", "*", "/", "%", "<",  ">" };
const int a_operators_l = 14;

const string a_functions[] = { "print", "textout", "debug", "glEnable", "glDisable", "tostr" };
const int a_functions_l = 6;

const string a_keywords[] = { "const", "goto", "int", "break", "for", "float", "string", "while" };
const int a_keywords_l = 8;

const string a_sysconstants[] = { "GL_TEXTURE_2D", "GL_BLEND" };
const int a_sysconstants_l = 2;

const string a_deprecated[] = { "$", "@", "!", "&", "-", "+", "%", "*", "^", "=" };
const int a_deprecated_l = 10;

typedef struct tagLEXEM_SWITCHER {
	bool int_switch;
	bool float_switch;
	bool str_switch;

	bool for_switch;
	bool while_switch;

	bool bracket_switch;
	bool brace_switch;
	bool sq_bracket_switch;

	bool comma_switch;
	bool dot_switch;
	bool semicolon_switch;

	bool equal_switch;

	bool op;
	bool minus_switch;
	bool plus_switch;
	bool multiply_switch;
	bool divide_switch;
	bool mod_switch;

	bool not_switch;
	bool less_switch;
	bool more_switch;
	bool less_equal_switch;
	bool more_equal_switch;

	void SwitchAllOff()
	{
	    int_switch = false;
	    float_switch = false;
	    str_switch = false;
	    for_switch = false;
	    while_switch = false;
		op = false;
		bracket_switch = false;
		brace_switch = false;
		sq_bracket_switch = false;
	    comma_switch = false;
	    dot_switch = false;
	    semicolon_switch = false;
	    equal_switch = false;
	    minus_switch = false;
	    plus_switch = false;
	    multiply_switch = false;
	    divide_switch = false;
	    mod_switch = false;
	    not_switch = false;
	    less_switch = false;
	    more_switch = false;
	    less_equal_switch = false;
	    more_equal_switch = false;
	}

	tagLEXEM_SWITCHER()
	{
		SwitchAllOff();
	}


} LEXEM_SWITCHER;

enum VAR_TYPE { vtInt, vtFloat, vtStr, vtNone };

bool osl_Input(const char* file_name, char** dest);
void osl_DumpInputStream(char* stream);
void osl_DumpLexemStream(std::vector<LEXEM>* lexem_stream);
bool osl_IsDivider(string s);
bool osl_IsOperator(string s);
bool osl_IsFunction(string s);
bool osl_IsKeyword(string s);
bool osl_IsSysConstant(string s);
bool osl_IsStrConstant(string s);
bool osl_IsVariable(string s);
I_TOKEN_TYPE osl_GetToken(string s);
int osl_Lexemize(char* char_stream, std::vector<LEXEM>* lexem_stream);
bool osl_MakeList(std::vector<LEXEM>* lexem_stream, std::vector<string>* list);

inline void osl_Error(string err_code);
inline void osl_Warning(string warn_code);

bool osl_IntVarDeclared(string var, vector<INT_VAR>* pool);
bool osl_FloatVarDeclared(string var, vector<FLOAT_VAR>* pool);
bool osl_StrVarDeclared(string var, vector<STR_VAR>* pool);
void osl_DeclareInt(string var, vector<INT_VAR>* pool);
void osl_DeclareFloat(string var, vector<FLOAT_VAR>* pool);
void osl_DeclareStr(string var, vector<STR_VAR>* pool);
void osl_SetIntValue(int value, string var, vector<INT_VAR>* pool);
void osl_SetFloatValue(float value, string var, vector<FLOAT_VAR>* pool);
void osl_SetStrValue(string value, string var, vector<STR_VAR>* pool);
INT_VAR* osl_GetIntVar(string var, vector<INT_VAR>* pool);
FLOAT_VAR* osl_GetFloatVar(string var, vector<FLOAT_VAR>* pool);
STR_VAR* osl_GetStrVar(string var, vector<STR_VAR>* pool);
VAR_TYPE osl_GetVarType(string var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);
bool osl_ExecuteMath(vector<string>* source, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);
void DumpExecuteList(vector<string>* list);
int osl_GetInt(string operand_var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);
float osl_GetFloat(string operand_var, vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);
string osl_GetStr(string operand_var,  vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);
bool osl_DoPlus(string result_var, VAR_TYPE result_type, string operand_var,
				vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);
bool osl_DoAssign(string result_var, VAR_TYPE result_type, string operand_var,
				  vector<INT_VAR>* int_pool, vector<FLOAT_VAR>* float_pool, vector<STR_VAR>* str_pool);

const string W1001 = "Warning: redeclaration";
const string W1002 = "Warning: enclosed brackets aren't supported yet";
const string W1003 = "Warning: float to int conversion";
const string S1001 = "Syntax error: \" expected";
const string S1002 = "Syntax error: variable expected, but keyword found";
const string S1003 = "Syntax error: ']' expected";
const string S1004 = "Syntax error: ']' unexpected";
const string S1005 = "Syntax error: matching closing/opening bracket not found";
const string S1006 = "Syntax error: type modifier not found";
const string S1007 = "Syntax error: unexpected token";
const string S1008 = "Syntax error: constant expected";
const string P1001 = "Parse error: variable undeclared";
const string P1002 = "Parse error: incompatible types";
const string P1003 = "Parse error: incorrect input string for execution (osl_ExecuteMath)";
const string P1004 = "Parse error: unterminated input string for execution (osl_ExecuteMath), forgot ';' ?";
const string P1005 = "Parse error: constant or variable expected (osl_ExecuteMath)";
const string P1006 = "Parse error: digital constant or variable expected (osl_ExecuteMath)";
const string P1007 = "Parse error: string constant or variable expected (osl_ExecuteMath)";
const string P1008 = "Parse error: unknown error";

#define COMMENT_CHAR '#'
#define SKIP_CHAR '|'





const string s_OSL_VAR_TYPE[] = {
	"int",
	"float",
	"string"
};
const UINT l_OSL_VAR_TYPE = 3;

const string s_operator[] = {
	"+",
	"-",
	"*",
	"/",
	"%",
	"^"
};
const UINT l_operator = 6;

const OSL_FUNCTION s_sysfunction[] =
{
	{ "echo", 1, true, vtNone },
	{ "exit", 0, true, vtNone },
	{ "init", 1, true, vtInt },
	{ "load", 1, true, vtInt }, // вернуть значение
	{ "loadfont", 4, true, vtInt },
	{ "loadfontfromfile", 3, true, vtInt },
	{ "loadtexture", 1, true, vtInt },
	{ "loadconfig", 1, true, vtInt },
	{ "newguielem", 2, true, vtNone },
	{ "setguielem", -1, true, vtNone },
	{ "getguielem", 2, true, vtStr }
};
const UINT l_sysfunction = 11;


const OSL_CONSTANT s_sysconstant[] =
{
	{ "gui_sgn_open_group", 550 },
	{ "gui_sgn_exit", 650 },
	{ "gui_sgn_set_var", 750 },
	{ "gui_sgn_start_game", 850 }
};
const UINT l_sysconstant = 4;


const string s_comparison[] = {
	"<",
	">",
	"<=",
	">="
};
const UINT l_comparison = 4;



//const string s_sysconstant[] = {
//	"gui_sgn_open_group",
//	"gui_sgn_exit",
//	"gui_sgn_set_var",
//	"gui_sgn_start_game"
//};
//const UINT l_sysconstant = 4;
//
//const OSL_SYSCONSTANT SYS_CONSTANTS[] =
//{
//	{ "gui_sgn_open_group", 550 },
//	{ "gui_sgn_exit",		650 },
//	{ "gui_sgn_set_var",	750 },
//	{ "gui_sgn_start_game", 850 }
//};

const string s_keyword[] = {
	"function",
	"if",
	"else",
	"while",
	"do",
	"break",
	"const"
};
const UINT l_keyword = 7;

const string s_sys[] = {
	"|"
};
const UINT l_sys = 1;


bool osl_Input(const char* file_name, char** dest);

E_TOKEN_TYPE osl_GetToken(string s);
OSL_FUNCTION* osl_GetSysFunction(string name);

bool osl_IsSemicolon(string s);
bool osl_IsComma(string s);
bool osl_IsEqualcolon(string s);
bool osl_IsLeftBracket(string s);
bool osl_IsRightBracket(string s);
bool osl_IsLeftBrace(string s);
bool osl_IsRightBrace(string s);
bool osl_IsOperator(string s);
bool osl_IsComparison(string s);
bool osl_IsVarType(string s);
bool osl_IsStrConstant(string s);
bool osl_IsIntConstant(string s);
bool osl_IsFloatConstant(string s);
bool osl_IsSysFunction(string s);
bool osl_IsSysConstant(string s);
bool osl_IsKeyword(string s);
bool osl_IsValidName(string s);
bool osl_IsSys(string s);
bool osl_IsConstant(string s);


int osl_Lexemize(char* char_stream, std::vector<OSL_LEXEM>* lexem_stream);
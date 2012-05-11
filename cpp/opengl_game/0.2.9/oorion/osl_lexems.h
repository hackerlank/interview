#define COMMENT_CHAR '#'
#define SKIP_CHAR '|'


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

typedef struct tagLEXEM {
	E_TOKEN_TYPE type;
	string token;

	tagLEXEM(E_TOKEN_TYPE t): type(t) { token.resize(0); }
	tagLEXEM(): type(ttUnknown) { token.resize(0); }
} LEXEM;

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

const string s_sysfunction[] = {
	"echo",
	"exit",
	"init",
	"loadfont",
	"loadfontfromfile",
	"loadtexture",
	"loadgui",
	"loadconfig",
	"newguielem",
	"setguielem",
	"getguielem",
	"addrace",
	"load"
};
const UINT l_sysfunction = 13;

const string s_comparison[] = {
	"<",
	">",
	"<=",
	">="
};
const UINT l_comparison = 4;

typedef struct _OSL_SYSCONSTANT {
	string constant;
	//union OSL_VARDATA data;
	int data;
} OSL_SYSCONSTANT;

const string s_sysconstant[] = {
	"gui_sgn_open_group",
	"gui_sgn_exit",
	"gui_sgn_set_var",
	"gui_sgn_start_game"
};
const UINT l_sysconstant = 4;

const OSL_SYSCONSTANT SYS_CONSTANTS[] =
{
	{ "gui_sgn_open_group", 550 },
	{ "gui_sgn_exit",		650 },
	{ "gui_sgn_set_var",	750 },
	{ "gui_sgn_start_game", 850 }
};

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


int osl_Lexemize(char* char_stream, std::vector<LEXEM>* lexem_stream);
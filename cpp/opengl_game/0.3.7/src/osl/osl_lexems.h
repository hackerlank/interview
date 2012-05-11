#define COMMENT_CHAR '#'
#define SKIP_CHAR '|'
#define INTERNAL_CONSTANT 65220




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
	{ "load", 1, true, vtInt },
	{ "loadfont", 4, true, vtInt },
	{ "loadfontfromfile", 3, true, vtInt },
	{ "loadtexture", 1, true, vtInt },
	{ "loadresource", 1, true, vtInt },
	{ "loadconfig", 1, true, vtInt },
	{ "loaddialog", 1, true, vtInt },
	{ "loadsound", 1, true, vtInt },
	{ "newguielem", 2, true, vtNone },
	{ "setguielem", -1, true, vtNone }, // -1 потому что несколько функций
	{ "getguielem", 2, true, vtStr },
	{ "createplayer", 6, true, vtNone },
	{ "createpowerup", 5, true, vtNone },
	{ "createenemy", 4, true, vtNone },
	{ "createevent", 5, true, vtNone },
	{ "createsprite", 3, true, vtNone },
	{ "setworldmaxx", 1, true, vtNone },
	{ "setworldminx", 1, true, vtNone }
};
const UINT l_sysfunction = 21;

const OSL_CONSTANT s_sysconstant[] =
{
	{ "gui_sgn_open_group", 550 },
	{ "gui_sgn_exit", 650 },
	{ "gui_sgn_set_var", 750 },
	{ "gui_sgn_start_game", 850 },
	{ "gui_sgn_load_file", 950 },
	{ "event_action_hero_say", 25 },
	{ "event_action_start_dialog", 26 },
	{ "event_action_set_ai", 27 },
	{ "event_action_set_pause", 28 },
	{ "event_action_disable_keyboard", 29 },
	{ "event_action_hide_hud", 30 },
	{ "event_action_disable_id_ai", 31 },
	{ "event_action_enable_id_ai", 32 },
	{ "event_action_move_id", 33 },
	{ "event_action_stop_id", 34 },
	{ "event_action_sound_play", 50 },
	{ "event_condition_time_elapsed", 1 },
	{ "event_condition_hero_x_gr", 2 },
	{ "event_condition_id_x_gr", 3 },
	{ "event_condition_id_x_eq", 4 },
	{ "animstand", 4 },
	{ "pwrhealth", 0 },
	{ "pwrscore", 1 },
	{ "pwrammo", 2 },
	{ "sprite", 0 },
	{ "animatedsprite", 1 },
	{ "character", 2 },
	{ "none", 3 },
	{ "sys_scr_width", INTERNAL_CONSTANT },
	{ "sys_scr_height", INTERNAL_CONSTANT }
};
const UINT l_sysconstant = 30;


const string s_comparison[] = {
	"<",
	">",
	"<=",
	">="
};
const UINT l_comparison = 4;


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
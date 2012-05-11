#ifndef __OSL_LEXEMS_H_
#define __OSL_LEXEMS_H_

#define SKIP_CHAR '|'
#define INTERNAL_CONSTANT 65220


void osl_AddVarType(string name, E_OSL_VAR_TYPE type);
void osl_AddOperator(string name, E_OSL_OPERATOR_TYPE type);
void osl_AddSysFunction(string name, int param_count, bool system, E_OSL_VAR_TYPE return_type);
void osl_AddSysConstant(string name, int value);
void osl_AddKeyword(string name, E_OSL_KEYWORD_TYPE type);

void osl_InitLexemizer();
bool osl_InputFile(const char* file_name, char** dest);
bool osl_InputString(const char* input, char** dest);
int osl_Lexemize(char* char_stream, vector<OSL_LEXEM>* lexem_stream);

E_OSL_OPERATOR_TYPE osl_GetOperatorType(string s);


bool osl_IsEqualcolon(string s);
bool osl_IsSemicolon(string s);
bool osl_IsComma(string s);
bool osl_IsLeftBracket(string s);
bool osl_IsRightBracket(string s);
bool osl_IsLeftBrace(string s);
bool osl_IsRightBrace(string s);
bool osl_IsOperator(string s);
bool osl_IsVarType(string s);
bool osl_IsStrConstant(string s);
bool osl_IsIntConstant(string s);
bool osl_IsFloatConstant(string s);
bool osl_IsSysFunction(string s);
bool osl_IsSysConstant(string s);
bool osl_IsKeyword(string s);
bool osl_IsValidName(string s);
bool osl_IsConstant(string s);
bool osl_IsSys(string s);
E_TOKEN_TYPE osl_GetToken(string s);
OSL_FUNCTION* osl_GetSysFunction(string name);

#endif
const string S_OSL_VAR_TYPE = "intfloatstring";

#define C_ACC 5



void osl_DeclareVar(OSL_VAR var, vector<OSL_VAR>* pool);

void osl_UndeclareVar(string name, vector<OSL_VAR>* pool);

bool osl_VarDeclared(string name, vector<OSL_VAR>* pool);

OSL_VAR* osl_GetVar(string name, vector<OSL_VAR>* pool);
bool osl_SetVar(string name, OSL_VARDATA data, vector<OSL_VAR>* pool);

bool osl_VarInitted(string name, vector<OSL_VAR>* pool);

OSL_VARDATA osl_GetVarData(string name, vector<OSL_VAR>* pool);
void osl_SetVarData(OSL_VARDATA* data, OSL_VAR_TYPE type, string what, bool add);

OSL_VAR_TYPE osl_GetVarType(string name, vector<OSL_VAR>* pool);

bool osl_CompareConstants(OSL_VAR_TYPE t, enum E_TOKEN_TYPE c);


string osl_VarTypeToString(OSL_VAR_TYPE type);
string osl_VarToString(OSL_VAR* var);

OSL_VAR_TYPE osl_StringToVarType(string s);



























//bool osl_VarInitted(string OSL_VAR, vector<OSL_INT_VAR>* int_pool, vector<OSL_FLOAT_VAR>* float_pool, vector<OSL_STR_VAR>* str_pool);
//bool osl_IntVarDeclared(string OSL_VAR, vector<OSL_INT_VAR>* pool);
//bool osl_FloatVarDeclared(string OSL_VAR, vector<OSL_FLOAT_VAR>* pool);
//bool osl_StrVarDeclared(string OSL_VAR, vector<OSL_STR_VAR>* pool);
//void osl_DeclareInt(string OSL_VAR, vector<OSL_INT_VAR>* pool);
//void osl_DeclareFloat(string OSL_VAR, vector<OSL_FLOAT_VAR>* pool);
//void osl_DeclareStr(string OSL_VAR, vector<OSL_STR_VAR>* pool);
//void osl_SetIntValue(int value, string OSL_VAR, vector<OSL_INT_VAR>* pool);
//void osl_SetFloatValue(float value, string OSL_VAR, vector<OSL_FLOAT_VAR>* pool);
//void osl_SetStrValue(string value, string OSL_VAR, vector<OSL_STR_VAR>* pool);
//OSL_INT_VAR* osl_GetIntVar(string OSL_VAR, vector<OSL_INT_VAR>* pool);
//OSL_FLOAT_VAR* osl_GetFloatVar(string OSL_VAR, vector<OSL_FLOAT_VAR>* pool);
//OSL_STR_VAR* osl_GetStrVar(string OSL_VAR, vector<OSL_STR_VAR>* pool);
//OSL_VAR_TYPE osl_GetVarType(string OSL_VAR, vector<OSL_INT_VAR>* int_pool, vector<OSL_FLOAT_VAR>* float_pool, vector<OSL_STR_VAR>* str_pool);
//int osl_GetInt(string operand_var, vector<OSL_INT_VAR>* int_pool, vector<OSL_FLOAT_VAR>* float_pool, vector<OSL_STR_VAR>* str_pool);
//float osl_GetFloat(string operand_var, vector<OSL_INT_VAR>* int_pool, vector<OSL_FLOAT_VAR>* float_pool, vector<OSL_STR_VAR>* str_pool);
//string osl_GetStr(string operand_var,  vector<OSL_INT_VAR>* int_pool, vector<OSL_FLOAT_VAR>* float_pool, vector<OSL_STR_VAR>* str_pool);
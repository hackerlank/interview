#define C_ACC 5

void osl_DeclareVar(OSL_VAR var, vector<OSL_VAR>* pool);
void osl_UndeclareVar(string name, vector<OSL_VAR>* pool);
void osl_FreeAllVar(vector<OSL_VAR>* pool);
bool osl_VarDeclared(string name, vector<OSL_VAR>* pool);
OSL_VAR* osl_GetVar(string name, vector<OSL_VAR>* pool);
bool osl_SetVar(string name, OSL_VARDATA data, vector<OSL_VAR>* pool);
bool osl_VarInitted(string name, vector<OSL_VAR>* pool);
OSL_VARDATA osl_GetVarData(string name, vector<OSL_VAR>* pool);
void osl_SetVarData(OSL_VARDATA* data, E_OSL_VAR_TYPE type, string what, bool add);
E_OSL_VAR_TYPE osl_GetVarType(string name, vector<OSL_VAR>* pool);
bool osl_CompareConstants(E_OSL_VAR_TYPE t, enum E_TOKEN_TYPE c);
string osl_VarTypeToString(E_OSL_VAR_TYPE type);
string osl_VarToString(OSL_VAR* var);
E_OSL_VAR_TYPE osl_StringToVarType(string s);
void ZeroVarData(OSL_VAR* var);
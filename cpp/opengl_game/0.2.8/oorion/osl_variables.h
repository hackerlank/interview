enum OSL_VAR_TYPE { vtInt, vtFloat, vtStr, vtNone };
const string S_OSL_VAR_TYPE = "intfloatstring";

#define C_ACC 5

typedef struct tagOSL_INT_VAR {
	string name;
	int value;
	bool init;
} OSL_INT_VAR;

typedef struct tagOSL_FLOAT_VAR {
	string name;
	float value;
	bool init;
} OSL_FLOAT_VAR;

typedef struct tagOSL_STR_VAR {
	string name;
	string value;
	bool init;
} OSL_STR_VAR;

union OSL_VARDATA {
	char* str_value;
	float float_value;
	int int_value;
};

typedef struct tagVAR {
	string name;
	OSL_VARDATA data;
	bool init;
	OSL_VAR_TYPE type;
} OSL_VAR;

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
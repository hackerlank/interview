typedef struct tagOSL_FUNCTION {
	string name;
	vector<LEXEM> params;
	vector<LEXEM> code;
	OSL_VAR_TYPE return_type;
} OSL_FUNCTION;


void osl_DeclareFunction(OSL_FUNCTION func, vector<OSL_FUNCTION>* pool);

void osl_UndeclareVar(string name, vector<OSL_FUNCTION>* pool);

bool osl_VarDeclared(string name, vector<OSL_FUNCTION>* pool);




void osl_DeclareFunction(OSL_FUNCTION func, vector<OSL_FUNCTION>* pool);
void osl_UndeclareFunction(string name, vector<OSL_FUNCTION>* pool);
bool osl_FunctionDeclared(string name, vector<OSL_FUNCTION>* pool);
OSL_FUNCTION* osl_GetFunction(string name, vector<OSL_FUNCTION>* pool);
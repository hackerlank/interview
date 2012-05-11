const string S_PRIOR_3 = "%^";
const string S_PRIOR_2 = "*/";
const string S_PRIOR_1 = "+-";
const string S_PRIOR_0 = "(";

#define C_ACC 5

bool osl_EvaluateMath(vector<LEXEM>* math, OSL_VAR* var, vector<OSL_VAR>* pool);
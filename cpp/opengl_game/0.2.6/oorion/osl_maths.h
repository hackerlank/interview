const string S_PRIOR_3 = "%^";
const string S_PRIOR_2 = "*/";
const string S_PRIOR_1 = "+-";
const string S_PRIOR_0 = "(";

const string S_PRIOR[] = { "(", "+-", "*/", "%^", "@$" };
const unsigned int L_S_PRIOR = 5;

#define C_ACC 5

int osl_GetStackPrior(LEXEM* lx);
char osl_Unar(char c);

bool osl_EvaluateMath(vector<LEXEM>* math, OSL_VAR* var, vector<OSL_VAR>* pool);
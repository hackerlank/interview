const string S_PRIOR_3 = "%^";
const string S_PRIOR_2 = "*/";
const string S_PRIOR_1 = "+-";
const string S_PRIOR_0 = "(";

const string S_PRIOR[] = { "(", "+-", "*/", "%^", "@$" };
const unsigned int L_S_PRIOR = 5;


int osl_GetStackPrior(OSL_LEXEM* lx);
char osl_Unar(char c);

bool osl_EvaluateMath(vector<OSL_LEXEM>* math, OSL_VAR* var, vector<OSL_VAR>* pool);
void osl_Error(string err_code); \
void osl_Error(string err_code, string hint);
void osl_Warning(string warn_code);

void osl_DumpList(char* file_name, vector<string>* list);


const string S_DEPRECATED = "!@#$%^&*()\\/\"\'+=-~`";
const string S_MATHS = "+-*/%^";

const string W1001 = "Warning: redeclaration";
const string W1002 = "Warning: enclosed brackets aren't supported yet";
const string W1003 = "Warning: float to int conversion";
const string S1001 = "Syntax error: \" expected";
const string S1002 = "Syntax error: variable expected, but keyword found";
const string S1003 = "Syntax error: ']' expected";
const string S1004 = "Syntax error: ']' unexpected";
const string S1005 = "Syntax error: matching closing/opening bracket not found";
const string S1006 = "Syntax error: type modifier not found";
const string S1007 = "Syntax error: unexpected token";
const string S1008 = "Syntax error: constant expected";
const string P1001 = "Parse error: variable undeclared";
const string P1002 = "Parse error: incompatible types";
const string P1003 = "Parse error: incorrect input string for execution (osl_ExecuteMath)";
const string P1004 = "Parse error: unterminated input string for execution (osl_ExecuteMath), forgot ';' ?";
const string P1005 = "Parse error: constant or variable expected (osl_ExecuteMath)";
const string P1006 = "Parse error: digital constant or variable expected (osl_ExecuteMath)";
const string P1007 = "Parse error: string constant or variable expected (osl_ExecuteMath)";
const string P1008 = "Parse error: unknown error";
const string P1009 = "Parse error: digital constant or variable expected";
#define OSL_CONVERT_BUFFER_SIZE 16
#define OSL_CONVERT_DIGITS 4


void osl_Error(string err_code); \
void osl_Error(string err_code, string hint);
void osl_Warning(string warn_code);



string osl_IntToStr(int n);
int osl_StrToInt(string s);

string osl_FloatToStr(float n);
float osl_StrToFloat(string s);

char* osl_RemoveQuotesChr(char* str);
string osl_RemoveQuotesStr(string str);
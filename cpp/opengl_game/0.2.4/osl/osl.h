// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OSL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OSL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef OSL_EXPORTS
#define OSL_API __declspec(dllexport)
#else
#define OSL_API __declspec(dllimport)
#endif


#define OSL_FUNCTION_CALL 45135
#define OSL_EXIT_PROGRAM 45136
#define OSL_ERROR 45137

typedef struct _OSL_RETURN_DATA {
	short data;
	string sParam;
	int iParam1;
	int iParam2;
	int reserved1;
	int reserved2;
} OSL_RETURN_DATA;

OSL_API bool osl_Load_v_04(const char* file_name);

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

OSL_API bool osl_Load_v_04(const char* file_name);

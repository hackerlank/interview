#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "lua/lua.hpp"

namespace SCRIPT
{
	BOOL InitScript();
	void FreeScript();
	
	BOOL LoadFile(const char* filename);
	BOOL LoadString(const char* str);
	BOOL ExecChunk();

	int AddToRegistry();
	void RemoveFromRegistry(int r);
	void GetFromRegistry(int r);

	BOOL ExecFile(const char* filename);
	BOOL ExecString(const char* str);
	BOOL ExecChunkFromReg(int r);
	int LoadFileToReg(const char* filename);
	int LoadStringToReg(const char* str);


	void GetUIntFieldByName(lua_State* L, const char* table, const char* name, UINT* val);
	void GetUIntFieldByName(lua_State* L, const char* name, UINT* val);
	void GetIntFieldByName(lua_State* L, const char* name, int* val);
	void GetFloatFieldByName(lua_State* L, const char* name, float* val);
	void GetStringFieldByName(lua_State* L, const char* name, char** val);


	void StackDumpToLog(lua_State *L);

}

//#define SCRIPTLOG(x) LogToFile( DEFAULT_SCRIPT_LOG_NAME, string(x)+"\n" );


#endif // __SCRIPT_H_
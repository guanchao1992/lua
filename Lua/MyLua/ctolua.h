#ifndef __CToLua_H__
#define __CToLua_H__

#include <map>
#include <iostream>
#include <functional>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifdef DLL_SAMPLE_EXPORTS
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif


extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a,int b);

#define MaxValue_NUM	255

extern "C" DLL_SAMPLE_API void RegFunction(lua_State * L, const char * name, lua_CFunction func);
extern "C" DLL_SAMPLE_API void PrintLuaStack(lua_State * L, const char*sign);

#define RegLuaFunctionStart(L,name);	RegFunction(L,name,[](lua_State * L){
#define RegLuaFunctionEnd(); return 0;});


#endif // !__CToLua_H__

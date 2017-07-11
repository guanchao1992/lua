#ifndef __CToLua_H__
#define __CToLua_H__

#include <map>
#include <iostream>
#include <functional>
#include "lua_get.h"

#ifdef DLL_SAMPLE_EXPORTS
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif


extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a,int b);

#define MaxValue_NUM	255

void RegFunction(lua_State * L, const char * name, lua_CFunction func)
{
	if (!name) return;

	lua_pushstring(L, name);
	lua_pushcclosure(L, func, 0);
	lua_pushvalue(L, -1);
	lua_setglobal(L, name);
}

#define RegLuaFunctionStart(L,name);	RegFunction(L,name,[](lua_State * L){
#define RegLuaFunctionEnd(); return 0;});


#endif // !__CToLua_H__

#pragma once

#include <functional>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

template<class T1,class T2>
int lua_function(lua_State * L,T1 t1,T2 t2)
{
	const char* ret = lua_tostring(L, 1);
	return 0;
}


int funcA(lua_State * L, const char*str, int num)
{

}


/*
	²ÎÊý
	TFunc : int Func(lua_State * L,T1 t1,T2 t2)
*/
template<class TFunc,class T1,class T2>
void RegFunction(lua_State * L, const char * name,void* func, T1 t1,T2 t2)
{
	if (!name) return;
	typedef int (FuncType)(lua_State * L, T1 t1, T2 t2);

	std::function<int(lua_State * L, T1 t1, T2 t2)> funcA = (std::function<int(lua_State * L, T1 t1, T2 t2)>)func;

	lua_pushstring(L, name);
	lua_pushcclosure(L, std::bind(funcA, t1, t2), 0);

	lua_pushvalue(L, -1);
	lua_setglobal(L, name);
}

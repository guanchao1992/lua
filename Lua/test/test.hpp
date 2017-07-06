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

template<class FUNC>
FUNC getRegLua_CFunc(FUNC)
{
	return FUNC;
}

int funcA(lua_State * L)
{
	return 0;
}


#ifndef __CToLua_H__
#define __CToLua_H__

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

template<class T1>
void RegFunction(lua_State * L, const char * name, lua_CFunction func, T1 t1)
{
	if (!name) return;

	lua_pushstring(L, name);
	lua_pushcclosure(L, func, 0);

	lua_pushvalue(L, -1);
	lua_setglobal(L, name);
}


#endif

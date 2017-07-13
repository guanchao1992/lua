#ifndef __LUA_GET_H__
#define __LUA_GET_H__

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


template<typename T>
inline T lua_get(lua_State*L, int idx)
{
	return ;
}

template<>
inline const char*lua_get<const char*>(lua_State*L, int idx)
{
	if (lua_isstring(L,idx))
	{
		return lua_tostring(L, idx);
	}
	return nullptr;
}

template<>
inline int lua_get<int>(lua_State*L, int idx)
{
	if (lua_isinteger(L,idx))
	{
		return lua_tointeger(L,idx);
	}
	return 0;
}

template<>
inline unsigned int lua_get<unsigned int>(lua_State*L, int idx)
{
	if (lua_isinteger(L,idx))
	{
		return (unsigned int)lua_tointeger(L,idx);
	}
	return 0;
}

template<>
inline double lua_get<double>(lua_State*L, int idx)
{
	if (lua_isnumber(L,idx))
	{
		return lua_tonumber(L, idx);
	}
	return 0;
}

#endif

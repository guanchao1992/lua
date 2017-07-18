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

template<>
inline bool lua_get<bool>(lua_State*L, int idx)
{
	if (lua_isboolean(L, idx))
	{
		return lua_toboolean(L, idx);
	}
	return false;
}






template<typename T>
inline void lua_push(lua_State*L, T t)
{
}

template<>
inline void lua_push<const char*>(lua_State*L, const char* t)
{
	lua_pushstring(L, t);
}

template<>
inline void lua_push<int>(lua_State*L, int t)
{
	lua_pushinteger(L, t);
}

template<>
inline void lua_push<unsigned int>(lua_State*L,unsigned int t)
{
	lua_pushinteger(L, t);
}

template<>
inline void lua_push<double>(lua_State*L, double t)
{
	lua_pushnumber(L, t);
}

template<>
inline void lua_push<bool>(lua_State*L, bool t)
{
	lua_toboolean(L, t);
}

#endif

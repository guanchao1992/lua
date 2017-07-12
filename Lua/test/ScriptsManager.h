#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "Singleton.h"

//½Å±¾¹ÜÀí
class ScriptsManager
{
	friend lua_State*GetLuaState();;
	SingletonClase(ScriptsManager);
private:
	ScriptsManager();
public:
	~ScriptsManager();
public:
	void Init();
	lua_State*	getLuaState() { return m_luaState; }
private:
	lua_State*	m_luaState;
};

inline lua_State*GetLuaState() 
{
	return ScriptsManager::getInstance()->m_luaState;
}

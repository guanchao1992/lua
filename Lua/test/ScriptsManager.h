#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "Singleton.h"
#include <string>

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
	lua_State*		getLuaState() { return m_luaState; }
	void			setScriptsPath(const std::string& path);
	const std::string&	getScriptsPath() { return m_rootPath; }
	void			doFile(const std::string&file);
private:
	lua_State*	m_luaState;
	std::string		m_rootPath;
};

inline lua_State*GetLuaState() 
{
	return ScriptsManager::getInstance()->m_luaState;
}

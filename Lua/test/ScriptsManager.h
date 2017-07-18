#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "base\Singleton.h"
#include <string>
#include "..\MyTool\tool_log.h"
#include "base\lua_get.h"

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
	void				Init();
	lua_State*			getLuaState() { return m_luaState; }
	void				setScriptsPath(const std::string& path);
	const std::string&	getScriptsPath() { return m_rootPath; }
	void				doFile(const std::string&file);
private:
	lua_State*		m_luaState;
	std::string		m_rootPath;
};

inline lua_State*GetLuaState() 
{
	return ScriptsManager::getInstance()->m_luaState;
}

class ScriptsManagerWrapper : public ScriptsManager
{
public:
	int setScriptsPath(lua_State*L)
	{
		std::string path = luaL_checkstring(L, -1);
		ScriptsManager::setScriptsPath(path);
		return 0;
	}
	int getScriptsPath(lua_State*L)
	{
		lua_pushstring(L, ScriptsManager::getScriptsPath().c_str());
		return 1;
	}
};


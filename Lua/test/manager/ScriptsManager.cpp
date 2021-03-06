#include "ScriptsManager.h"
#include "..\..\MyTool\tool_log.h"
#include "..\config.h"
#include "..\..\MyLua\ctolua.h"

SingletonClaseCpp(ScriptsManager);


ScriptsManager::ScriptsManager()
{
}


ScriptsManager::~ScriptsManager()
{
}

void ScriptsManager::Init()
{
	lua_State *L = luaL_newstate();
	m_luaState = L;
	luaL_openlibs(L);
	lua_checkstack(L, 10);

	std::string path = getAccuratePath("scripts\\");
	setScriptsPath(path);

}

void ScriptsManager::setScriptsPath(const std::string& path)
{
	m_rootPath = path;
}

void ScriptsManager::doFile(const std::string&file)
{
	std::string filePath = m_rootPath + file;
	int error = luaL_dofile(getLuaState(),filePath.c_str());
	if (error != 0)
	{		
		LOG_D_F("ScriptsManager","dofile '%s' error.code %d. error:\n%s",filePath.c_str(),error, lua_tostring(GetLuaState(), -1));
	}
}

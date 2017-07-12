#include "ScriptsManager.h"
#include "../MyTool/tool_log.h"

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

	luaL_dofile(L, "init.lua");
}

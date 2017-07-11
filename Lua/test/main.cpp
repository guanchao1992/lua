#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <iostream>
using namespace std;
#include "../MyLua/ctolua.h"

#include "../MyTool/tool.h"
int main()
{
	initLog();
	LOG_D("========== = 12332131dvfsdivgfj");
	LOG_F_T("========== = 12332131dvfsdivgfj");
	//LogLog::getLogLog()->debug(LOG4CPLUS_TEXT("This is a Debug statement..."));
	auto logger = Logger::getInstance(LOG4CPLUS_TEXT("log"));
	LOG4CPLUS_DEBUG(logger, LOG4CPLUS_C_STR_TO_TSTRING("This is a Debug statement..."));

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lua_checkstack(L, 10);

	RegLuaFunctionStart(L, "testF");
	cout << "--------------" << lua_get<int>(L, 1) << endl;
	RegLuaFunctionEnd();
	
	luaL_dostring(L, "testF(1231231)");

	//lua_call();
	
	lua_close(L);
	system("pause");
	return 0;
}
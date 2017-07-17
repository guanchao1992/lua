#include <stdio.h>
#include <stdlib.h>

#include <iostream>
using namespace std;
#include "../MyLua/ctolua.h"
#include "../MyTool/tool_log.h"

#include "ScriptsManager.h"
#include "test.h"
#include "ClassToLua.h"

int test_regFunc()
{
	LOG_D_F("main","register ctolua function <testF>");
	lua_State*L = GetLuaState();
	RegLuaFunctionStart(L, "testF");
	cout << "--------------" << lua_get<int>(GetLuaState(), 1) << endl;
	RegLuaFunctionEnd();
	luaL_dostring(GetLuaState(), "testF(1231231)");
	return 1;
}

void Init()
{
	log_init();
	ScriptsManager::getInstance()->Init();
}

int main()
{
	Init();

//	register_all_moonton(GetLuaState());
//	RegisterClass(GetLuaState());
	regAllClass();
	ScriptsManager::getInstance()->doFile("init.lua");

	LOG_D_F("main","ÖÐÎÄ.main start and init log sure.");

	test_regFunc();

	lua_close(GetLuaState());
	log_close();
	system("pause");
	return 0;
}

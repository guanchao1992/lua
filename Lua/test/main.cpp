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

int test_regFunc(lua_State * L)
{
	LOG_D_F(_T("register ctolua function <testF>"));
	RegLuaFunctionStart(L, "testF");
	cout << "--------------" << lua_get<int>(L, 1) << endl;
	RegLuaFunctionEnd();

	luaL_dostring(L, "testF(1231231)");
	return 1;
}

void initLua(lua_State*L)
{

}

int main()
{
	log_init();
	LOG_D_F(_T("ÖÐÎÄ.main start and init log sure."));

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lua_checkstack(L, 10);

	test_regFunc(L);

	lua_close(L);
	log_close();
	system("pause");
	return 0;
}

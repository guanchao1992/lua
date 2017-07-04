#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


int main()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	luaL_Reg lr;
	lr.name = "testfunc";
	lr.func = [](lua_State*L)->int {
		float ret = lua_tonumber(L,1);
		printf("----------------- func \n %f" ,ret);
		return 0;
	};
	luaL_requiref(L, lr.name, lr.func, 1);
	lua_pop(L, 1);  /* remove lib */

	//lua_RegisterCFunc(L, "testfunc",func);

	luaL_dostring(L, "testfunc(10)");
	lua_close(L);
	system("pause");
	return 0;
}
#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "../MyLua/ToLua.h"

#include "test.hpp"
#include <iostream>


/*
void RegFunction(lua_State*L, const char*name, lua_CFunction func)
{
	if (!name) return;
	lua_pushstring(L, name);
	lua_pushcclosure(L, func, 0);
}
*/


 int main()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	lua_checkstack(L, 10);

	luaL_Reg lr;
	lr.name = "testfunc";
	lr.func = [](lua_State*L)->int {
		const char* ret = lua_tostring(L,1);
		printf("----------------- func  %s\n" ,ret);
		return 0;
	};
	lua_pushstring(L, lr.name);
	lua_pushcclosure(L, lr.func,0);
	lua_pushvalue(L, -1);  /* copy of module */
	lua_setglobal(L, lr.name);  /* _G[modname] = module */
	lua_pop(L, 1);

	//luaL_requiref(L, lr.name, lr.func, false);
	//lua_pop(L, 1);  /* remove lib */

	//lua_RegisterCFunc(L, "testfunc",func);

	//RegFunction(L, "testfunc2", [](lua_State * L, const char*str, int num) {}, "123121",123321);

	const char* str = "testfunc2";
	RegFunction(L, "testfunc2", (void*) [](lua_State * L,const char*str,int num){
		std::cout << "string£º" << str << std::endl;
		std::cout << "num£º" << num << std::endl;
		return 0; }, str, 123321);
	luaL_dostring(L, "testfunc2('stringstring',100)");

	CTOLuaAdd(1,2);
	lua_close(L);
	system("pause");
	return 0;
}
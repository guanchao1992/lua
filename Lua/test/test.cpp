#include "test.h"
#include "..\MyLua\lua_get.h"
#include <string.h>
#include "..\MyLua\ctolua.h"

#define TOLUA_VALUE_ROOT "root"

typedef struct
{
	const char*		name;
	lua_CFunction	method;
} LUA_METHOD;


LUA_METHOD lua_Foo_functions[] = {
	{ "add",
	[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, -1, "Foo");
		lua_remove(L, -1);
		int num = (*obj)->add(lua_get<int>(L, 1), lua_get<int>(L, 2));
		lua_pushnumber(L, num);
		return 1; } },
	{ "setV",
		[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, -1, "Foo");
		lua_remove(L, -1);
		(*obj)->setV(lua_get<int>(L, 1));
		return 1; } },
	{ "getV",
		[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, -1, "Foo");
		lua_remove(L, -1);
		(*obj)->getV();
		return 1; } },
};

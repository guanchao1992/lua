#include "ctolua.h"
#include <string.h>
#include "lua_get.h"

extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a, int b)
{
	return a + b;
}

extern "C" DLL_SAMPLE_API void RegFunction(lua_State * L, const char * name, lua_CFunction func)
{
	if (!name) return;

	lua_pushstring(L, name);
	lua_pushcclosure(L, func, 0);
	lua_pushvalue(L, -1);
	lua_setglobal(L, name);
	lua_pop(L,-1);
}

extern "C" DLL_SAMPLE_API void PrintLuaStack(lua_State * L, const char*sign)
{
	int stackTop = lua_gettop(L);//获取栈顶的索引值  
	int nIdx = 0;
	int nType;

	//printf("--输出lua栈信息：%s element count: %d --\n", sign, stackTop);
	printf("--栈顶<%s>(v)(%d)--\n", sign, stackTop);
	//显示栈中的元素  
	for (nIdx = stackTop; nIdx > 0; --nIdx)
	{
		nType = lua_type(L, nIdx);
		printf("(i:%d) %s(%s)\n", nIdx, lua_typename(L, nType), lua_tostring(L, nIdx));
	}
	printf("--栈底--\n");
}

void Lua_get_class_registry(lua_State*L)
{
	//如果键"SWIG"下存的不是table，就把它变成table
	lua_pushstring(L, "SWIG");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (!lua_istable(L,-1))
	{
		lua_pop(L, 1);
		lua_pushstring(L, "SWIG");
		lua_newtable(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		lua_pushstring(L, "SWIG");
		lua_rawget(L, LUA_REGISTRYINDEX);
	}
}

void Lua_get_class_metatable(lua_State*L, const char*cname)
{
	Lua_get_class_registry(L);
	lua_pushstring(L, cname);
	lua_rawget(L, -2);
	lua_remove(L, -2);
}

void Lua_test(lua_State*L)
{
}
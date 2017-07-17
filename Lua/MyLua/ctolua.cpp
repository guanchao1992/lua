#include "ctolua.h"
#include <string.h>
#include "lua_get.h"
#include <assert.h>

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
	lua_pop(L, -1);
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


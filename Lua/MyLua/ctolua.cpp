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
	int stackTop = lua_gettop(L);//��ȡջ��������ֵ  
	int nIdx = 0;
	int nType;

	//printf("--���luaջ��Ϣ��%s element count: %d --\n", sign, stackTop);
	printf("--ջ��<%s>(v)(%d)--\n", sign, stackTop);
	//��ʾջ�е�Ԫ��  
	for (nIdx = stackTop; nIdx > 0; --nIdx)
	{
		nType = lua_type(L, nIdx);
		printf("(i:%d) %s(%s)\n", nIdx, lua_typename(L, nType), lua_tostring(L, nIdx));
	}
	printf("--ջ��--\n");
}


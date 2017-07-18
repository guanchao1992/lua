#include "ClassToLua.h"
#include "ScriptsManager.h"
#include "base\lua_get.h"

template<class T>
const char*getClassName()
{
	static char name[256];
	strcpy_s(name, 255, typeid(T).name());
	replace(name, name + strlen(name),' ','_');
	return name;
}

template<class T>
static int ctolua_constructor(lua_State*L)
{
	int value = 0;
	if (!lua_isnumber(L, -1))
	{
		lua_pop(L, -1);
		assert(1);
		return 0;
	}
	value = lua_tonumber(L, -1);
	lua_pop(L, -1);
	T*obj = new T(value);
	T**a = (T**)lua_newuserdata(L, sizeof(T*));
	*a = obj;
	int i = luaL_getmetatable(L, getClassName<T>());
	lua_setmetatable(L, -2);
	return 1;
}

template<class T>
int ctolua_gc(lua_State*L)
{
	T** obj = (T**)luaL_checkudata(L, -1, getClassName<T>());
	delete (*obj);
	return 0;
}

/*	下列宏解析后会变成这个lambda表达式
[](lua_State*L) {
lua_gettable(L, 1);
Foo** obj = (Foo**)luaL_checkudata(L, 1, "class_Foo");
lua_remove(L, -1);
int num = (*obj)->add(lua_get<int>(L, 1), lua_get<int>(L, 2));
lua_pushnumber(L,num);
return 1; }
*/
#define CToLuaFunc_Start(ClassType)	[](lua_State*L){ClassType**obj = (ClassType**)luaL_checkudata(L,1,"class_"#ClassType)
#define CToLuaFunc_End return 1;}
#define CToLuaFunc_End_R(ReturnType,RET) lua_push<ReturnType>(L,RET); return 1;}
#define CToLuaFunc_Get(PARAM,NO)	PARAM p##NO = lua_get<PARAM>(L,NO+1)
#define CToLuaFunc_Run_R(ReturnType,Func,...)		ReturnType ret = (*obj)->Func(__VA_ARGS__)
#define CToLuaFunc_Run(Func,...)			(*obj)->Func(__VA_ARGS__)

#define CToLuaFunc_0_r(ClassType,Func,ReturnType)\
CToLuaFunc_Start(ClassType);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_1_r(ClassType,Func,ReturnType,P1)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_2_r(ClassType,Func,ReturnType,P1,P2)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_3_r(ClassType,Func,ReturnType,P1,P2,P3)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_4_r(ClassType,Func,ReturnType,P1,P2,P3,P4)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_5_r(ClassType,Func,ReturnType,P1,P2,P3,P4,P5)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4,p5);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_6_r(ClassType,Func,ReturnType,P1,P2,P3,P4,P5,P6)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4,p5,p6);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_7_r(ClassType,Func,ReturnType,P1,P2,P3,P4,P5,P6,P7)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4,p5,p6,p7);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_8_r(ClassType,Func,ReturnType,P1,P2,P3,P4,P5,P6,P7,P8)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);CToLuaFunc_Get(P8,8);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4,p5,p6,p7,p8);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_9_r(ClassType,Func,ReturnType,P1,P2,P3,P4,P5,P6,P7,P8,P9)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);CToLuaFunc_Get(P8,8);CToLuaFunc_Get(P9,9);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4,p5,p6,p7,p8,p9);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_10_r(ClassType,Func,ReturnType,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);CToLuaFunc_Get(P8,8);CToLuaFunc_Get(P9,9);CToLuaFunc_Get(P10,10);\
lua_pop(L,-1);\
CToLuaFunc_Run_R(ReturnType,Func,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);\
CToLuaFunc_End_R(ReturnType,ret)

#define CToLuaFunc_0(ClassType,Func)	\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Run(Func);\
lua_pop(L,-1);\
CToLuaFunc_End

#define CToLuaFunc_1(ClassType,Func,P1)	\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);\
CToLuaFunc_Run(Func,p1);\
lua_pop(L,-1);\
CToLuaFunc_End

#define CToLuaFunc_2(ClassType,Func,P1,P2)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2);\
CToLuaFunc_End

#define CToLuaFunc_3(ClassType,Func,P1,P2,P3)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3);\
CToLuaFunc_End

#define CToLuaFunc_4(ClassType,Func,P1,P2,P3,P4)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4);\
CToLuaFunc_End

#define CToLuaFunc_5(ClassType,Func,P1,P2,P3,P4,P5)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4,p5);\
CToLuaFunc_End

#define CToLuaFunc_6(ClassType,Func,P1,P2,P3,P4,P5,P6)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4,p5,p6);\
CToLuaFunc_End

#define CToLuaFunc_7(ClassType,Func,P1,P2,P3,P4,P5,P6,P7)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4,p5,p6,p7);\
CToLuaFunc_End

#define CToLuaFunc_8(ClassType,Func,P1,P2,P3,P4,P5,P6,P7,P8)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);CToLuaFunc_Get(P8,8);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4,p5,p6,p7,p8);\
CToLuaFunc_End

#define CToLuaFunc_9(ClassType,Func,P1,P2,P3,P4,P5,P6,P7,P8,P9)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);CToLuaFunc_Get(P8,8);CToLuaFunc_Get(P9,9);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4,p5,p6,p7,p8,p9);\
CToLuaFunc_End

#define CToLuaFunc_10(ClassType,Func,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)\
CToLuaFunc_Start(ClassType);\
CToLuaFunc_Get(P1,1);CToLuaFunc_Get(P2,2);CToLuaFunc_Get(P3,3);CToLuaFunc_Get(P4,4);CToLuaFunc_Get(P5,5);CToLuaFunc_Get(P6,6);CToLuaFunc_Get(P7,7);CToLuaFunc_Get(P8,8);CToLuaFunc_Get(P9,9);CToLuaFunc_Get(P10,10);\
lua_pop(L,-1);\
CToLuaFunc_Run(Func,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);\
CToLuaFunc_End


void ctolua_registerclass_func(lua_State*L, const MapClassFunc& mapFunc)
{
	for(auto it: mapFunc)
	{
		// 注册所有方法
		lua_pushstring(L, it.first);
		lua_pushcfunction(L, it.second);
		lua_settable(L, -3);
	}
}

//不支持异步调用
template<class T>
void ctolua_registerclass(lua_State* L, const MapClassFunc& mapFunc)
{
	lua_pushcfunction(L, ctolua_constructor<T>);
	lua_setglobal(L, "newFoo");

	luaL_newmetatable(L, getClassName<T>());	//class_Foo

	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, ctolua_gc<T>);
	lua_settable(L, -3);

	lua_pushstring(L, "__index");
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_insert(L, 1);
	lua_settable(L, -3);
	lua_pop(L, 1);

	ctolua_registerclass_func(L, mapFunc);

	lua_pop(L, -1);
}

MapClassFunc s_Foo_luafuncs;
void register_lua_Foo_func()
{
	s_Foo_luafuncs["add"] = CToLuaFunc_2_r(Foo, add, int, int, int);
	s_Foo_luafuncs["setV"] = CToLuaFunc_1(Foo, setV, int);
	s_Foo_luafuncs["getV"] = CToLuaFunc_0_r(Foo, getV, int);
}

void regAllClass()
{
	register_lua_Foo_func();
	ctolua_registerclass<Foo>(GetLuaState(), s_Foo_luafuncs);
}
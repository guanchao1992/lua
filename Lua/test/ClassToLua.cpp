#include "ClassToLua.h"
#include "ScriptsManager.h"
#include "lua_get.h"

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

#define CToLuaFunc_Start(ClassType)	[](lua_State*L){ClassType**obj = (ClassType**)luaL_checkudata(L,1,"class_"#ClassType);
#define CToLuaFunc_End return 1;}

#define CToLuaFunc_0_r(ClassType,Func,ReturnType)	\
CToLuaFunc_Start(ClassType)	\
lua_pop(L,-1);\
ReturnType ret = (*obj)->Func();\
lua_push<ReturnType>(L,ret);\
CToLuaFunc_End

#define CToLuaFunc_1_r(ClassType,Func,ReturnType,P1)	\
CToLuaFunc_Start(ClassType)	\
P1 p1 = lua_get<P1>(L,2);\
lua_pop(L,-1);\
ReturnType ret = (*obj)->Func(p1);\
lua_push<ReturnType>(L,ret);\
CToLuaFunc_End

#define CToLuaFunc_2_r(ClassType,Func,ReturnType,P1,P2)	\
CToLuaFunc_Start(ClassType)	\
P1 p1 = lua_get<P1>(L,2);\
P2 p2 = lua_get<P2>(L,2);\
lua_pop(L,-1);\
ReturnType ret = (*obj)->Func(p1,p2);\
lua_push<ReturnType>(L,ret);\
CToLuaFunc_End

#define CToLuaFunc_1(ClassType,Func,P1)	\
CToLuaFunc_Start(ClassType)	\
P1 p1 = lua_get<P1>(L,2);\
(*obj)->Func(p1);\
lua_pop(L,-1);\
CToLuaFunc_End

typedef std::map<const char*, lua_CFunction> MapClassFunc;
MapClassFunc s_Foo_luafuncs;
void register_lua_Foo_func()
{
	s_Foo_luafuncs["add"] = CToLuaFunc_2_r(Foo, add, int, int,int);
	s_Foo_luafuncs["setV"] = CToLuaFunc_1(Foo, setV,int);
	s_Foo_luafuncs["getV"] = CToLuaFunc_0_r(Foo, getV,int);
}

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

void regAllClass()
{
	
	register_lua_Foo_func();
	ctolua_registerclass<Foo>(GetLuaState(), s_Foo_luafuncs);
}
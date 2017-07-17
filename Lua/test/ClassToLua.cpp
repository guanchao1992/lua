#include "ClassToLua.h"
#include "ScriptsManager.h"

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
	const char* name = getClassName<T>();
	int i = luaL_getmetatable(L, name);
	lua_setmetatable(L, -2);
	return 1;
}

template<class T>
int ctolua_gc(lua_State*L)
{
	const char* name = getClassName<T>();
	T** obj = (T**)luaL_checkudata(L, -1, name);
	delete (*obj);
	return 0;
}

typedef struct
{
	const char*		name;
	lua_CFunction	method;
} LUA_METHOD;

LUA_METHOD lua_Foo_functions[] = {
	{ 
		"add",
		[](lua_State*L) {
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, 1, "class_Foo");
		lua_remove(L, -1);
		int num = (*obj)->add(lua_get<int>(L, 1), lua_get<int>(L, 2));
		lua_pushnumber(L,num);
		return 1; }
	},
	{
		"setV",
		[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, 1, "class_Foo");
		lua_remove(L, -1);
		(*obj)->setV(lua_get<int>(L, 1));
		return 1; }
	},
	{ 
		"getV",
		[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, 1, "class_Foo");
		lua_remove(L, -1);
		(*obj)->getV();
		return 1; }
	},
};


template<class T>
void ctolua_registerclass(lua_State* L)
{
	lua_pushcfunction(L, ctolua_constructor<T>);
	lua_setglobal(L, "newFoo");

	const char* name = getClassName<T>();
	luaL_newmetatable(L, name);	//class_Foo

	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, ctolua_gc<T>);
	lua_settable(L, -3);

	lua_pushstring(L, "__index");
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_insert(L, 1);
	lua_settable(L, -3);
	lua_pop(L, 1);

	unsigned num = sizeof(lua_Foo_functions) / sizeof(*lua_Foo_functions);
	for (int i = 0; i < num; ++i)
	{
		// 注册所有方法
		lua_pushstring(L, lua_Foo_functions[i].name);
		lua_pushcfunction(L, lua_Foo_functions[i].method);
		lua_settable(L, -3);
	}
	lua_pop(L, -1);
}

void regAllClass()
{
	ctolua_registerclass<Foo>(GetLuaState());
}
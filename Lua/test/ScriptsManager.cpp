#include "ScriptsManager.h"
#include "../MyTool/tool_log.h"
#include "config.h"
#include "../MyLua/ctolua.h"

SingletonClaseCpp(ScriptsManager);

static void RegisterClass(lua_State* L);

ScriptsManager::ScriptsManager()
{
}


ScriptsManager::~ScriptsManager()
{
}

void ScriptsManager::Init()
{
	lua_State *L = luaL_newstate();
	m_luaState = L;
	luaL_openlibs(L);
	lua_checkstack(L, 10);

	std::string path = Data_Path;
	path.append("\\scripts\\");
	setScriptsPath(path);

	RegisterClass(L);
	doFile("init.lua");

}

void ScriptsManager::setScriptsPath(const std::string& path)
{
	m_rootPath = path;
}

void ScriptsManager::doFile(const std::string&file)
{
	std::string filePath = m_rootPath + file;
	int error = luaL_dofile(getLuaState(),filePath.c_str());
	if (error != 0)
	{
		LOG_D_F("ScriptsManager","dofile '%s' error.errno %d.",filePath.c_str(),error);
	}
}

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
		int num =(*obj)->add(lua_get<int>(L, 1), lua_get<int>(L, 2));
		lua_pushnumber(L,num);
		return 1; }},
	{ "setV",
	[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, -1, "Foo");
		lua_remove(L, -1);
		(*obj)->setV(lua_get<int>(L, 1));
		return 1; }},
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

static int gc_obj(lua_State* L)
{
	Foo** obj = (Foo**)luaL_checkudata(L, -1, "Foo");
	delete (*obj);
	return 0;
}

static int constructor(lua_State*L)
{
	int value = 0;
	if (!lua_isnumber(L, -1))
	{
		lua_pop(L, -1);
		LOG_E_F("lua_reg", "error in Lua_reg_Foo.");
		return 0;
	}
	value = lua_tonumber(L, -1);
	lua_pop(L, -1);
	Foo*obj = new Foo(value);
	// 2. 新建一个表 tt = {}
	lua_newtable(L);
	Foo**a = (Foo**)lua_newuserdata(L, sizeof(Foo*));
	*a = obj;
	//4
	luaL_getmetatable(L, "Foo");
	lua_pushvalue(L, -1);
	lua_setmetatable(L, -3);
	// 5. tt[0] = userdata
	lua_insert(L, -2);
	lua_pushnumber(L, 0);
	lua_insert(L, -2);
	lua_settable(L, -4);
	//6
	lua_pushstring(L, "method");
	lua_gettable(L, -2);
	lua_setmetatable(L, -3);
	lua_pop(L, 1);
	PrintLuaStack(L, "end");
	return 1;
}

static void RegisterClass(lua_State* L)
{
	lua_pushcfunction(L, constructor);
	lua_setglobal(L, "Foo");
	PrintLuaStack(L, "1");

	luaL_newmetatable(L, "Foo");
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, &gc_obj);
	lua_settable(L, -3);
	// 创建一个方法元表
	PrintLuaStack(L, "2");
	lua_newtable(L);
	// 指定__index方法
	int meta = lua_gettop(L);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, meta);
	PrintLuaStack(L, "3");
	lua_settable(L, -3);

	unsigned num = sizeof(lua_Foo_functions) / sizeof(*lua_Foo_functions);
	for (int i = 0; i < num; ++i)
	{
		// 注册所有方法
		lua_pushstring(L, lua_Foo_functions[i].name);
		lua_pushcclosure(L, lua_Foo_functions[i].method, 0);
		lua_settable(L, -3);

		/*
		lua_pushstring(L, name);
		lua_pushcclosure(L, func, 0);
		lua_pushvalue(L, -1);
		lua_setglobal(L, name);
*/
	}

	PrintLuaStack(L, "000000");
	// 把这个表放入元表以便后用, 起名为methods
	lua_pushstring(L, "methods");
	lua_insert(L, -2);
	lua_settable(L, -3);
}
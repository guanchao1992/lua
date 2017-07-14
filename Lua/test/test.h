#pragma once

#include <functional>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class Foo
{
public:
	Foo(int value)
	{
		_value = value;
		printf("Foo Constructor!\n");
	}
	~Foo()
	{
		printf("Foo Destructor!\n");
	}
	int add(int a, int b)
	{
		printf("Foo add!\n");
		return  a + b;
	}
	void setV(int value)
	{
		printf("Foo setV!\n");
		_value = value;
	}
	int getV()
	{
		printf("Foo getV!\n");
		return _value;
	}
	int _value;
};


int register_all_moonton(lua_State* tolua_S);
void tolua_open(lua_State* L);
void tolua_module(lua_State* L, const char* name, int hasvar);
void tolua_beginmodule(lua_State* L, const char* name);
void tolua_endmodule(lua_State*L);
void tolua_function(lua_State* L, const char* name, lua_CFunction func);
int lua_register_moonton_Foo(lua_State* tolua_S);
void tolua_usertype(lua_State* L, const char* type);
static int tolua_newmetatable(lua_State* L, const char* name);
static void mapsuper(lua_State* L, const char* name, const char* base);
void tolua_cclass(lua_State* L, const char* lname, const char* name, const char* base, lua_CFunction col);

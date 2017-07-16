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
int lua_register_moonton_Foo(lua_State* tolua_S);
static void mapsuper(lua_State* L, const char* name, const char* base);

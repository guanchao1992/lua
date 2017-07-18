#pragma once
#include "..\MyLua\ctolua.h"
#include "..\MyTool\tool_log.h"
#include "lua_get.h"

class Foo
{
public:
	Foo(int value)
	{
		_value = value;
		printf("Foo Constructor! value=%d\n", _value);
	}
	~Foo()
	{
		printf("Foo Destructor! value=%d\n", _value);
	}
	int add(int a, int b)
	{
		printf("Foo add! value=%d\n", _value);
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

void regAllClass();

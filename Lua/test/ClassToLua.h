#pragma once
#include "..\MyLua\ctolua.h"
#include "..\MyLua\lua_get.h"
#include "..\MyTool\tool_log.h"

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
		return _value;
	}
	int _value;
};

void regAllClass();

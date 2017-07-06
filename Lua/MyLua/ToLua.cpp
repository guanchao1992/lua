#include "ToLua.hpp"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <string.h>

LuaValueManager* LuaValueManager::g_luaValueManager = new LuaValueManager();

extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a, int b)
{
	return a + b;
}

LuaValue::LuaValue(int value)
{
	_type = ValueType_int;
	_value = new int(value);
}

LuaValue::LuaValue(unsigned int value)
{
	_type = ValueType_unsigned_int;
	_value = new unsigned int(value);
}

LuaValue::LuaValue(double value)
{
	_type = ValueType_double;
	_value = new double(value);
}

LuaValue::LuaValue(const char * value)
{
	_type = ValueType_string;
	size_t len = strlen(value);
	if (len > MaxValue_NUM)
	{
		len = MaxValue_NUM;
	}
	_value = new char[len+1];
	strcpy_s((char*)_value,len+1,value);
}

LuaValue::~LuaValue()
{
	delete _value;
}

LuaValueManager * LuaValueManager::getIinstance()
{
	static LuaValueManager* g_luaValuemanager = nullptr;
	if (g_luaValuemanager == nullptr)
	{
		g_luaValueManager = new LuaValueManager();
	}
	return g_luaValueManager;
}

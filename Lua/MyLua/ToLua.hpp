#ifndef __ToLua_H__
#define __ToLua_H__

#include "CToLua.hpp"
#include <map>

#ifdef DLL_SAMPLE_EXPORTS
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif


extern "C" DLL_SAMPLE_API int _stdcall CTOLuaAdd(int a,int b);

#define MaxValue_NUM	255

enum ValueType
{
	ValueType_int = 0,
	ValueType_unsigned_int,
	ValueType_double,
	ValueType_string,
};

struct LuaValue 
{
public:
	LuaValue(int value);
	LuaValue(unsigned int value);
	LuaValue(double value);
	LuaValue(const char*value);
	~LuaValue();
public:
	ValueType	_type;
	void*		_value;
};

//用于管理所有的lua变量类型
class LuaValueManager
{
private:
	LuaValueManager() {};
public:
	LuaValueManager*getIinstance();
	std::map<void*, LuaValue>	m_allValue;
};


template<class T1>
void Lua_RegFunc(lua_State*L,const char*name,T1 t1)
{
	int index = 1000;
	auto func = [=](lua_State*L) {
		index;
		return 0;
	};
	RegFunction(L, name, func, t1);
}


#endif // !__CToLua_H__

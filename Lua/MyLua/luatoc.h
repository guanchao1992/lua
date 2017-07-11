#ifndef __LuaToC_H__
#define __LuaToC_H__


class LuaFunctor
{
public:
	LuaFunctor(const char*func);
	~LuaFunctor();
private:


	template<typename T>
	T call_return_r(lua_State*L,int argc)
	{
		int err = lua_pcall(L,argc,1,0);
		if (err)
		{
			const char*errstr = lua_tostring(L, -1);
			lua_pop(L, 1); 

		}
	}
};


#endif

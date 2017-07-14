#include "test.h"
#include "..\MyLua\lua_get.h"
#include <string.h>
#include "..\MyLua\ctolua.h"

#define TOLUA_VALUE_ROOT "root"

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
		int num = (*obj)->add(lua_get<int>(L, 1), lua_get<int>(L, 2));
		lua_pushnumber(L, num);
		return 1; } },
	{ "setV",
		[](lua_State*L) {
		int i = (int)lua_tonumber(L, lua_upvalueindex(1));
		lua_pushnumber(L, 0);
		lua_gettable(L, 1);
		Foo** obj = (Foo**)luaL_checkudata(L, -1, "Foo");
		lua_remove(L, -1);
		(*obj)->setV(lua_get<int>(L, 1));
		return 1; } },
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

int class_gc_event(lua_State*L)
{
	return 1;
}

int register_all_moonton(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, nullptr, 0);
	tolua_beginmodule(tolua_S, nullptr);

	lua_register_moonton_Foo(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

void tolua_open(lua_State* L)
{
	int top = lua_gettop(L);//获取栈顶编号（因为index从1开始，所以变相等于获取栈上面一共有多少元素）
	lua_pushstring(L, "tolua_opened");//将“tolua_opened”字符串压入栈，此时栈结构：stack：“tolua_opened”
	lua_rawget(L, LUA_REGISTRYINDEX);//获取名为LUA_REGISTRYINDEX的表t，然后取栈顶元素k，弹出栈顶元素k，然后将值t[k]压入栈顶，此时栈结构：stack：value
	if (!lua_isboolean(L, -1))//这里是重复初始化，以后会把tolua_opened置为true，这里-1指的是栈顶，关于栈的正负索引，下面给图，这里判断栈顶是否为bool值
	{
		lua_pushstring(L, "tolua_opened");//将“tolua_opened”压入栈，此时栈结构：stack：value “tolua_opened”
		lua_pushboolean(L, 1);//将1当做一个bool值压入栈，此时栈结构：value “tolua_opened” 1
		lua_rawset(L, LUA_REGISTRYINDEX);//获取lua注册表t，栈顶元素作为value，栈顶后面一个作为key，t[key]=value,弹出value和key，此时栈结构：stack：value

										 // create value root table
		lua_pushstring(L, TOLUA_VALUE_ROOT);//string压入栈，栈结构：stack：value TOLUA_VALUE_ROOT
		lua_newtable(L);//创建新表，并压入栈，栈结构：value TOLUA_VALUE_ROOT table
		lua_rawset(L, LUA_REGISTRYINDEX);//获取lua注册表t，栈中的table作为value，TOLUA_VALUE_ROOT作为key，实际上等于t[TOLUA_VALUE_ROOT] = table，分别弹出table，TOLUA_VALUE_ROOT，栈结构：stack：value

#ifndef LUA_VERSION_NUM /* only prior to lua 5.1 */
										 /* create peer object table */
		lua_pushstring(L, "tolua_peers");//字符串压入栈，栈结构：stack：value "tolua_peers"
		lua_newtable(L);//创建一个新表并压入栈，栈结构：stack：value "tolua_peers" table
						/* make weak key metatable for peers indexed by userdata object */
		lua_newtable(L);//创建一个新表并压入栈，栈结构：stack：value "tolua_peers" table table1
		lua_pushliteral(L, "__mode");//压入一个字符串，栈结构：stack：value "tolua_peers" table table1 "__mode"
		lua_pushliteral(L, "k");//压入一个字符串，栈结构：stack：value "tolua_peers" table table1 "__mode" "k"
		lua_rawset(L, -3);//分别取栈顶元素作为value，下一个作为key，第三个作为t，t[key]=value,并弹出栈顶和第二个元素，(这里实际上是实现table的弱引用)当前栈结构：stack：value "tolua_peers" table table1
		lua_setmetatable(L, -2);//将栈顶table1作为第二个元素table的元表，弹出table1，栈结构：stack：value "tolua_peers" table
		lua_rawset(L, LUA_REGISTRYINDEX);//栈顶元素作为value，第二个元素作为key，lua注册表作为t，t[key]=value,分别弹出栈顶元素、第二个元素，栈结构：stack：value
#endif
										 //这里与上面类似，就不在阐述
										 /* create object ptr -> udata mapping table */
		lua_pushstring(L, "tolua_ubox");
		lua_newtable(L);
		/* make weak value metatable for ubox table to allow userdata to be
		garbage-collected */
		lua_newtable(L);
		lua_pushliteral(L, "__mode");
		lua_pushliteral(L, "v");
		lua_rawset(L, -3);               /* stack: string ubox mt */
		lua_setmetatable(L, -2);  /* stack: string ubox */
		lua_rawset(L, LUA_REGISTRYINDEX);

		//        /* create object ptr -> class type mapping table */
		//        lua_pushstring(L, "tolua_ptr2type");
		//        lua_newtable(L);
		//        lua_rawset(L, LUA_REGISTRYINDEX);

		//这里实际上创建了名为"tolua_super"和"tolua_gc"两张表，并且保存在lua注册表里
		lua_pushstring(L, "tolua_super");
		lua_newtable(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		lua_pushstring(L, "tolua_gc");
		lua_newtable(L);
		lua_rawset(L, LUA_REGISTRYINDEX);

		//垃圾回收
		//这里用到了lua_pushcclosure，其实就是关联了一些变量的c函数，与lua_pushcfunction类似，只是多了一些参数关联，具体如下
		/* create gc_event closure */
		lua_pushstring(L, "tolua_gc_event");//stack:"tolua_gc_event"
		lua_pushstring(L, "tolua_gc");//stack:"tolua_gc_event" "tolua_gc"
		lua_rawget(L, LUA_REGISTRYINDEX);//stack:"tolua_gc_event" t["tolua_gc"](t是LUA_REGISTRYINDEX对应的lua注册表）
		lua_pushstring(L, "tolua_super");//stack:"tolua_gc_event" t["tolua_gc"] "tolua_super"
		lua_rawget(L, LUA_REGISTRYINDEX);//stack:"tolua_gc_event" t["tolua_gc"] t["tolua_super"]
		lua_pushcclosure(L, class_gc_event, 2);//这里指明了  函数class_gc_event关联了两个参数，也就是栈顶以及第二个参数，并且弹出他们，stack:"tolua_gc_event" class_gc_event
		lua_rawset(L, LUA_REGISTRYINDEX);//设置了t["tolua_gc_event"] = class_gc_event,并且将他们弹出栈，stack:空

		tolua_newmetatable(L, "tolua_commonclass");//创建了一个名字为XXX的元表，并且在这个函数里面做了一些初始化，有兴趣的同学可以跟进函数内部看看他是怎么实现的，里面也很简单

												   //这里获得module，module可以简单粗暴地理解为c的命名空间，实际上它是一个table,tolua_module函数的展开在下面详解,ps:不会改变栈结构，因为他最终会把压入的都弹出
		tolua_module(L, NULL, 0);
		tolua_beginmodule(L, NULL);//这里就是使用刚才通过tolua_module创建出来的表，也就是把对应的module表压入栈，具体实现放在下面,这里栈已经存在一个module了，这里是全局表在栈顶
		tolua_module(L, "tolua", 0);//上面讲过了，这里就是创建了一个名字叫做tolua的table作为module
		tolua_beginmodule(L, "tolua");//使用tolua的module，注意：上面之前栈上已经存在全局表在栈顶了，这里还会再把tolua的table压入栈顶
		/*
		tolua_function(L, "type", tolua_bnd_type);//这个函数比较简单，具体分析在下面，几个相同的就不重复分析了
		tolua_function(L, "takeownership", tolua_bnd_takeownership);
		tolua_function(L, "releaseownership", tolua_bnd_releaseownership);
		tolua_function(L, "cast", tolua_bnd_cast);
		tolua_function(L, "isnull", tolua_bnd_isnulluserdata);
		tolua_function(L, "inherit", tolua_bnd_inherit);
#ifdef LUA_VERSION_NUM // lua 5.1 
		tolua_function(L, "setpeer", tolua_bnd_setpeer);
		tolua_function(L, "getpeer", tolua_bnd_getpeer);
#endif
		tolua_function(L, "getcfunction", tolua_bnd_getcfunction);
		tolua_function(L, "iskindof", tolua_bnd_iskindof);
*/
		tolua_endmodule(L);//顾名思义，肯定就是回收module，就是从栈上弹出module
		tolua_endmodule(L);
	}
	lua_settop(L, top);//恢复栈的初始状态
}

void tolua_module(lua_State* L, const char* name, int hasvar)
{
	//在这里可以看出其实module就是一张table
	//cocos源生的注释已经很详细了，这里大概流程就是，
	//如果传入name，即module名字，那么就会先查一下它以前是不是创建过module表了，如果没有那么就会新建一个table，
	if (name)
	{
		/* tolua module */
		lua_pushstring(L, name);//stack: name
		lua_rawget(L, -2);//(这里其实有点小坑，这里要根据上下文，也就是要追溯很久之前的栈状态，因为这里栈只有一个值，但是尝试去取-2，应该是在之前往栈中push一个全局表)尝试获取名字为name的全局表，并push入栈
		if (!lua_istable(L, -1))  /* check if module already exists */
		{
			//如果没有就先把刚才那个nil弹出去，新建一个表，取名为name，并且push入栈
			lua_pop(L, 1);//stack：global
			lua_newtable(L);//stack：global table
			lua_pushstring(L, name);//stack：global table name
			lua_pushvalue(L, -2);//stack: global table name table
			lua_rawset(L, -4);//stack：global table
		}
	}
	else
	{
		//如果没传名字，就直接把全局表拿出来push入栈
		/* global table */
		//int globalIndex = luaL_ref(L,LUA_RIDX_GLOBALS);
		//lua_pushvalue(L, globalIndex);
		lua_pushglobaltable(L);
	}
	/*
	if (hasvar)
	{
		//这里是设置metatable的，语法也都讲过，这里可以尝试自己翻译一下
		if (!tolua_ismodulemetatable(L))  // check if it already has a module metatable /
		{
			// create metatable to get/set C/C++ variable /
			lua_newtable(L);
			tolua_moduleevents(L);
			if (lua_getmetatable(L, -2))
				lua_setmetatable(L, -2);  /// set old metatable as metatable of metatable /
			lua_setmetatable(L, -2);
		}
	}
*/
	//最后把建好的module表弹出栈
	lua_pop(L, 1);               /* pop module */
}

void tolua_beginmodule(lua_State* L, const char* name)
{
	//这里很多api都与之前讲过的类似，各位同学可以自行翻译栈结构
	//大体意思就是如果你传了name，那么他就会尝试查找名字叫做name的一个table，如果isclass标志位为true，那就会获取它的metatable，最终的栈结构为 stack: module metatable；如果他不是一个class，那么直接就用这个table，而不用取它的metatable，栈结构就是：stack:module table
	//如果没有传name，那么直接就用全局表作为返回值，push入栈以供后面使用
	if (name) { // ... module
				//---- now module[name] is a table, get it's metatable to store keys
				// get module[name]
		lua_pushstring(L, name); // ... module name
		lua_rawget(L, -2);       // ... module module[name]
								 // Is module[name] a class table?
		lua_pushliteral(L, ".isclass");
		lua_rawget(L, -2);                  // stack: ... module module[name] class_flag
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);                  // stack: ... module module[name]
			return;                         // not a class table, use origin table
		}
		lua_pop(L, 1);                      // stack: ... module class_table
											// get metatable
		if (lua_getmetatable(L, -1)) {  // ... module class_table mt
			lua_remove(L, -2);          // ... module mt
		}
		//---- by SunLightJuly, 2014.6.5
	}
	else {
		//int globalIndex = luaL_ref(L, LUA_RIDX_GLOBALS);
		//lua_pushvalue(L, globalIndex);
		lua_pushglobaltable(L);
	}
}

void tolua_endmodule(lua_State*L)
{
	lua_pop(L, 1);
}

void tolua_function(lua_State* L, const char* name, lua_CFunction func)
{
	//之前已经把module压入栈了，所以大概栈结构stack：... module
	lua_pushstring(L, name);//stack：... module name
	lua_pushcfunction(L, func);//stack： ... module name func
	lua_rawset(L, -3);//这里把module[name] = func，并且弹出name func, 意思就是在module表中绑定了一个名字为name的函数,stack: ... module
}

int lua_register_moonton_Foo(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "Foo");//创建元表
	tolua_cclass(tolua_S, "Foo", "Foo", "", nullptr);//创建类表，并把元表复制给类表

	tolua_beginmodule(tolua_S, "Foo");
	/*
	tolua_function(tolua_S, "GetObjID", lua_moonton_CGameFunc_GetObjID);
	tolua_function(tolua_S, "Int64Comp", lua_moonton_CGameFunc_Int64Comp);
	tolua_function(tolua_S, "TextFieldDetachIME", lua_moonton_CGameFunc_TextFieldDetachIME);
*/
	tolua_function(tolua_S, "new", [](lua_State*L) {
		Foo*obj = new Foo(123);
		lua_newtable(L);
		Foo**a = (Foo**)lua_newuserdata(L, sizeof(Foo*));
		*a = obj;
		return 1;
	});
	unsigned num = sizeof(lua_Foo_functions) / sizeof(*lua_Foo_functions);
	for (int i = 0; i < num; ++i)
	{
		tolua_function(tolua_S, lua_Foo_functions[i].name, lua_Foo_functions[i].method);
	}
	//tolua_function(tolua_S, "TextFieldDetachIME", lua_moonton_CGameFunc_TextFieldDetachIME);

	tolua_endmodule(tolua_S);
	std::string typeName = typeid(Foo).name();
	/*
	g_luaType[typeName] = "Foo";
	g_typeCast["Foo"] = "Foo";
*/
	return 1;
}

void tolua_usertype(lua_State* L, const char* type)
{
	char ctype[128] = "const ";
	strncat_s(ctype, 128, type, 120);

	/* create both metatables */
	//创建了两个元表，并且在mapsuper中继承，具体的细节在下面
	if (tolua_newmetatable(L, ctype) && tolua_newmetatable(L, type))
		mapsuper(L, type, ctype);             /* 'type' is also a 'const type' */
}

static int tolua_newmetatable(lua_State* L, const char* name)
{
	int r = luaL_newmetatable(L, name);//创建了一个名为name的表，并且压入栈顶，返回值表示是否是新建的还是以前就曾经建过的（1，新建，0老的）

	if (r) {
		//如果是新的那就把这张表作为key，name作为value保存在注册表里
		lua_pushvalue(L, -1);
		lua_pushstring(L, name);
		lua_settable(L, LUA_REGISTRYINDEX); /* reg[mt] = type_name */
	};

	//将栈顶的metatable弹出，保持栈干净
	lua_pop(L, 1);
	return r;
}

static void mapsuper(lua_State* L, const char* name, const char* base)
{
	/* push registry.super */
	//tolua_super这个东东在一开始的lua_open中创建的，是一张表,这里把它当做基类取出
	lua_pushstring(L, "tolua_super");    /* stack: "tolua_super" */
	lua_rawget(L, LUA_REGISTRYINDEX);    /* stack: super */
										 //获取名为name的元表，之前的函数已经创建好
	luaL_getmetatable(L, name);          /* stack: super mt */
										 //尝试从tolua_super这个table中获取metatable为key的value，之前初始化啥都没做，正常这里应该是空的，然后把结果压入栈
	lua_rawget(L, -2);                   /* stack: super table */
	if (lua_isnil(L, -1))
	{
		//发现如果曾经没初始化过，就进来初始化，就是创建一个metatable，并把它赋给tolua_super基类作为元表，栈结构cocos已经写得很详细了，我这里就不修改它的了
		/* create table */
		lua_pop(L, 1);
		lua_newtable(L);                    /* stack: super table */
		luaL_getmetatable(L, name);          /* stack: super table mt */
		lua_pushvalue(L, -2);                /* stack: super table mt table */
		lua_rawset(L, -4);                   /* stack: super table */
	}

	/* set base as super class */
	lua_pushstring(L, base);//传入的基类名压入栈
	lua_pushboolean(L, 1);//压入true
	lua_rawset(L, -3);//table[base]=true                    /* stack: super table */

					  /* set all super class of base as super class of name */
	luaL_getmetatable(L, base);          /* stack: super table base_mt */
	lua_rawget(L, -3);                   /* stack: super table base_table */
	if (lua_istable(L, -1))
	{
		//遍历积累的metatable，然后将基类metatable的key value值全部copy到name对应的元表中
		/* traverse base table */
		lua_pushnil(L);  /* first key */ /* stack: super table base_table nil*/
		while (lua_next(L, -2) != 0)
		{
			/* stack: ... base_table key value */
			lua_pushvalue(L, -2);    /* stack: ... base_table key value key */
			lua_insert(L, -2);       /* stack: ... base_table key key value */
			lua_rawset(L, -5);       /* stack: ... base_table key */
		}
	}
	lua_pop(L, 3);                       /* stack: <empty> */
}

void tolua_cclass(lua_State* L, const char* lname, const char* name, const char* base, lua_CFunction col)
{
	char cname[128] = "const ";
	char cbase[128] = "const ";
	strncat_s(cname, 128, name, 120);
	strncat_s(cbase, 128, base, 120);

	//mapinheritance(L, name, base);
	//mapinheritance(L, cname, name);

	mapsuper(L, cname, cbase);
	mapsuper(L, name, base);

	lua_pushstring(L, lname);

	//push_collector(L, name, col);

	luaL_getmetatable(L,name);
	lua_pushstring(L,".collector");
	lua_pushcfunction(L,col);
	lua_rawset(L,-3);
	lua_pop(L,1);

	//关键部分的栈结构注释已经很全，就不再重复注释：
	//---- create a new class table, set it's metatable, and assign it to module
	lua_newtable(L);                    // stack: module lname table
	luaL_getmetatable(L, name);          // stack: module lname table mt
	lua_setmetatable(L, -2);            // stack: module lname table
										//在这里把新创建的表的".isclass"标识置为true，然后在module表中保存新建的类表，module[lname] = table
										//Use a key named ".isclass" to be a flag of class_table
	lua_pushliteral(L, ".isclass");
	lua_pushboolean(L, 1);
	lua_rawset(L, -3);                  // stack: module lname table
	lua_rawset(L, -3);                  // stack: module
										//---- by SunLightJuly, 2014.6.5

										/* now we also need to store the collector table for the const
										instances of the class */
	//push_collector(L, cname, col);

	luaL_getmetatable(L,cname);
	lua_pushstring(L,".collector");
	lua_pushcfunction(L,col);
	lua_rawset(L,-3);
	lua_pop(L,1);
}



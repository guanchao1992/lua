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
	int top = lua_gettop(L);//��ȡջ����ţ���Ϊindex��1��ʼ�����Ա�����ڻ�ȡջ����һ���ж���Ԫ�أ�
	lua_pushstring(L, "tolua_opened");//����tolua_opened���ַ���ѹ��ջ����ʱջ�ṹ��stack����tolua_opened��
	lua_rawget(L, LUA_REGISTRYINDEX);//��ȡ��ΪLUA_REGISTRYINDEX�ı�t��Ȼ��ȡջ��Ԫ��k������ջ��Ԫ��k��Ȼ��ֵt[k]ѹ��ջ������ʱջ�ṹ��stack��value
	if (!lua_isboolean(L, -1))//�������ظ���ʼ�����Ժ���tolua_opened��Ϊtrue������-1ָ����ջ��������ջ�����������������ͼ�������ж�ջ���Ƿ�Ϊboolֵ
	{
		lua_pushstring(L, "tolua_opened");//����tolua_opened��ѹ��ջ����ʱջ�ṹ��stack��value ��tolua_opened��
		lua_pushboolean(L, 1);//��1����һ��boolֵѹ��ջ����ʱջ�ṹ��value ��tolua_opened�� 1
		lua_rawset(L, LUA_REGISTRYINDEX);//��ȡluaע���t��ջ��Ԫ����Ϊvalue��ջ������һ����Ϊkey��t[key]=value,����value��key����ʱջ�ṹ��stack��value

										 // create value root table
		lua_pushstring(L, TOLUA_VALUE_ROOT);//stringѹ��ջ��ջ�ṹ��stack��value TOLUA_VALUE_ROOT
		lua_newtable(L);//�����±���ѹ��ջ��ջ�ṹ��value TOLUA_VALUE_ROOT table
		lua_rawset(L, LUA_REGISTRYINDEX);//��ȡluaע���t��ջ�е�table��Ϊvalue��TOLUA_VALUE_ROOT��Ϊkey��ʵ���ϵ���t[TOLUA_VALUE_ROOT] = table���ֱ𵯳�table��TOLUA_VALUE_ROOT��ջ�ṹ��stack��value

#ifndef LUA_VERSION_NUM /* only prior to lua 5.1 */
										 /* create peer object table */
		lua_pushstring(L, "tolua_peers");//�ַ���ѹ��ջ��ջ�ṹ��stack��value "tolua_peers"
		lua_newtable(L);//����һ���±�ѹ��ջ��ջ�ṹ��stack��value "tolua_peers" table
						/* make weak key metatable for peers indexed by userdata object */
		lua_newtable(L);//����һ���±�ѹ��ջ��ջ�ṹ��stack��value "tolua_peers" table table1
		lua_pushliteral(L, "__mode");//ѹ��һ���ַ�����ջ�ṹ��stack��value "tolua_peers" table table1 "__mode"
		lua_pushliteral(L, "k");//ѹ��һ���ַ�����ջ�ṹ��stack��value "tolua_peers" table table1 "__mode" "k"
		lua_rawset(L, -3);//�ֱ�ȡջ��Ԫ����Ϊvalue����һ����Ϊkey����������Ϊt��t[key]=value,������ջ���͵ڶ���Ԫ�أ�(����ʵ������ʵ��table��������)��ǰջ�ṹ��stack��value "tolua_peers" table table1
		lua_setmetatable(L, -2);//��ջ��table1��Ϊ�ڶ���Ԫ��table��Ԫ������table1��ջ�ṹ��stack��value "tolua_peers" table
		lua_rawset(L, LUA_REGISTRYINDEX);//ջ��Ԫ����Ϊvalue���ڶ���Ԫ����Ϊkey��luaע�����Ϊt��t[key]=value,�ֱ𵯳�ջ��Ԫ�ء��ڶ���Ԫ�أ�ջ�ṹ��stack��value
#endif
										 //�������������ƣ��Ͳ��ڲ���
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

		//����ʵ���ϴ�������Ϊ"tolua_super"��"tolua_gc"���ű����ұ�����luaע�����
		lua_pushstring(L, "tolua_super");
		lua_newtable(L);
		lua_rawset(L, LUA_REGISTRYINDEX);
		lua_pushstring(L, "tolua_gc");
		lua_newtable(L);
		lua_rawset(L, LUA_REGISTRYINDEX);

		//��������
		//�����õ���lua_pushcclosure����ʵ���ǹ�����һЩ������c��������lua_pushcfunction���ƣ�ֻ�Ƕ���һЩ������������������
		/* create gc_event closure */
		lua_pushstring(L, "tolua_gc_event");//stack:"tolua_gc_event"
		lua_pushstring(L, "tolua_gc");//stack:"tolua_gc_event" "tolua_gc"
		lua_rawget(L, LUA_REGISTRYINDEX);//stack:"tolua_gc_event" t["tolua_gc"](t��LUA_REGISTRYINDEX��Ӧ��luaע���
		lua_pushstring(L, "tolua_super");//stack:"tolua_gc_event" t["tolua_gc"] "tolua_super"
		lua_rawget(L, LUA_REGISTRYINDEX);//stack:"tolua_gc_event" t["tolua_gc"] t["tolua_super"]
		lua_pushcclosure(L, class_gc_event, 2);//����ָ����  ����class_gc_event����������������Ҳ����ջ���Լ��ڶ������������ҵ������ǣ�stack:"tolua_gc_event" class_gc_event
		lua_rawset(L, LUA_REGISTRYINDEX);//������t["tolua_gc_event"] = class_gc_event,���ҽ����ǵ���ջ��stack:��

		tolua_newmetatable(L, "tolua_commonclass");//������һ������ΪXXX��Ԫ�����������������������һЩ��ʼ��������Ȥ��ͬѧ���Ը��������ڲ�����������ôʵ�ֵģ�����Ҳ�ܼ�

												   //������module��module���Լ򵥴ֱ������Ϊc�������ռ䣬ʵ��������һ��table,tolua_module������չ�����������,ps:����ı�ջ�ṹ����Ϊ�����ջ��ѹ��Ķ�����
		tolua_module(L, NULL, 0);
		tolua_beginmodule(L, NULL);//�������ʹ�øղ�ͨ��tolua_module���������ı�Ҳ���ǰѶ�Ӧ��module��ѹ��ջ������ʵ�ַ�������,����ջ�Ѿ�����һ��module�ˣ�������ȫ�ֱ���ջ��
		tolua_module(L, "tolua", 0);//���潲���ˣ�������Ǵ�����һ�����ֽ���tolua��table��Ϊmodule
		tolua_beginmodule(L, "tolua");//ʹ��tolua��module��ע�⣺����֮ǰջ���Ѿ�����ȫ�ֱ���ջ���ˣ����ﻹ���ٰ�tolua��tableѹ��ջ��
		/*
		tolua_function(L, "type", tolua_bnd_type);//��������Ƚϼ򵥣�������������棬������ͬ�ľͲ��ظ�������
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
		tolua_endmodule(L);//����˼�壬�϶����ǻ���module�����Ǵ�ջ�ϵ���module
		tolua_endmodule(L);
	}
	lua_settop(L, top);//�ָ�ջ�ĳ�ʼ״̬
}

void tolua_module(lua_State* L, const char* name, int hasvar)
{
	//��������Կ�����ʵmodule����һ��table
	//cocosԴ����ע���Ѿ�����ϸ�ˣ����������̾��ǣ�
	//�������name����module���֣���ô�ͻ��Ȳ�һ������ǰ�ǲ��Ǵ�����module���ˣ����û����ô�ͻ��½�һ��table��
	if (name)
	{
		/* tolua module */
		lua_pushstring(L, name);//stack: name
		lua_rawget(L, -2);//(������ʵ�е�С�ӣ�����Ҫ���������ģ�Ҳ����Ҫ׷�ݺܾ�֮ǰ��ջ״̬����Ϊ����ջֻ��һ��ֵ�����ǳ���ȥȡ-2��Ӧ������֮ǰ��ջ��pushһ��ȫ�ֱ�)���Ի�ȡ����Ϊname��ȫ�ֱ���push��ջ
		if (!lua_istable(L, -1))  /* check if module already exists */
		{
			//���û�о��ȰѸղ��Ǹ�nil����ȥ���½�һ����ȡ��Ϊname������push��ջ
			lua_pop(L, 1);//stack��global
			lua_newtable(L);//stack��global table
			lua_pushstring(L, name);//stack��global table name
			lua_pushvalue(L, -2);//stack: global table name table
			lua_rawset(L, -4);//stack��global table
		}
	}
	else
	{
		//���û�����֣���ֱ�Ӱ�ȫ�ֱ��ó���push��ջ
		/* global table */
		//int globalIndex = luaL_ref(L,LUA_RIDX_GLOBALS);
		//lua_pushvalue(L, globalIndex);
		lua_pushglobaltable(L);
	}
	/*
	if (hasvar)
	{
		//����������metatable�ģ��﷨Ҳ��������������Գ����Լ�����һ��
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
	//���ѽ��õ�module����ջ
	lua_pop(L, 1);               /* pop module */
}

void tolua_beginmodule(lua_State* L, const char* name)
{
	//����ܶ�api����֮ǰ���������ƣ���λͬѧ�������з���ջ�ṹ
	//������˼��������㴫��name����ô���ͻ᳢�Բ������ֽ���name��һ��table�����isclass��־λΪtrue���Ǿͻ��ȡ����metatable�����յ�ջ�ṹΪ stack: module metatable�����������һ��class����ôֱ�Ӿ������table��������ȡ����metatable��ջ�ṹ���ǣ�stack:module table
	//���û�д�name����ôֱ�Ӿ���ȫ�ֱ���Ϊ����ֵ��push��ջ�Թ�����ʹ��
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
	//֮ǰ�Ѿ���moduleѹ��ջ�ˣ����Դ��ջ�ṹstack��... module
	lua_pushstring(L, name);//stack��... module name
	lua_pushcfunction(L, func);//stack�� ... module name func
	lua_rawset(L, -3);//�����module[name] = func�����ҵ���name func, ��˼������module���а���һ������Ϊname�ĺ���,stack: ... module
}

int lua_register_moonton_Foo(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "Foo");//����Ԫ��
	tolua_cclass(tolua_S, "Foo", "Foo", "", nullptr);//�����������Ԫ���Ƹ����

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
	//����������Ԫ��������mapsuper�м̳У������ϸ��������
	if (tolua_newmetatable(L, ctype) && tolua_newmetatable(L, type))
		mapsuper(L, type, ctype);             /* 'type' is also a 'const type' */
}

static int tolua_newmetatable(lua_State* L, const char* name)
{
	int r = luaL_newmetatable(L, name);//������һ����Ϊname�ı�����ѹ��ջ��������ֵ��ʾ�Ƿ����½��Ļ�����ǰ�����������ģ�1���½���0�ϵģ�

	if (r) {
		//������µ��ǾͰ����ű���Ϊkey��name��Ϊvalue������ע�����
		lua_pushvalue(L, -1);
		lua_pushstring(L, name);
		lua_settable(L, LUA_REGISTRYINDEX); /* reg[mt] = type_name */
	};

	//��ջ����metatable����������ջ�ɾ�
	lua_pop(L, 1);
	return r;
}

static void mapsuper(lua_State* L, const char* name, const char* base)
{
	/* push registry.super */
	//tolua_super���������һ��ʼ��lua_open�д����ģ���һ�ű�,���������������ȡ��
	lua_pushstring(L, "tolua_super");    /* stack: "tolua_super" */
	lua_rawget(L, LUA_REGISTRYINDEX);    /* stack: super */
										 //��ȡ��Ϊname��Ԫ��֮ǰ�ĺ����Ѿ�������
	luaL_getmetatable(L, name);          /* stack: super mt */
										 //���Դ�tolua_super���table�л�ȡmetatableΪkey��value��֮ǰ��ʼ��ɶ��û������������Ӧ���ǿյģ�Ȼ��ѽ��ѹ��ջ
	lua_rawget(L, -2);                   /* stack: super table */
	if (lua_isnil(L, -1))
	{
		//�����������û��ʼ�������ͽ�����ʼ�������Ǵ���һ��metatable������������tolua_super������ΪԪ��ջ�ṹcocos�Ѿ�д�ú���ϸ�ˣ�������Ͳ��޸�������
		/* create table */
		lua_pop(L, 1);
		lua_newtable(L);                    /* stack: super table */
		luaL_getmetatable(L, name);          /* stack: super table mt */
		lua_pushvalue(L, -2);                /* stack: super table mt table */
		lua_rawset(L, -4);                   /* stack: super table */
	}

	/* set base as super class */
	lua_pushstring(L, base);//����Ļ�����ѹ��ջ
	lua_pushboolean(L, 1);//ѹ��true
	lua_rawset(L, -3);//table[base]=true                    /* stack: super table */

					  /* set all super class of base as super class of name */
	luaL_getmetatable(L, base);          /* stack: super table base_mt */
	lua_rawget(L, -3);                   /* stack: super table base_table */
	if (lua_istable(L, -1))
	{
		//�������۵�metatable��Ȼ�󽫻���metatable��key valueֵȫ��copy��name��Ӧ��Ԫ����
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

	//�ؼ����ֵ�ջ�ṹע���Ѿ���ȫ���Ͳ����ظ�ע�ͣ�
	//---- create a new class table, set it's metatable, and assign it to module
	lua_newtable(L);                    // stack: module lname table
	luaL_getmetatable(L, name);          // stack: module lname table mt
	lua_setmetatable(L, -2);            // stack: module lname table
										//��������´����ı��".isclass"��ʶ��Ϊtrue��Ȼ����module���б����½������module[lname] = table
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



#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "..\base\Singleton.h"
#include <string>
#include "..\..\MyTool\tool_log.h"
#include <list>
#include <functional>
#include "..\base\Position2D.h"

typedef unsigned long EventType;

enum RegEventType
{
	EventRegType_Null = 0,
	EventRegType_Mouse,
	EventRegType_Key,


	EventRegType_Custom = 9999,
	EventRegType_Max,
};

class EventArgs
{
public:
	EventArgs(EventType type_) 
		:type(type_)
	{ }
public:
	EventType type;
};
class CustomEventArgs :public EventArgs
{
public:
	CustomEventArgs(const string&args) :
		EventArgs(EventRegType_Custom),
		param(args)
	{ }
public:
	string param;
};

class MouseEventArgs :public EventArgs
{
public:
	enum MouseType
	{
		LBMouseDown,
		LBMouseUp,
		RBMouseDown,
		RBMouseUp,
		MouseMove,
	};
	MouseEventArgs(const Position2D& viewPos_, MouseType mouseType_) :
		EventArgs(EventRegType_Mouse),
		viewPos(viewPos_),
		mouseType(mouseType_)
	{ }
public:
	Position2D	viewPos;
	MouseType	mouseType;
};

class KeyEventArgs :public EventArgs
{
public:
	enum KeyType
	{
		KeyDown,
		KeyUp,
	};
	KeyEventArgs(KeyType keyType_,unsigned int key_, unsigned int controlKey_, unsigned int virtualKey_) :
		EventArgs(EventRegType_Key),
		keyType(keyType_),
		key(key_),
		controlKey(controlKey_),
		virtualKey(virtualKey_)
	{ }
public:
	unsigned int key;
	unsigned int keyType;
	unsigned int controlKey;	// VK_SHIFT;
	unsigned int virtualKey;
};

typedef std::function<void(EventArgs*)> EventFunc;

struct Event
{
	EventType		type;		//事件类型
	std::string		name;		//事件标识
	EventFunc		func;	
	int				order;		//在type相同时生效，越小越先响应
};

typedef std::list<Event> Events;

class EventManager
{
	friend lua_State*GetLuaState();;
	SingletonClase(EventManager);
private:
	EventManager();
public:
	~EventManager();
public:
	void	Init();

	void	regEvent(EventType type, const std::string&name, const EventFunc& function, int order = 0);
	void	clearEvent(EventType type, const std::string&name);
	void	clearAllEvent();

	//请传入一个new的指针
	void	fireEvent(EventArgs* args);
	bool	runEvent();

	//不添加到消息队列，立即执行 
	void    fireEventImmediately(EventArgs*args);
private:
	std::map<EventType, Events> m_mapEvent;
	std::list<EventArgs*>		m_listFireEvent;
};


#define RegEvent(TYPE,NAME,FUNC,ORDER) EventManager::getInstance()->regEvent(TYPE, NAME, std::bind(&FUNC, this, std::placeholders::_1),ORDER);
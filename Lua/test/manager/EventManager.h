#ifndef ____EventManager_H____
#define ____EventManager_H____

#include "EventBase.h"

#include "..\base\Singleton.h"
#include <string>
#include "..\..\MyTool\tool_log.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


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
private:
	std::map<EventType, Events> m_mapEvent;
	std::list<EventArgs*>		m_listFireEvent;
};


#define RegEvent(TYPE,NAME,FUNC,ORDER) EventManager::getInstance()->regEvent(TYPE, NAME, std::bind(&FUNC, this, std::placeholders::_1),ORDER);

#endif
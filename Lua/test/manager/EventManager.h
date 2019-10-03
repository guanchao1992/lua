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
	EventType		type;		//�¼�����
	std::string		name;		//�¼���ʶ
	EventFunc		func;
	int				order;		//��type��ͬʱ��Ч��ԽСԽ����Ӧ
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

	//�봫��һ��new��ָ��
	void	fireEvent(EventArgs* args);
	bool	runEvent();
private:
	std::map<EventType, Events> m_mapEvent;
	std::list<EventArgs*>		m_listFireEvent;
};


#define RegEvent(TYPE,NAME,FUNC,ORDER) EventManager::getInstance()->regEvent(TYPE, NAME, std::bind(&FUNC, this, std::placeholders::_1),ORDER);

#endif
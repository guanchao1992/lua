#include "EventManager.h"
#include "..\..\MyTool\tool_log.h"
#include "..\config.h"
#include "..\..\MyLua\ctolua.h"

SingletonClaseCpp(EventManager);


EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}

void EventManager::Init()
{

}

void EventManager::regEvent(EventType type, const std::string&name, const EventFunc& function, int order)
{
	clearEvent(type, name);
	Events&evs = m_mapEvent[type];
	Event newEv;
	newEv.type = type;
	newEv.name = name;
	newEv.func = function;
	newEv.order = order;
	bool mark = false;

	for (auto it = evs.begin(); it != evs.end(); ++it)
	{
		if (it->order >= order)
		{
			evs.insert(it, newEv);
			mark = true;
		}
	}
	if (!mark)
	{
		evs.push_back(newEv);
	}
}

void EventManager::clearEvent(EventType type, const std::string&name)
{
	if (m_mapEvent.find(type) == m_mapEvent.end())
		return;
	Events& evs = m_mapEvent[type];
	for (auto it = evs.begin(); it != evs.end(); ++it)
	{
		if (it->name == name)
		{
			evs.erase(it);
			break;
		}
	}
}

void EventManager::clearAllEvent()
{
	m_mapEvent.clear();
}

void EventManager::fireEvent(EventArgs*args)
{
	m_listFireEvent.push_back(args);
}

bool EventManager::runEvent()
{
	if (m_listFireEvent.size() == 0)
	{
		return false;
	}
	//将消息列表拷贝出来，防止出现消息死循环
	std::list<EventArgs*> listFireEvent = m_listFireEvent;
	m_listFireEvent.clear();

	for (auto fireEv : listFireEvent)
	{
		Events&evs = m_mapEvent[fireEv->type];
		for (auto ev : evs)
		{
			ev.func(fireEv);
		}
		delete fireEv;
	}
	listFireEvent.clear();
	return true;
}

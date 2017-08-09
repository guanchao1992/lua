#include "KeyManager.h"
#include "..\base\NodeList.h"


SingletonClaseCpp(KeyManager);

KeyManager::KeyManager()
{
}

KeyManager::~KeyManager()
{
}

bool KeyManager::Init()
{
	return true;
}

bool KeyManager::RegKey(unsigned int key, const std::string&id, UINT keyEventType, KEYMANAGER_FUNC_EVENT functor, float span)
{
	KeyMS& ems = m_mapKeyEvent[key];
	ems.mapKey[id].id = id;
	ems.mapKey[id].functor = functor;
	ems.mapKey[id].type = keyEventType;
	ems.mapKey[id].loopSpan = span;
	return true;
}

void KeyManager::Update(float t)
{
	for (auto &ems : m_mapKeyEvent)
	{
		if (ems.second.isDown)
		{
			for (auto &mapKey : ems.second.mapKey)
			{
				if (mapKey.second.type&KeyEventType::Loop)
				{
					mapKey.second.dtime += t;
					if (mapKey.second.dtime > mapKey.second.loopSpan)
					{
						mapKey.second.dtime -= mapKey.second.loopSpan;
						mapKey.second.functor();
					}
				}
			}
		}
	}
}

void KeyManager::KeyDown(unsigned int key)
{
	if (m_mapKeyEvent.find(key) == m_mapKeyEvent.end())
		return;
	KeyMS& ems = m_mapKeyEvent[key];
	if (ems.isDown == false)
	{
		ems.isDown = true;
		for (auto& mapKey : ems.mapKey)
		{
			mapKey.second.dtime = 0.f;
			if (mapKey.second.type & KeyEventType::Down)
			{
				mapKey.second.functor();
			}
		}
	}
}

void KeyManager::KeyUp(unsigned int key)
{
	if (m_mapKeyEvent.find(key) == m_mapKeyEvent.end())
		return;
	KeyMS& ems = m_mapKeyEvent[key];
	if (ems.isDown == true)
	{
		ems.isDown = false;
		for (auto mapKey : ems.mapKey)
		{
			if (mapKey.second.type & KeyEventType::Up)
			{
				mapKey.second.functor();
			}
		}
	}
}

#include "ObjectManager.h"
#include "..\base\RList.h"


SingletonClaseCpp(ObjectManager);

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::autoRelease(Ref * ref)
{
	m_listRef.push_back(ref);
}

void ObjectManager::checkDelete()
{
	for (auto it = m_listRef.begin(); it != m_listRef.end();)
	{
		if ((*it)->m_countReference == 1)
		{
			(*it)->release();
			it = m_listRef.erase(it);
		}
		else
		{
			++it;
		}
	}
}

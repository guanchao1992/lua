#pragma once
#include "..\base\Singleton.h"
#include "..\base\Ref.h"
#include <list>

class ObjectManager
{
	SingletonClase(ObjectManager);
public:
	ObjectManager();
	~ObjectManager();

public:	
	void autoRelease(Ref* ref);

	void checkDelete();
private:
	std::list<Ref*>			m_listRef;
};

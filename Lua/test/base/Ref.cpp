#include "Ref.h"
#include "..\manager\ObjectManager.h"
#include <assert.h>


Ref::Ref()
	:m_countReference(1)
	, m_delete(false)
{
}

Ref::~Ref()
{
}

void Ref::retain()
{
	++m_countReference;
}

void Ref::release()
{
	if (m_countReference == 0)
	{
		assert(1);	//不可能执行到这里
		return;
	}
	if (--m_countReference == 0)
	{
		delete this;
	}
}

void Ref::autorelease()
{
	ObjectManager::getInstance()->autoRelease(this);
}

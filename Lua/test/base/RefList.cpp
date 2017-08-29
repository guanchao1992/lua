#include "RefList.h"
#include "..\manager\ObjectManager.h"
#include <assert.h>
#include "Ref.h"


RefList::RefList()
{
}

RefList::~RefList()
{
	Clear();
}

void RefList::PushBack(Ref*ref)
{
	m_listRef.push_back(ref);
	ref->retain();
}

void RefList::PushFront(Ref*ref)
{
	m_listRef.push_front(ref);
	ref->retain();
}

void RefList::PopBack()
{
	if (m_listRef.size() == 0)
	{
		return;
	}
	Ref* ref = m_listRef.back();
	m_listRef.pop_back();
	ref->release();
}

void RefList::PopFront()
{
	if (m_listRef.size() == 0)
	{
		return;
	}
	Ref*ref = m_listRef.front();
	m_listRef.pop_front();
	ref->release();
}

Ref* RefList::Front()
{
	if (m_listRef.size() == 0)
	{
		return nullptr;
	}
	return m_listRef.front();
}

Ref* RefList::Back()
{
	if (m_listRef.size() == 0)
	{
		return nullptr;
	}
	return m_listRef.back();
}

std::list<Ref*>::iterator RefList::Begin()
{
	return m_listRef.begin();
}

std::list<Ref*>::iterator RefList::End()
{
	return m_listRef.end();
}

void RefList::Insert(unsigned int index, Ref*ref)
{
	if (index >= m_listRef.size())
	{
		PushBack(ref);
		return;
	}
	int i = 0;
	for (auto it = m_listRef.begin(); it != m_listRef.end(); ++i, ++it)
	{
		if (i == index)
		{
			m_listRef.insert(it, ref);
			ref->retain();
			return;
		}
	}
	assert(1);	//index是错的
}

void RefList::Clear()
{
	for (auto it = m_listRef.begin(); it != m_listRef.end(); ++it)
	{
		(*it)->release();
	}
	m_listRef.clear();
}

Ref * RefList::getRefAtIndex(int index)
{
	int i = 0;
	for (auto it : m_listRef)
	{
		if (i == index)
		{
			return it;
		}
		++i;
	}
	return nullptr;
}

Ref* RefList::removeFromTag(int tag)
{
	for (auto it = m_listRef.begin(); it != m_listRef.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			Ref * ret = (*it);
			(*it)->release();
			m_listRef.erase(it);
			return ret;
		}
	}
	assert(1);	//要移除的元素不存在
	return nullptr;
}

Ref* RefList::removeFromIndex(unsigned int index)
{
	int i = 0;
	for (auto it = m_listRef.begin(); it != m_listRef.end(); ++i, ++it)
	{
		if (i == index)
		{
			Ref * ret = (*it);
			(*it)->release();
			m_listRef.erase(it);
			return ret;
		}
	}
	assert(1);	//index是错的
	return nullptr;
}

void RefList::removeRef(Ref*ref)
{
	for (auto it = m_listRef.begin(); it != m_listRef.end(); ++it)
	{
		if (ref == *it)
		{
			(*it)->release();
			m_listRef.erase(it);
			return;
		}
	}
	assert(1);	//要移除的元素不存在
}

void RefList::ergodicFunc(FUNC_ListErgodic func)
{
	bool isDelete = false;
	bool isEnd = false;
	for (auto it = m_listRef.begin(); it != m_listRef.end(); ++it)
	{
		isDelete = false;
		func(*it, isDelete, isEnd);
		if (isDelete)
		{
			(*it)->release();
			it = m_listRef.erase(it);
		}
		else
		{
			++it;
		}
		if (isEnd)
		{
			break;
		}
	}
}

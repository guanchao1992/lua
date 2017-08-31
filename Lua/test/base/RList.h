#pragma once
#include <list>
#include "Ref.h"
#include <functional>


template<class NODE>
class RList : public Ref
{
protected:
	RList();
public:
	typedef std::function<void(NODE*, bool&outDel, bool&outEnd)> FUNC_ListErgodic;
	virtual ~RList();
public:
	STATIC_NODE_CREATE(RList);
	bool		init() { return true; };
	void		PushBack(NODE*node);
	void		PushFront(NODE*node);
	void		PopBack();
	void		PopFront();
	NODE*		Front();
	NODE*		Back();
	std::_List_iterator<std::_List_val<std::_List_simple_types<NODE*>>> Begin();
	std::_List_iterator<std::_List_val<std::_List_simple_types<NODE*>>> End();
	void		Insert(unsigned int index, NODE*node);

	void		removeFromTag(int tag);
	void		removeFromIndex(unsigned int index);
	void		removeNode(NODE*node);
	void		Clear();

	NODE*		getNodeAtTag(int tag);
	NODE*		getNodeAtIndex(int index);
	const std::list<NODE*>&	getListNode() { return m_listNode; };
	size_t		getCount() { return m_listNode.size(); };
	void		ergodicFunc(FUNC_ListErgodic func);
private:
	std::list<NODE*> m_listNode;
};

template<class NODE>
RList<NODE>::RList()
{
}

template<class NODE>
RList<NODE>::~RList()
{
	Clear();
}

template<class NODE>
void RList<NODE>::PushBack(NODE*node)
{
	m_listNode.push_back(node);
	node->retain();
}

template<class NODE>
void RList<NODE>::PushFront(NODE*node)
{
	m_listNode.push_front(node);
	node->retain();
}

template<class NODE>
void RList<NODE>::PopBack()
{
	if (m_listNode.size() == 0)
	{
		return;
	}
	NODE* node = m_listNode.back();
	m_listNode.pop_back();
	node->release();
}

template<class NODE>
void RList<NODE>::PopFront()
{
	if (m_listNode.size() == 0)
	{
		return;
	}
	NODE*node = m_listNode.front();
	m_listNode.pop_front();
	node->release();
}

template<class NODE>
NODE* RList<NODE>::Front()
{
	if (m_listNode.size() == 0)
	{
		return nullptr;
	}
	return m_listNode.front();
}

template<class NODE>
NODE* RList<NODE>::Back()
{
	if (m_listNode.size() == 0)
	{
		return nullptr;
	}
	return m_listNode.back();
}

template<class NODE>
std::_List_iterator<std::_List_val<std::_List_simple_types<NODE*>>> RList<NODE>::Begin()
{
	 
	return m_listNode.begin();
}

template<class NODE>
std::_List_iterator<std::_List_val<std::_List_simple_types<NODE*>>> RList<NODE>::End()
{
	return m_listNode.end();
}

template<class NODE>
void RList<NODE>::Insert(unsigned int index, NODE*node)
{
	std::list<NODE*>::iterator
	if (index >= m_listNode.size())
	{
		PushBack(node);
		return;
	}
	int i = 0;
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++i, ++it)
	{
		if (i == index)
		{
			m_listNode.insert(it, node);
			node->retain();
			return;
		}
	}
	assert(1);	//index是错的
}

template<class NODE>
void RList<NODE>::Clear()
{
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
	{
		(*it)->release();
	}
	m_listNode.clear();
}


template<class NODE>
NODE * RList<NODE>::getNodeAtTag(int tag)
{
	for (auto it : m_listNode)
	{
		if (it->getTag() == tag)
		{
			return it;
		}
	}
	return nullptr;
}


template<class NODE>
NODE * RList<NODE>::getNodeAtIndex(int index)
{
	int i = 0;
	for (auto it : m_listNode)
	{
		if (i == index)
		{
			return it;
		}
		++i;
	}
	return nullptr;
}

template<class NODE>
void RList<NODE>::removeFromTag(int tag)
{
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			(*it)->release();
			m_listNode.erase(it);
			return;
		}
	}
	assert(1);	//要移除的元素不存在
	return;
}

template<class NODE>
void RList<NODE>::removeFromIndex(unsigned int index)
{
	int i = 0;
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++i, ++it)
	{
		if (i == index)
		{
			(*it)->release();
			m_listNode.erase(it);
			return;
		}
	}
	assert(1);	//index是错的
	return;
}

template<class NODE>
void RList<NODE>::removeNode(NODE*node)
{
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
	{
		if (node == *it)
		{
			(*it)->release();
			m_listNode.erase(it);
			return;
		}
	}
	assert(1);	//要移除的元素不存在
}

template<class NODE>
void RList<NODE>::ergodicFunc(FUNC_ListErgodic func)
{
	bool isDelete = false;
	bool isEnd = false;
	for (std::_List_iterator<std::_List_val<std::_List_simple_types<NODE*>>> it = m_listNode.begin(); it != m_listNode.end();)
	{
		isDelete = false;
		func(*it, isDelete, isEnd);
		if (isDelete)
		{
			(*it)->release();
			it = m_listNode.erase(it);
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

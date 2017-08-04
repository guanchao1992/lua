#include "NodeList.h"
#include "..\manager\ObjectManager.h"
#include <assert.h>
#include "Node.h"


NodeList::NodeList()
{
}

NodeList::~NodeList()
{
	Clear();
}

void NodeList::PushBack(Node*node)
{
	m_listNode.push_back(node);
	node->retain();
}

void NodeList::PushFront(Node*node)
{
	m_listNode.push_front(node);
	node->retain();
}

void NodeList::PopBack()
{
	if (m_listNode.size() == 0)
	{
		return;
	}
	Node* node = m_listNode.back();
	m_listNode.pop_back();
	node->release();
}

void NodeList::PopFront()
{
	if (m_listNode.size() == 0)
	{
		return;
	}
	Node*node = m_listNode.front();
	m_listNode.pop_front();
	node->release();
}

Node* NodeList::Front()
{
	if (m_listNode.size() == 0)
	{
		return nullptr;
	}
	return m_listNode.front();
}

Node* NodeList::Back()
{
	if (m_listNode.size() == 0)
	{
		return nullptr;
	}
	return m_listNode.back();
}

void NodeList::Insert(unsigned int index, Node*node)
{
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

void NodeList::Clear()
{
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
	{
		(*it)->release();
	}
	m_listNode.clear();
}

Node * NodeList::getNodeAtIndex(int index)
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

Node* NodeList::removeFromTag(int tag)
{
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
	{
		if ((*it)->getTag() == tag)
		{
			Node * ret = (*it);
			(*it)->release();
			m_listNode.erase(it);
			return ret;
		}
	}
	assert(1);	//要移除的元素不存在
	return nullptr;
}

Node* NodeList::removeFromIndex(unsigned int index)
{
	int i = 0;
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++i, ++it)
	{
		if (i == index)
		{
			Node * ret = (*it);
			(*it)->release();
			m_listNode.erase(it);
			return ret;
		}
	}
	assert(1);	//index是错的
	return nullptr;
}

void NodeList::removeNode(Node*node)
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

static bool sortNode_order(Node*a, Node*b)
{
	if (a->getOrder() < b->getOrder())
	{
		return true;
	}
	return false;
}

void NodeList::sortNodeByOrder()
{
	m_listNode.sort(sortNode_order);
}

void NodeList::ergodicFunc(FUNC_ListErgodic func)
{
	bool isDelete = false;
	bool isEnd = false;
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
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

#include "NodeVector.h"
#include "..\manager\ObjectManager.h"
#include <assert.h>
#include "Node.h"


NodeVector::NodeVector()
{
}

NodeVector::~NodeVector()
{
	Clear();
}

void NodeVector::PushBack(Node*node)
{
	m_listNode.push_back(node);
	node->retain();
}

void NodeVector::PopBack()
{
	Node* node = m_listNode.back();
	m_listNode.pop_back();
	node->release();
}

void NodeVector::Insert(unsigned int index, Node*node)
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

void NodeVector::Clear()
{
	for (auto it = m_listNode.begin(); it != m_listNode.end(); ++it)
	{
		(*it)->release();
	}
	m_listNode.clear();
}

Node * NodeVector::getNodeAtIndex(int index)
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

Node* NodeVector::removeFromTag(int tag)
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

Node* NodeVector::removeFromIndex(unsigned int index)
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

void NodeVector::removeNode(Node*node)
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
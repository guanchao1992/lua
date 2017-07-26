#include "Node.h"
#include "..\manager\ObjectManager.h"
#include <assert.h>
#include "NodeList.h"


Node::Node()
	: m_parent(nullptr)
	, m_listChildren(nullptr)
	, m_tag(0)
	, m_order(100000)
{
	m_listChildren = NodeList::create();
	m_listChildren->retain();
}

Node::~Node()
{
	removeAllChild();
	m_listChildren->release();
}

void Node::addChild(Node*node, int order)
{
	if (node == nullptr || node->getParent() != nullptr)
	{
		assert(1);
		return;
	}
	if (node)
	{
		node->setParent(this);
		node->m_order = order;
		m_listChildren->PushBack(node);
	}
}

Node * Node::getChildFromTag(int tag)
{
	for (auto it : m_listChildren->getListNode())
	{
		if (it->getTag() == tag)
		{
			return it;
		}
	}
	return nullptr;
}

void Node::removeFromTag(int tag)
{
	auto node = m_listChildren->removeFromTag(tag);
	if (node)
	{
		node->setParent(nullptr);
	}
}

void Node::removeChild(Node*node)
{
	if (node->getParent() != this)
	{
		return;
	}
	node->setParent(nullptr);
	m_listChildren->removeNode(node);
}

void Node::removeAllChild()
{
	for (auto it : m_listChildren->getListNode())
	{
		it->setParent(nullptr);
	}
	m_listChildren->Clear();
}


void Node::setOrder(int order)
{
	if (m_order != order)
	{
		m_order = order;
	}
	m_listChildren->sortNodeByOrder();
}

#pragma once
#include <list>
#include "Ref.h"

class Node;
class NodeList : public Ref
{
protected:
	NodeList();
public:
	virtual ~NodeList();
public:
	STATIC_NODE_CREATE(NodeList);
	bool		init() { return true; };
	void		PushBack(Node*node);
	void		PushFront(Node*node);
	void		PopBack();
	void		PopFront();
	void		Insert(unsigned int index, Node*node);

	Node*		removeFromTag(int tag);
	Node*		removeFromIndex(unsigned int index);
	void		removeNode(Node*node);
	void		Clear();

	Node*		getNodeAtIndex(int index);
	const std::list<Node*>&	getListNode() { return m_listNode; };
	void		sortNodeByOrder();
private:
	std::list<Node*> m_listNode;
};


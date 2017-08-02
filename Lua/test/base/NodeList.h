#pragma once
#include <list>
#include "Ref.h"

class Node;
//这个是会自动排序
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
	size_t		getCount() { return m_listNode.size(); };
private:
	std::list<Node*> m_listNode;
};


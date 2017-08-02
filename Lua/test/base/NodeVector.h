#pragma once
#include <vector>
#include "Ref.h"

class Node;
class NodeVector : public Ref
{
protected:
	NodeVector();
public:
	virtual ~NodeVector();
public:
	STATIC_NODE_CREATE(NodeVector);
	bool		init() { return true; };
	void		PushBack(Node*node);
	void		PopBack();
	void		Insert(unsigned int index, Node*node);

	Node*		removeFromTag(int tag);
	Node*		removeFromIndex(unsigned int index);
	void		removeNode(Node*node);
	void		Clear();

	Node*		getNodeAtIndex(int index);
	const std::vector<Node*>&	getListNode() { return m_listNode; };
	size_t		getCount() { return m_listNode.size(); };
private:
	std::vector<Node*> m_listNode;
};


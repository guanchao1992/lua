#pragma once
#include "Ref.h"


class NodeList;
class Node : public Ref
{
protected:
	Node();
public:
	~Node();
public:
	STATIC_NODE_CREATE(Node);
	bool		init() { return true; };
	void		addChild(Node*node, int order = 0);

	void		setTag(int tag) { m_tag = tag; };
	int			getTag() { return m_tag; };

	Node*		getParent() { return m_parent; };
	NodeList*	getChildren() { return m_listChildren; };
	Node*		getChildFromTag(int tag);

	void		removeFromTag(int tag);
	void		removeChild(Node*node);
	void		removeAllChild();

	virtual void		setOrder(int order);
	virtual int			getOrder() { return m_order; };
	virtual void		render() {};
private:
	void		setParent(Node*node) { m_parent = node; };
private:
	NodeList*	m_listChildren;
	Node*		m_parent;
	int			m_tag;
	int			m_order;
};


#pragma once
#include "Ref.h"
#include "Position2D.h"


class NodeList;
class Node : public Ref
{
protected:
	Node();
public:
	virtual ~Node();
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
	void		removeFromeParent();

	virtual void		setOrder(int order);
	virtual int			getOrder() { return m_order; };
	virtual void		render() {};
	virtual void		redraw() { m_bRedraw = false; };

	virtual void				setPosition(const Position2D&position);
	virtual const Position2D&	getPosition() { return m_positoin; };
	virtual void				setScale(float scale);
	virtual float				getScale() { return m_scale; };

	virtual void				setVisible(bool visible) { m_isVisible = visible; };
	virtual bool				isVisible() { return m_isVisible; };
protected:
	virtual Position2D			getSurePosition();
	bool						isRedraw();
	void						doRedraw() { m_bRedraw = true; };
private:
	void						setParent(Node*node) { m_parent = node; };
private:
	NodeList*	m_listChildren;
	Node*		m_parent;
	int			m_tag;
	int			m_order;
	Position2D	m_positoin;
	float		m_scale;
	bool		m_bRedraw;
	bool		m_isVisible;
};


#pragma once
#include "Ref.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Color4F.h"
#include "Matrix4.h"
#include <vector>
#include "RList.h"

class DrawBuffer;
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

	Node*		getParent() { return m_parent; };
	RList<Node>*getChildren() { return m_listChildren; };
	Node*		getChildFromTag(int tag);

	void		removeFromTag(int tag);
	void		removeChild(Node*node);
	void		removeAllChild();
	void		removeFromeParent();

	virtual void		render(const Matrix4& transform);
	virtual void		draw(const Matrix4& transform);
	virtual void		drawEnd();
	virtual void		renderThis(const Matrix4& transform);

	virtual void		setOrder(int order);
	virtual int			getOrder() { return m_order; };

	virtual void				setPosition(const Vector3&position);
	virtual void				setPosition(const Vector2&position);
	virtual const Vector3&		getPosition() { return m_positoin; };

	virtual void				setScaleX(float scale);
	virtual float				getScaleX() { return m_scale.x; };
	virtual void				setScaleY(float scale);
	virtual float				getScaleY() { return m_scale.y; };
	virtual void				setScaleZ(float scale);
	virtual float				getScaleZ() { return m_scale.z; };
	virtual void				setScale(float scale);
	virtual void				setScale(const Vector3& scale);
	virtual const Vector3&		getScale() { return m_scale; };

	virtual void				setRotateX(float rotate);
	virtual float				getRotateX() { return m_rotate.x; };
	virtual void				setRotateY(float rotate);
	virtual float				getRotateY() { return m_rotate.y; };
	virtual void				setRotateZ(float rotate);
	virtual float				getRotateZ() { return m_rotate.z; };
	virtual void				setRotate(const Vector3& rotate);
	virtual const Vector3&		getRotate() { return m_rotate; };

	virtual void				setVisible(bool visible) { m_isVisible = visible; };
	virtual bool				isVisible() { return m_isVisible; };

	virtual	void				clearBuffer();//清空绘制内容
protected:
	bool						isRedraw();
	void						doRedraw() { m_bRedraw = true; };

protected:
	virtual void				updateBuffer(const Matrix4& transform);
	Matrix4						getTransform(const Matrix4&transform);
private:
	void						setParent(Node*node) { m_parent = node; };
protected:
	std::vector<DrawBuffer*>		m_vecBuffer;
	RList<Node>*					m_listChildren;
	Node*							m_parent;
	int								m_order;		//越小，越在前面
	Vector3							m_positoin;
	Vector3							m_scale;
	Vector3							m_rotate;
	bool							m_bRedraw;
	bool							m_isVisible;
};


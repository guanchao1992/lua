#include "Node.h"
#include "..\manager\ObjectManager.h"
#include <assert.h>
#include "RList.h"
#include <wtypes.h>
#include "..\draw\DrawBuffer.h"
#include "MyMath.h"


Node::Node()
	: m_parent(nullptr)
	, m_listChildren(nullptr)
	, m_order(100000)
	, m_positoin(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_rotate(0.f, 0.f, 0.f)
	, m_bRedraw(false)
	, m_isVisible(true)
{
	m_listChildren = RList<Node>::create();
	m_listChildren->retain();
}

Node::~Node()
{
	clearBuffer();
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
		m_bRedraw = true;
	}
}

Node * Node::getChildFromTag(int tag)
{
	for (auto it : m_listChildren->getListNode())
	{
		if (it->getTag() == tag)
		{
			return (Node*)it;
		}
	}
	return nullptr;
}

void Node::removeFromTag(int tag)
{
	auto node = m_listChildren->getNodeAtTag(tag);
	if (node)
	{
		node->removeFromeParent();
		m_bRedraw = true;
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
	m_bRedraw = true;
}

void Node::removeAllChild()
{
	for (auto it : m_listChildren->getListNode())
	{
		((Node*)it)->setParent(nullptr);
	}
	m_listChildren->Clear();
	m_bRedraw = true;
}

void Node::removeFromeParent()
{
	if (m_parent == nullptr)
	{
		return;
	}
	m_parent->removeChild(this);
	m_parent = nullptr;
	m_bRedraw = true;
}

void Node::render(const Matrix4& transform)
{
	if (isVisible() == false)
	{
		return;
	}
	Matrix4 newTransform = getTransform(transform);
	draw(newTransform);
	//在底下的
	for (Ref* it : getChildren()->getListNode())
	{
		Node* node = (Node*)it;
		if (node->getOrder() < 0)
		{
			node->render(newTransform);
		}
	}
	renderThis(newTransform);
	for (Ref* it : getChildren()->getListNode())
	{
		Node* node = (Node*)it;
		if (node->getOrder() <= 0)
		{
			node->render(newTransform);
		}
	}
	drawEnd();
}

void Node::drawEnd()
{
	m_bRedraw = false;
}

void Node::clearBuffer()
{
	for (auto it : m_vecBuffer)
	{
		delete it;
	}
	m_vecBuffer.clear();
}

void Node::renderThis(const Matrix4& transform)
{
	for (auto it : m_vecBuffer)
	{
		it->render();
	}
}

void Node::draw(const Matrix4& transform)
{
	if (m_bRedraw)
	{
		updateBuffer(transform);
	}
}

static bool sortNode_order(Node*a, Node*b)
{
	if (a->getOrder() < b->getOrder())
	{
		return true;
	}
	return false;
}

void Node::setOrder(int order)
{
	if (m_order == order) {
		return;
	}
	m_order = order;

	//std::sort(m_listChildren->Begin(), m_listChildren->End(), sortNode_order);
	m_bRedraw = true;
}

void Node::setPosition(const Vector3&position)
{
	if (m_positoin == position) {
		return;
	}
	m_positoin = position;
	m_bRedraw = true;
};

void Node::setPosition(const Vector2&position)
{
	if (m_positoin.x == position.x && m_positoin.y == position.y) {
		return;
	}
	m_positoin.x = position.x;
	m_positoin.y = position.y;
	m_bRedraw = true;
}

void Node::setScaleX(float scale)
{
	if (m_scale.x == scale) {
		return;
	}
	m_scale.x = scale;
	m_bRedraw = true;
}

void Node::setScaleY(float scale)
{
	if (m_scale.y == scale) {
		return;
	}
	m_scale.y = scale;
	m_bRedraw = true;
}

void Node::setScaleZ(float scale)
{
	if (m_scale.z == scale) {
		return;
	}
	m_scale.z = scale;
	m_bRedraw = true;
}

void Node::setScale(float scale)
{
	if (m_scale.x == scale &&m_scale.y == scale &&m_scale.z == scale) {
		return;
	}
	m_scale.x = m_scale.y = m_scale.z = scale;
	m_bRedraw = true;
}

void Node::setScale(const Vector3& scale)
{
	if (m_scale == scale) {
		return;
	}
	m_scale = scale;
	m_bRedraw = true;
}

void Node::setRotateX(float rotate)
{
	if (m_rotate.x == rotate) {
		return;
	}
	m_rotate.x = rotate;
	m_bRedraw = true;
}

void Node::setRotateY(float rotate)
{
	if (m_rotate.y == rotate) {
		return;
	}
	m_rotate.y = rotate;
	m_bRedraw = true;
}

void Node::setRotateZ(float rotate)
{
	if (m_rotate.z == rotate) {
		return;
	}
	m_rotate.z = rotate;
	m_bRedraw = true;
}

void Node::setRotate(const Vector3& rotate)
{
	if (m_rotate == rotate) {
		return;
	}
	m_rotate = rotate;
	m_bRedraw = true;
}

bool Node::isRedraw()
{
	if (m_bRedraw)
	{
		return true;
	}
	if (getParent() && getParent()->isRedraw())
	{
		return true;
	}
	return false;
}

Matrix4	Node::getTransform(const Matrix4&transform)
{

	Matrix4 newTransform = transform;
	//平移
	Matrix4 mv;
	mv.setColumn(3, m_positoin);
	newTransform = newTransform.multiply(mv);

	//旋转
	Vector3 radian = m_rotate.DegreeRadian();

	const float ti = radian.x;
	const float tj = radian.y;
	const float th = radian.z;

	const float ci = cosf(ti);
	const float cj = cosf(tj);
	const float ch = cosf(th);

	const float si = sinf(ti);
	const float sj = sinf(tj);
	const float sh = sinf(th);

	const float cc = ci * ch;
	const float cs = ci * sh;
	const float sc = si * ch;
	const float ss = si * sh;

	Matrix4 mr;
	mr.set(cj*ch, sj*sc - cs, sj*cc + ss, 0,
		cj*sh, sj*ss + cc, sj*cs - sc, 0,
		-sj, cj*si, cj*ci, 0,
		0, 0, 0, 1);
	newTransform = newTransform.multiply(mr);

	//缩放
	Matrix4 ms(m_scale.x, 0.f, 0.f, 0.f,
		0.f, m_scale.y, 0.f, 0.f,
		0.f, 0.f, m_scale.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
	newTransform = newTransform.multiply(ms);

	return newTransform;
}

void Node::updateBuffer(const Matrix4& transform)
{
	for (auto &it : m_vecBuffer)
	{
		it->updateBuffer(transform);
	}
}


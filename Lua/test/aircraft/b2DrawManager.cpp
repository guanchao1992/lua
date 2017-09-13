#include "b2DrawManager.h"
#include "..\draw\DrawLayout.h"
#include "..\draw\DrawNode.h"
#include "..\manager\DrawManager.h"

SingletonClaseCpp(b2DrawManager);


b2DrawManager::b2DrawManager()
{
}


b2DrawManager::~b2DrawManager()
{
}

void b2DrawManager::Init()
{
	m_drawLayout = DrawLayout::create(-1000);
	m_drawNode = DrawNode::create();
	m_drawLayout->addChild(m_drawNode, 0);
	m_drawLayout->retain();
	DrawManager::getInstance()->addLayout(m_drawLayout, "b2DrawManager");
}

void b2DrawManager::Cleanup()
{
	m_drawLayout->removeFromeParent();
	m_drawLayout->release();
}

void b2DrawManager::ClearDraw()
{
	m_drawNode->clearBuffer();
}

void b2DrawManager::DrawPolygon(const Vector2* vertices, int32 vertexCount, const b2Color& color)
{
	std::vector<Vector3> vec;
	for (int i = 0; i < vertexCount; ++i)
	{
		vec.push_back(Vector3(vertices[i]));
	}
	m_drawNode->DrawPolygon(vec, Color4F(color.r, color.g, color.b, color.a));
}

void b2DrawManager::DrawSolidPolygon(const Vector2* vertices, int32 vertexCount, const b2Color& color)
{
	std::vector<Vector3> vec;
	for (int i = 0; i < vertexCount; ++i)
	{
		vec.push_back(Vector3(vertices[i]));
	}
	m_drawNode->DrawSolidPolygon(vec, Color4F(color.r, color.g, color.b, color.a));
}

void b2DrawManager::DrawCircle(const Vector2& center, float32 radius, const b2Color& color)
{
	m_drawNode->DrawCircle(Vector3(center), radius, Color4F(color.r, color.g, color.b, color.a));
}

void b2DrawManager::DrawSolidCircle(const Vector2& center, float32 radius, const Vector2& axis, const b2Color& color)
{
	m_drawNode->DrawSolidCircle(Vector3(center), radius, Color4F(color.r, color.g, color.b, color.a));
}

void b2DrawManager::DrawSegment(const Vector2& p1, const Vector2& p2, const b2Color& color)
{

}

void b2DrawManager::DrawTransform(const b2Transform& xf)
{

}

void b2DrawManager::DrawPoint(const Vector2& p, float32 size, const b2Color& color)
{

}

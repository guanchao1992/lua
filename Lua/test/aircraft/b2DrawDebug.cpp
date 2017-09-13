#include "b2DrawDebug.h"
#include "../draw/DrawNode.h"
#include "../manager/DrawManager.h"
#include "../draw/DrawLayout.h"
#include "b2DrawManager.h"

b2DrawDebug::b2DrawDebug()
{
	
}

b2DrawDebug:: ~b2DrawDebug()
{
}

void b2DrawDebug::ClearAllDraw()
{
	b2DrawManager::getInstance()->ClearDraw();
}

void b2DrawDebug::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	std::vector<Vector2> vec;
	for (int i = 0; i < vertexCount; ++i)
	{
		vec.push_back(Vector2(vertices[i].x* BOX2D_LENTH_RATIO, vertices[i].y * BOX2D_LENTH_RATIO));
	}
	b2DrawManager::getInstance()->DrawPolygon(vec.data(),vertexCount,color);
}

void b2DrawDebug::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	std::vector<Vector2> vec;
	for (int i = 0; i < vertexCount; ++i)
	{
		vec.push_back(Vector2(vertices[i].x* BOX2D_LENTH_RATIO, vertices[i].y * BOX2D_LENTH_RATIO));
	}
	b2DrawManager::getInstance()->DrawSolidPolygon(vec.data(), vertexCount, color);
}

void b2DrawDebug::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	b2DrawManager::getInstance()->DrawCircle(Vector2(center.x * BOX2D_LENTH_RATIO, center.y * BOX2D_LENTH_RATIO), radius* BOX2D_LENTH_RATIO, color);
}

void b2DrawDebug::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	b2DrawManager::getInstance()->DrawSolidCircle(Vector2(center.x * BOX2D_LENTH_RATIO, center.y * BOX2D_LENTH_RATIO), radius* BOX2D_LENTH_RATIO, Vector2(0, 10), color);
}

void b2DrawDebug::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
}
void b2DrawDebug::DrawTransform(const b2Transform& xf)
{
}
void b2DrawDebug::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
}

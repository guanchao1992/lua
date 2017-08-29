#include "DrawNode.h"
#include "..\manager\DrawManager.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

using namespace DirectX;

DrawNode::DrawNode()
{
}

DrawNode::~DrawNode()
{
}

bool DrawNode::init()
{
	//DrawImage("2", Rect2D(-100, 0, 100, 100));
	//DrawImage("bg", Rect2D(0, -100, 100, 100));
	//DrawImage("redbox", Rect2D(10, 100, 100, 100));
	return true;
}

void DrawNode::DrawImage(const std::string&imageName, const Rect2D&rect)
{
	TextureRectBuffer * db = new TextureRectBuffer(imageName, rect);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawLine(const Vector2&pos1, const Vector2&pos2, const Color4F&color)
{
	DrawLineBuffer *db = new DrawLineBuffer(Vector3(pos1.x, pos1.y, 0), Vector3(pos2.x, pos2.y, 0), color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawLine(const Vector3&pos1, const Vector3&pos2, const Color4F&color)
{
	DrawLineBuffer *db = new DrawLineBuffer(pos1, pos2, color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawPolygon(const std::vector<Vector3>& vec, const Color4F&color)
{
	DrawPolygonBuffer *db = new DrawPolygonBuffer(vec, color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawSolidPolygon(const std::vector<Vector3>& vec, const Color4F&color)
{
	DrawSolidPolygonBuffer *db = new DrawSolidPolygonBuffer(vec, color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawRect(const Rect2D&rect, const Color4F&color)
{
	DrawRectBuffer *db = new DrawRectBuffer(rect, color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawSolidRect(const Rect2D&rect, const Color4F&color)
{
	//DrawSolidRectBuffer *db = new DrawSolidRectBuffer(rect, color);
	//DrawRectBuffer *db2 = new DrawRectBuffer(rect, color);
	std::vector<Vector3> vec;
	vec.push_back(Vector3(rect.getOrigin()));
	vec.push_back(Vector3(rect.getOrigin() + Vector2(0, rect.getHeight())));
	vec.push_back(Vector3(rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight())));
	vec.push_back(Vector3(rect.getOrigin() + Vector2(rect.getWidth(), 0)));
	DrawSolidPolygonBuffer *db = new DrawSolidPolygonBuffer(vec, color);
	DrawPolygonBuffer *db2 = new DrawPolygonBuffer(vec, Color4F(0xff0000ff));
	m_vecBuffer.push_back(db);
	m_vecBuffer.push_back(db2);
	doRedraw();
}

void DrawNode::DrawCircle(const Vector3&pos, float radius, const Color4F&color)
{
	DrawCircleBuffer *db = new DrawCircleBuffer(pos, radius, color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawSolidCircle(const Vector3&pos, float radius, const Color4F&color)
{
	DrawSolidCircleBuffer *db = new DrawSolidCircleBuffer(pos, radius, color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

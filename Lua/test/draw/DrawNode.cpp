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
	clear();
}

bool DrawNode::init()
{
	//DrawImage("2", Rect2D(-100, 0, 100, 100));
	//DrawImage("bg", Rect2D(0, -100, 100, 100));
	//DrawImage("redbox", Rect2D(10, 100, 100, 100));
	return true;
}

void DrawNode::renderThis(const Matrix4& transform)
{
	for (auto it : m_vecBuffer)
	{
		it->render();
	}
}

void DrawNode::draw(const Matrix4& transform)
{
	if (m_bRedraw)
	{
		m_bRedraw = false;
		updateBuffer(transform);
	}
}

void DrawNode::clear()
{
	for (auto it:m_vecBuffer)
	{
		delete it;
	}
	m_vecBuffer.clear();
}

void DrawNode::DrawImage(const std::string&imageName, const Rect2D&rect)
{
	TextureRectBuffer * db = new TextureRectBuffer(imageName, rect);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawLine(const Vector2&pos1, const Vector2&pos2, const Color4F&color)
{
	DrawLineBuffer *db = new DrawLineBuffer(Vector3(pos1.x, pos1.y, 0), Vector3(pos2.x, pos2.y, 0));
	setColor(color);
	db->setColor(color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawRect(const Rect2D&rect, const Color4F&color)
{
	DrawRectBuffer *db = new DrawRectBuffer(rect);
	setColor(color);
	db->setColor(color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::DrawSolidRect(const Rect2D&rect, const Color4F&color)
{
	DrawSolidRectBuffer *db = new DrawSolidRectBuffer(rect);
	setColor(color);
	db->setColor(color);
	m_vecBuffer.push_back(db);
	doRedraw();
}

void DrawNode::updateBuffer(const Matrix4& transform)
{
	for (auto &it : m_vecBuffer)
	{
		it->updateBuffer(transform);
	}
}

#include "DrawNode.h"
#include "..\manager\DrawManager.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "..\base\Size.h"
#include "..\manager\VideoManager.h"
#include "..\base\NodeList.h"
#include <D3DX11tex.h>
#include "..\manager\TextureManager.h"

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

void DrawNode::render()
{
	if (isVisible() == false)
	{
		return;
	}
	if (isRedraw())
	{
		redraw();
	}

	for (auto it : m_vecBuffer)
	{
		it->render();
	}
	for (Node* it : getChildren()->getListNode())
	{
		it->render();
	}
}

void DrawNode::redraw()
{
	__super::redraw();
	updateBuffer();
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
	updateBuffer();
}

void DrawNode::DrawRect(const Rect2D&rect, LONG32 rgb)
{
	DrawRectBuffer *db = new DrawRectBuffer(rect, rgb);
	m_vecBuffer.push_back(db);
	updateBuffer();
}

void DrawNode::updateBuffer()
{
	for (auto &it : m_vecBuffer)
	{
		it->updateBuffer(getSurePosition(), getScale());
	}
}

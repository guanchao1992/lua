#pragma once
#include "..\base\Node.h"
#include <vector>
#include "..\base\Rect2D.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "..\dxconfig.h"
#include "..\base\Image.h"
#include "..\draw\DrawBuffer.h"

using namespace DirectX;

class DrawNode :public Node
{
	STATIC_NODE_CREATE(DrawNode);
private:
	DrawNode();
public:
	virtual	~DrawNode();
public:
	bool	init();
	virtual void render();
	virtual void redraw();
	void	DrawImage(const std::string&imageName, const Rect2D&rect);
	void	DrawRect(const Rect2D&rect, LONG32 rgb);
	void	clear();

private:
	void	updateBuffer();
private:
	std::vector<DrawBuffer*>		m_vecBuffer;

};
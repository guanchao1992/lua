#pragma once
#include "..\base\Node.h"
#include <vector>
#include "..\base\Rect2D.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "..\dxconfig.h"
#include "..\base\Image.h"
#include "..\draw\DrawBuffer.h"
#include "..\base\Matrix4.h"

using namespace DirectX;

class DrawNode :public Node
{
	STATIC_NODE_CREATE(DrawNode);
protected:
	DrawNode();
public:
	virtual	~DrawNode();
public:
	bool	init();
	virtual void draw(const Matrix4& transform) override;
	//virtual void render(const Matrix4& transform) override;
	virtual void renderThis(const Matrix4& transform) override;
	void	DrawImage(const std::string&imageName, const Rect2D&rect);
	void	DrawLine(const Vector2&pos1, const Vector2&pos2, const Color4F&color);
	void	DrawLine(const Vector3&pos1, const Vector3&pos2, const Color4F&color);
	void	DrawRect(const Rect2D&rect, const Color4F&color);
	void	DrawSolidRect(const Rect2D&rect, const Color4F&color);
	void	clear();

protected:
	virtual void	updateBuffer(const Matrix4& transform) override;
protected:
	std::vector<DrawBuffer*>		m_vecBuffer;

};
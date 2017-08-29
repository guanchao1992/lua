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
	void	DrawImage(const std::string&imageName, const Rect2D&rect);
	void	DrawLine(const Vector2&pos1, const Vector2&pos2, const Color4F&color);
	void	DrawLine(const Vector3&pos1, const Vector3&pos2, const Color4F&color);
	void	DrawPolygon(const std::vector<Vector3>& vec, const Color4F&color);
	void	DrawSolidPolygon(const std::vector<Vector3>& vec, const Color4F&color);
	void	DrawRect(const Rect2D&rect, const Color4F&color);
	void	DrawSolidRect(const Rect2D&rect, const Color4F&color);
	void	DrawCircle(const Vector3&pos, float radius, const Color4F&color);
	void	DrawSolidCircle(const Vector3&pos, float radius, const Color4F&color);

};
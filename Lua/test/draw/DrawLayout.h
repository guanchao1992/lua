#pragma once
#include "..\base\Node.h"

class ID3D11InputLayout;
class ID3D11VertexShader;
class ID3D11PixelShader;

//尽量不要单独创建
class DrawLayout :public Node
{
	STATIC_NODE_CREATE(DrawLayout);
private:
	DrawLayout();
public:
	virtual ~DrawLayout();
public:
	bool	init();

	void	render(const Matrix4& transform);
private:
	ID3D11InputLayout*					m_pDrawVertexLayout;
	int									m_order;
};
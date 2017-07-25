#pragma once
#include "..\base\Node.h"

class ID3D11InputLayout;
class ID3D11VertexShader;
class ID3D11PixelShader;

//������Ҫ��������
class DrawLayout :public Node
{
	STATIC_NODE_CREATE(DrawLayout);
private:
	DrawLayout();
public:
	~DrawLayout();
public:
	bool	init();

	void	render();
private:
	ID3D11InputLayout*					m_pDrawVertexLayout;
	int									m_order;
};
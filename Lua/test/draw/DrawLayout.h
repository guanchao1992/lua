#pragma once
#include "..\base\Node.h"

class ID3D11InputLayout;
class ID3D11VertexShader;
class ID3D11PixelShader;

//创建之后要显示必须通过addLayout添加到DrawManager里
class DrawLayout :public Node
{
	STATIC_NODE_CREATE(DrawLayout);
protected:
	DrawLayout();
public:
	virtual ~DrawLayout();
public:
	bool	init();

	void	render();
private:
	ID3D11InputLayout*					m_pDrawVertexLayout;
	int									m_order;
};
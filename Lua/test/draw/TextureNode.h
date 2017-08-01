#pragma once
#include "..\base\Node.h"
#include <vector>
#include "..\base\Rect2D.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "..\dxconfig.h"
#include "..\base\Image.h"

using namespace DirectX;

struct TextureNodeRectBuffer
{
	TextureNodeRectBuffer(const std::string&imageName, const Rect2D& rect, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT bindFlags = D3D11_BIND_VERTEX_BUFFER);
	~TextureNodeRectBuffer();
	bool				updateBuffer(const Position2D&surePos, float scale);
	void				render();
	Image*						m_image;
	ID3D11Buffer*				m_d3dBuffer;	
	SimpleVertexMain*			m_OriginalVertex;		//保存最初的定点集合
	SimpleVertexMain*			m_nowlVertex;		
	UINT						m_vertexSize;
	D3D11_USAGE					m_Usage;
	UINT						m_BindFlags;
	D3D11_PRIMITIVE_TOPOLOGY	m_primitiveTopology;	

};

class TextureNode :public Node
{
	STATIC_NODE_CREATE(TextureNode);
private:
	TextureNode();
public:
	virtual	~TextureNode();
public:
	bool	init();
	virtual void render();
	virtual void redraw();
	void	DrawRect(const Rect2D&rect);
	void	clear();

private:
	void	updateBuffer();
private:
	std::vector<TextureNodeRectBuffer*>		m_vecBuffer;

};
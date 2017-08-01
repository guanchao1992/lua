#pragma once
#include "..\base\Node.h"
#include <vector>
#include "..\base\Rect2D.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "..\dxconfig.h"
#include "..\base\Image.h"

using namespace DirectX;

struct TextureNodeBuffer
{
	TextureNodeBuffer()
		: m_d3dBuffer(nullptr)
		, m_OriginalVertex(nullptr)
		, m_vertexSize(0)
		, m_Usage(D3D11_USAGE_DEFAULT)
		, m_BindFlags(D3D11_BIND_VERTEX_BUFFER)
		, m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP)
	{}
	~TextureNodeBuffer()
	{
		if (m_d3dBuffer)
		{
			m_d3dBuffer->Release();
			m_d3dBuffer = nullptr;
		}
		if (m_OriginalVertex)
		{
			delete[] m_OriginalVertex;
			m_OriginalVertex = nullptr;
		}
	}
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
	bool	createBuffer(const SimpleVertexMain*vertex, UINT vertexSize, D3D11_USAGE usage, UINT bindFlags, ID3D11Buffer**outBuffer);
	void	updateBuffer();
private:
	std::vector<TextureNodeBuffer*>		m_vecBuffer;

};
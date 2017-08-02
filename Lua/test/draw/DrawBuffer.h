#pragma once
#include "..\base\Node.h"
#include <vector>
#include "..\base\Rect2D.h"
#include "..\dxconfig.h"
#include "..\base\Image.h"

using namespace DirectX;

struct DrawBuffer
{
	DrawBuffer(UINT vertexSize);
	~DrawBuffer();
	virtual void				render() = 0;
	virtual bool				updateBuffer(const Position2D&surePos, float scale);
protected:
	ID3D11Buffer*				m_d3dBuffer;
	SimpleVertexMain*			m_OriginalVertex;		//保存最初的定点集合
	SimpleVertexMain*			m_nowlVertex;
	UINT						m_vertexSize;
	D3D11_USAGE					m_Usage;
	UINT						m_BindFlags;
	D3D11_PRIMITIVE_TOPOLOGY	m_primitiveTopology;

};

struct DrawRectBuffer :public DrawBuffer
{
	DrawRectBuffer(const Rect2D& rect, unsigned long rgb);
	~DrawRectBuffer();
	//virtual bool				updateBuffer(const Position2D&surePos, float scale) override;
	virtual void				render() override;
protected:
};

struct TextureRectBuffer :public DrawBuffer
{
	TextureRectBuffer(const std::string&imageName, const Rect2D& rect);
	~TextureRectBuffer();
	//bool				updateBuffer(const Position2D&surePos, float scale);
	void				render();
protected:
	Image*						m_image;
};
#pragma once
#include "..\base\Node.h"
#include <vector>
#include "..\base\Rect2D.h"
#include "..\dxconfig.h"
#include "..\base\Image.h"
#include "..\base\Color4F.h"
#include "..\base\Matrix3.h"
#include "..\base\Matrix4.h"

using namespace DirectX;

struct DrawBuffer
{
	DrawBuffer(UINT vertexSize);
	~DrawBuffer();
	void						updateRender();
	bool						updateBuffer(const Matrix4&transform);
	virtual void				render();
	virtual bool				bindVertex();
	virtual bool				bindIndex();
	virtual void				setColor(const Color4F&color);
protected:
	ID3D11Buffer*				m_d3dVertexBuffer;
	ID3D11Buffer*				m_d3dIndexBuffer;
	SimpleVertexMain*			m_OriginalVertex;		//保存最初的定点集合
	SimpleVertexMain*			m_nowlVertex;
	UINT						m_vertexSize;
	D3D11_USAGE					m_Usage;
};

struct DrawLineBuffer : public DrawBuffer
{
	DrawLineBuffer(const Vector3&pos1, const Vector3&pos2);
	~DrawLineBuffer();
	virtual void render() override;
};

struct DrawRectBuffer : public DrawBuffer
{
	DrawRectBuffer(const Rect2D& rect);
	~DrawRectBuffer();
	virtual void				render() override;
protected:
};

struct DrawSolidRectBuffer : public DrawBuffer
{
	DrawSolidRectBuffer(const Rect2D& rect);
	~DrawSolidRectBuffer();
	virtual bool				bindIndex() override;
	virtual void				render() override;
protected:
};

struct TextureRectBuffer : public DrawBuffer
{
	TextureRectBuffer(const std::string&imageName, const Rect2D& rect);
	~TextureRectBuffer();
	virtual void				render() override;
protected:
	Image*						m_image;
};
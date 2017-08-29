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

class DrawBuffer
{
public:
	DrawBuffer(UINT vertexSize, const Color4F&color = Color4F(1, 1, 1, 1));
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
	//SimpleVertexMain*			m_OriginalVertex;		//保存最初的定点集合
	Vector3*					m_OriginalVector3;
	SimpleVertexMain*			m_nowlVertex;
	UINT						m_vertexSize;
	D3D11_USAGE					m_Usage;
	D3D11_PRIMITIVE_TOPOLOGY	m_primitiveTopology;
};

class DrawSolidBuffer : public DrawBuffer
{
protected:
	DrawSolidBuffer(UINT vertexSize, const Color4F&color = Color4F(1, 1, 1, 1));
	~DrawSolidBuffer() {};
public:
	virtual bool				bindIndex() override;
	virtual void				render() override;
};

class DrawLineBuffer : public DrawBuffer
{
public:
	DrawLineBuffer(const Vector3&pos1, const Vector3&pos2, const Color4F&color);
	~DrawLineBuffer();
};

class DrawRectBuffer : public DrawBuffer
{
public:
	DrawRectBuffer(const Rect2D& rect, const Color4F&color);
	~DrawRectBuffer();
protected:
};

class DrawSolidRectBuffer : public DrawSolidBuffer
{
public:
	DrawSolidRectBuffer(const Rect2D& rect, const Color4F&color);
	~DrawSolidRectBuffer();
protected:
};

class DrawPolygonBuffer : public DrawBuffer
{
public:
	DrawPolygonBuffer(const std::vector<Vector3>&vertices, const Color4F&color);
	~DrawPolygonBuffer() {};
};

class DrawSolidPolygonBuffer : public DrawSolidBuffer
{
public:
	DrawSolidPolygonBuffer(const std::vector<Vector3>&vertices, const Color4F&color);
	~DrawSolidPolygonBuffer() {};
};

class DrawCircleBuffer : public DrawBuffer
{
public:
	DrawCircleBuffer(const Vector3&center, float radius, const Color4F&color);
	~DrawCircleBuffer() {};
};

class DrawSolidCircleBuffer : public DrawSolidBuffer
{
public:
	DrawSolidCircleBuffer(const Vector3&center, float radius, const Color4F&color);
	~DrawSolidCircleBuffer() {};
};


class TextureRectBuffer : public DrawBuffer
{
public:
	TextureRectBuffer(const std::string&imageName, const Rect2D& rect);
	~TextureRectBuffer();
	virtual void				render() override;
protected:
	Image*						m_image;
};
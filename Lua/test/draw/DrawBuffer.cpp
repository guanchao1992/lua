#include "DrawBuffer.h"
#include "..\manager\DrawManager.h"
#include "..\manager\VideoManager.h"
#include "..\manager\TextureManager.h"


DrawBuffer::DrawBuffer(UINT vertexSize,const Color4F&color)
	: m_d3dVertexBuffer(nullptr)
	, m_d3dIndexBuffer(nullptr)
	, m_OriginalVector3(nullptr)
	, m_nowlVertex(nullptr)
	, m_vertexSize(vertexSize)
	, m_Usage()
	, m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP)
{
	m_OriginalVector3 = new Vector3[m_vertexSize];
	m_nowlVertex = new SimpleVertexMain[m_vertexSize];
	setColor(color);
}

DrawBuffer::~DrawBuffer()
{
	if (m_d3dVertexBuffer)
	{
		m_d3dVertexBuffer->Release();
		m_d3dVertexBuffer = nullptr;
	}
	if (m_d3dIndexBuffer)
	{
		m_d3dIndexBuffer->Release();
		m_d3dIndexBuffer = nullptr;
	}
	if (m_OriginalVector3)
	{
		delete[] m_OriginalVector3;
		m_OriginalVector3 = nullptr;
	}
	if (m_nowlVertex)
	{
		delete[] m_nowlVertex;
		m_nowlVertex = nullptr;
	}
}

bool DrawBuffer::updateBuffer(const Matrix4&transform)
{
	for (size_t i = 0; i < m_vertexSize; i++)
	{
		Vector3 posEnd = transform.transformPosition(m_OriginalVector3[i]);
		m_nowlVertex[i].Pos.x = posEnd.x;
		m_nowlVertex[i].Pos.y = posEnd.y;
		m_nowlVertex[i].Pos.z = posEnd.z;
	}
	bindVertex();
	bindIndex();
	return true;
}

bool DrawBuffer::bindVertex()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertexMain) * m_vertexSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_nowlVertex;
	HRESULT result = S_OK;

	if (m_d3dVertexBuffer)
	{
		m_d3dVertexBuffer->Release();
		m_d3dVertexBuffer = nullptr;
	}
	if (m_d3dVertexBuffer == nullptr)
	{
		result = getD3DDevice()->CreateBuffer(&bd, &InitData, &m_d3dVertexBuffer);
	}
	if (FAILED(result))
	{
		OutputDebugStringA("error: void DrawBuffer::bindVertex()");
		return false;
	}
	return true;
}

bool DrawBuffer::bindIndex()
{
	return true;
}

void DrawBuffer::updateRender()
{
	DrawManager::getInstance()->setShaderType(ShaderType::Shader_Normal);

	UINT stride = sizeof(SimpleVertexMain);
	UINT offset = 0;
	getD3DContext()->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &stride, &offset);

	if (m_d3dIndexBuffer)
	{
		getD3DContext()->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}
}

void DrawBuffer::render()
{
	updateRender();
	getD3DContext()->IASetPrimitiveTopology(m_primitiveTopology);
	getD3DContext()->Draw(m_vertexSize, 0);
}

void DrawBuffer::setColor(const Color4F&color)
{
	for (int i=0;i<m_vertexSize;++i)
	{
		m_nowlVertex[i].Color.x = color.r;
		m_nowlVertex[i].Color.y = color.g;
		m_nowlVertex[i].Color.z = color.b;
		m_nowlVertex[i].Color.w = color.w;
	}
}

//*********************------- DrawLineBuffer ------***********************//

DrawLineBuffer::DrawLineBuffer(const Vector3&pos1, const Vector3&pos2, const Color4F&color)
	:DrawBuffer(2, color)
{
	m_OriginalVector3[0] = pos1;
	m_OriginalVector3[1] = pos2;
}

DrawLineBuffer::~DrawLineBuffer()
{
}

//*********************------- DrawSolidBuffer ------***********************//

DrawSolidBuffer::DrawSolidBuffer(UINT vertexSize, const Color4F&color)
	:DrawBuffer(vertexSize, color)
{ 
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

bool DrawSolidBuffer::bindIndex()
{
	if (m_d3dIndexBuffer)
	{
		return true;
	}
	int indicesSize = 3 * (m_vertexSize - 2);
	UINT *indices = new UINT[indicesSize];
	for (int i = 0; i < m_vertexSize - 2; ++i)
	{
		indices[i * 3] = 0;
		indices[i * 3 + 1] = i + 1;
		indices[i * 3 + 2] = i + 2;
	}

	// 要创建的索引的描述
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT) * indicesSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	// 设定用于初始化索引缓冲的数据
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	HRESULT result = S_OK;
	// 创建索引缓冲
	if (m_d3dIndexBuffer == nullptr)
	{
		result = getD3DDevice()->CreateBuffer(&bd, &InitData, &m_d3dIndexBuffer);

	}
	delete[]indices;
	if (FAILED(result))
	{
		OutputDebugStringA("error: void DrawBuffer::bindIndex()");
		return false;
	}
	return true;
}

void DrawSolidBuffer::render()
{
	updateRender();
	getD3DContext()->IASetPrimitiveTopology(m_primitiveTopology);
	getD3DContext()->DrawIndexed(3 * (m_vertexSize - 2), 0, 0);
}

//*********************------- DrawRectBuffer ------***********************//
DrawRectBuffer::DrawRectBuffer(const Rect2D& rect, const Color4F&color)
	:DrawBuffer(5, color)
{
	m_OriginalVector3[0] = rect.getOrigin();
	m_OriginalVector3[1] = rect.getOrigin() + Vector2(0, rect.getHeight());
	m_OriginalVector3[2] = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());
	m_OriginalVector3[3] = rect.getOrigin() + Vector2(rect.getWidth(), 0);
	m_OriginalVector3[4] = m_OriginalVector3[0];

	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
}

DrawRectBuffer::~DrawRectBuffer()
{}

//*********************------- DrawSolidRectBuffer ------***********************//

DrawSolidRectBuffer::DrawSolidRectBuffer(const Rect2D& rect, const Color4F&color)
	:DrawSolidBuffer(4, color)
{
	m_OriginalVector3[0] = rect.getOrigin();
	m_OriginalVector3[1] = rect.getOrigin() + Vector2(0, rect.getHeight());
	m_OriginalVector3[2] = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());
	m_OriginalVector3[3] = rect.getOrigin() + Vector2(rect.getWidth(), 0);
}

DrawSolidRectBuffer::~DrawSolidRectBuffer()
{}

//*********************------- DrawPolygonBuffer ------***********************//

DrawPolygonBuffer::DrawPolygonBuffer(const std::vector<Vector3>&vertices, const Color4F&color)
	:DrawBuffer(vertices.size() + 1, color)
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		m_OriginalVector3[i] = vertices[i];
	}
	m_OriginalVector3[vertices.size()] = m_OriginalVector3[0];
}

//*********************------- DrawSolidPolygonBuffer ------***********************//

DrawSolidPolygonBuffer::DrawSolidPolygonBuffer(const std::vector<Vector3>&vertices, const Color4F&color)
	:DrawSolidBuffer(vertices.size(), color)
{
	for (int i = 0; i < vertices.size(); ++i)
	{
		m_OriginalVector3[i] = vertices[i];
	}
}

//*********************------- DrawCircleBuffer ------***********************//

DrawCircleBuffer::DrawCircleBuffer(const Vector3&center, float radius, const Color4F&color)
	:DrawBuffer(13, color)
{
	float a = XM_2PI / 12;
	for (int i = 0; i < 12; ++i)
	{
		m_OriginalVector3[i] = Vector3(center.x + radius*sin(a*i), center.y + radius*cos(a*i), 0);
	}
	m_OriginalVector3[12] = m_OriginalVector3[0];
}

//*********************------- DrawSolidCircleBuffer ------***********************//

DrawSolidCircleBuffer::DrawSolidCircleBuffer(const Vector3&center, float radius, const Color4F&color)
	:DrawSolidBuffer(12, color)
{
	float a = XM_2PI / 12;
	for (int i = 0; i < 12; ++i)
	{
		m_OriginalVector3[i] = Vector3(center.x + radius*sin(a*i), center.y + radius*cos(a*i), 0);
	}
}

//*********************------- TextureRectBuffer ------***********************//

TextureRectBuffer::TextureRectBuffer(const std::string&imageName, const Rect2D& rect)
	:DrawBuffer(5)
{
	m_image = TextureManager::getInstance()->loadImage(imageName);
	if (m_image == nullptr)
	{
		return;
	}
	m_image->retain();

	m_OriginalVector3[0] = rect.getOrigin();
	m_OriginalVector3[1] = rect.getOrigin() + Vector2(0, rect.getHeight());
	m_OriginalVector3[2] = rect.getOrigin() + Vector2(rect.getWidth(), 0);
	m_OriginalVector3[3] = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());

	const Rect2D& textrect = m_image->getRect();
	m_nowlVertex[0].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY());
	m_nowlVertex[1].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY() + textrect.getHeight());
	m_nowlVertex[2].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY());
	m_nowlVertex[3].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY() + textrect.getHeight());
}

TextureRectBuffer::~TextureRectBuffer()
{
	if (m_image)
	{
		m_image->release();
		m_image = nullptr;
	}
}

void TextureRectBuffer::render()
{
	DrawManager::getInstance()->setShaderType(ShaderType::Shader_Texture);
	TextureBuffer*tb = m_image->getTexture();
	tb->PSSetView(getD3DContext());
	getD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	getD3DContext()->Draw(m_vertexSize, 0);
}

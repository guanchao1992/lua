#include "DrawBuffer.h"
#include "..\manager\DrawManager.h"
#include "..\manager\VideoManager.h"
#include "..\manager\TextureManager.h"


DrawBuffer::DrawBuffer(UINT vertexSize)
	: m_d3dVertexBuffer(nullptr)
	, m_d3dIndexBuffer(nullptr)
	, m_OriginalVertex(nullptr)
	, m_nowlVertex(nullptr)
	, m_vertexSize(vertexSize)
	, m_Usage()
{
	m_OriginalVertex = new SimpleVertexMain[m_vertexSize];
	m_nowlVertex = new SimpleVertexMain[m_vertexSize];
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
	if (m_OriginalVertex)
	{
		delete[] m_OriginalVertex;
		m_OriginalVertex = nullptr;
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
		Vector3 pos(m_OriginalVertex[i].Pos.x, m_OriginalVertex[i].Pos.y, m_OriginalVertex[i].Pos.z);
		Vector3 posEnd = transform.transformPosition(pos);

		m_nowlVertex[i] = m_OriginalVertex[i];
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
	getD3DContext()->Draw(m_vertexSize, 0);
}

void DrawBuffer::setColor(const Color4F&color)
{
	for (int i=0;i<m_vertexSize;++i)
	{
		m_OriginalVertex[i].Color.x = color.r;
		m_OriginalVertex[i].Color.y = color.g;
		m_OriginalVertex[i].Color.z = color.b;
		m_OriginalVertex[i].Color.w = color.w;
	}
}

//*********************------- DrawLineBuffer ------***********************//

DrawLineBuffer::DrawLineBuffer(const Vector3&pos1, const Vector3&pos2)
	:DrawBuffer(2)
{
	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.x, pos1.y, pos1.z, 1.3f);
	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.x, pos2.y, pos2.z, 1.5f);

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
}

DrawLineBuffer::~DrawLineBuffer()
{
}

void DrawLineBuffer::render()
{
	updateRender();
	getD3DContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	getD3DContext()->Draw(m_vertexSize, 0);
}

//*********************------- DrawRectBuffer ------***********************//

DrawRectBuffer::DrawRectBuffer(const Rect2D& rect)
	:DrawBuffer(5)
{
	Vector2 pos1 = rect.getOrigin();
	Vector2 pos2 = rect.getOrigin() + Vector2(0, rect.getHeight());
	Vector2 pos3 = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());
	Vector2 pos4 = rect.getOrigin() + Vector2(rect.getWidth(), 0);

	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.x, pos1.y, 1.0f, 1.3f);
	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.x, pos2.y, 1.0f, 1.5f);
	m_OriginalVertex[2].Pos = XMFLOAT4(pos3.x, pos3.y, 1.0f, 1.1f);
	m_OriginalVertex[3].Pos = XMFLOAT4(pos4.x, pos4.y, 1.0f, 1.5f);
	m_OriginalVertex[4] = m_OriginalVertex[0];

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
}

DrawRectBuffer::~DrawRectBuffer()
{}

void DrawRectBuffer::render()
{
	updateRender();
	getD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	getD3DContext()->Draw(m_vertexSize, 0);
}


//*********************------- DrawSolidRectBuffer ------***********************//

DrawSolidRectBuffer::DrawSolidRectBuffer(const Rect2D& rect)
	:DrawBuffer(4)
{
	Vector2 pos1 = rect.getOrigin();
	Vector2 pos2 = rect.getOrigin() + Vector2(0, rect.getHeight());
	Vector2 pos3 = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());
	Vector2 pos4 = rect.getOrigin() + Vector2(rect.getWidth(), 0);

	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.x, pos1.y, 1.0f, 1.3f);
	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.x, pos2.y, 1.0f, 1.5f);
	m_OriginalVertex[2].Pos = XMFLOAT4(pos3.x, pos3.y, 1.0f, 1.1f);
	m_OriginalVertex[3].Pos = XMFLOAT4(pos4.x, pos4.y, 1.0f, 1.5f);
	
	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
}

DrawSolidRectBuffer::~DrawSolidRectBuffer()
{}

bool DrawSolidRectBuffer::bindIndex()
{
	if (m_d3dIndexBuffer)
	{
		return true;
	}
	UINT indices[6] = {
		0, 1, 2,   //  Triangle  0
		0, 2, 3,   //  Triangle  1
	};

	// 要创建的索引的描述
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT) * 6;
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
	if (FAILED(result))
	{
		OutputDebugStringA("error: void DrawBuffer::bindIndex()");
		return false;
	}
	return true;
}

void DrawSolidRectBuffer::render()
{
	updateRender(); 
	getD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//getD3DContext()->Draw(m_vertexSize, 0);
	getD3DContext()->DrawIndexed(6, 0, 0);
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

	Vector2 pos1 = rect.getOrigin();
	Vector2 pos2 = rect.getOrigin() + Vector2(0, rect.getHeight());
	Vector2 pos3 = rect.getOrigin() + Vector2(rect.getWidth(), 0);
	Vector2 pos4 = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());

	const Rect2D& textrect = m_image->getRect();

	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.x, pos1.y, 1.0f, 0.3f);
	m_OriginalVertex[0].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY());

	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.x, pos2.y, 1.0f, 0.5f);
	m_OriginalVertex[1].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY() + textrect.getHeight());

	m_OriginalVertex[2].Pos = XMFLOAT4(pos3.x, pos3.y, 1.0f, 0.1f);
	m_OriginalVertex[2].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY());

	m_OriginalVertex[3].Pos = XMFLOAT4(pos4.x, pos4.y, 1.0f, 0.5f);
	m_OriginalVertex[3].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY() + textrect.getHeight());

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
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

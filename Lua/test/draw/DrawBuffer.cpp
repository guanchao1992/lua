#include "DrawBuffer.h"
#include "..\manager\DrawManager.h"
#include "..\manager\VideoManager.h"
#include "..\manager\TextureManager.h"


DrawBuffer::DrawBuffer(UINT vertexSize)
	: m_d3dBuffer(nullptr)
	, m_OriginalVertex(nullptr)
	, m_nowlVertex(nullptr)
	, m_vertexSize(vertexSize)
	, m_primitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST)
	, m_Usage(D3D11_USAGE_DEFAULT)
	, m_BindFlags(D3D11_BIND_VERTEX_BUFFER)
{
	m_OriginalVertex = new SimpleVertexOriginal[m_vertexSize];
	m_nowlVertex = new SimpleVertexMain[m_vertexSize];
}

DrawBuffer::~DrawBuffer()
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
	if (m_nowlVertex)
	{
		delete[] m_nowlVertex;
		m_nowlVertex = nullptr;
	}
}

bool DrawBuffer::updateBuffer(const Matrix4&transform)
{
	if (m_d3dBuffer)
	{
		m_d3dBuffer->Release();
		m_d3dBuffer = nullptr;
	}
	/*
	XMFLOAT3 rotate ={m_drawNode->getRotate().}
*/

	for (size_t i = 0; i < m_vertexSize; i++)
	{
		//Vector2 pos(m_OriginalVertex[i].Pos.x, m_OriginalVertex[i].Pos.y);
		//Vector2 d3dPos = VideoManager::getInstance()->ViewPostoD3D(pos);
		//Vector3 posEnd = transform.transformPosition(Vector3(d3dPos.x, d3dPos.y, 0));
		Vector3 pos(m_OriginalVertex[i].Pos.x, m_OriginalVertex[i].Pos.y, m_OriginalVertex[i].Pos.z);
		Vector3 posEnd = transform.transformPosition(pos);

		m_nowlVertex[i].Pos.x = posEnd.x;
		m_nowlVertex[i].Pos.y = posEnd.y;
		m_nowlVertex[i].Pos.z = posEnd.z;
		m_nowlVertex[i].Color = m_color;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = m_Usage;
	bd.ByteWidth = sizeof(SimpleVertexMain) * m_vertexSize;
	bd.BindFlags = m_BindFlags;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = m_nowlVertex;
	HRESULT result = S_OK;
	if (m_d3dBuffer == nullptr)
	{
		result = getD3DDevice()->CreateBuffer(&bd, &InitData, &m_d3dBuffer);
	}
	if (FAILED(result))
	{
		OutputDebugStringA("error: void DrawNodeRectBuffer::updateBuffer()");
		return false;
	}
	return true;
}

void DrawBuffer::render()
{
	DrawManager::getInstance()->setShaderType(ShaderType::Shader_Normal);
	UINT stride = sizeof(SimpleVertexMain);
	UINT offset = 0;
	getD3DContext()->IASetPrimitiveTopology(m_primitiveTopology);
	getD3DContext()->IASetVertexBuffers(0, 1, &m_d3dBuffer, &stride, &offset);
	getD3DContext()->Draw(m_vertexSize, 0);
}

void DrawBuffer::setColor(const Color4F&color)
{
	m_color.x = color.r;
	m_color.y = color.g;
	m_color.z = color.b;
	m_color.w = color.w;
}

//*********************------- DrawLineBuffer ------***********************//

DrawLineBuffer::DrawLineBuffer(const Vector3&pos1, const Vector3&pos2)
	:DrawBuffer(2)
{
	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.x, pos1.y, 1.0f, 1.3f);
	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.x, pos2.y, 1.0f, 1.5f);

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
	m_primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
}

DrawLineBuffer::~DrawLineBuffer()
{
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
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
}

DrawRectBuffer::~DrawRectBuffer()
{}

//*********************------- DrawSolidRectBuffer ------***********************//

DrawSolidRectBuffer::DrawSolidRectBuffer(const Rect2D& rect)
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
	m_OriginalVertex[4] = m_OriginalVertex[0];;

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
}

DrawSolidRectBuffer::~DrawSolidRectBuffer()
{}


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
	Vector2 pos3 = rect.getOrigin() + Vector2(rect.getWidth(), rect.getHeight());
	Vector2 pos4 = rect.getOrigin() + Vector2(rect.getWidth(), 0);

	const Rect2D& textrect = m_image->getRect();

	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.x, pos1.y, 1.0f, 0.3f);
	m_OriginalVertex[0].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY());

	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.x, pos2.y, 1.0f, 0.5f);
	m_OriginalVertex[1].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY() + textrect.getHeight());

	m_OriginalVertex[2].Pos = XMFLOAT4(pos3.x, pos3.y, 1.0f, 0.1f);
	m_OriginalVertex[2].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY() + textrect.getHeight());

	m_OriginalVertex[3].Pos = XMFLOAT4(pos4.x, pos4.y, 1.0f, 0.5f);
	m_OriginalVertex[3].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY());

	m_OriginalVertex[4] = m_OriginalVertex[0];

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
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
	static UINT stride = sizeof(SimpleVertexMain);
	UINT offset = 0;
	TextureBuffer*tb = m_image->getTexture();
	tb->PSSetView(getD3DContext());
	getD3DContext()->IASetPrimitiveTopology(m_primitiveTopology);
	getD3DContext()->IASetVertexBuffers(0, 1, &m_d3dBuffer, &stride, &offset);
	getD3DContext()->Draw(m_vertexSize, 0);
}

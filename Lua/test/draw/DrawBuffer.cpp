#include "DrawBuffer.h"
#include "..\manager\DrawManager.h"
#include "..\manager\VideoManager.h"
#include "..\manager\TextureManager.h"


DrawBuffer::DrawBuffer(UINT vertexSize)
	: m_d3dBuffer(nullptr)
	, m_OriginalVertex(nullptr)
	, m_nowlVertex(nullptr)
	, m_vertexSize(vertexSize)
	, m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
	, m_Usage(D3D11_USAGE_DEFAULT)
	, m_BindFlags(D3D11_BIND_VERTEX_BUFFER)
{
	m_OriginalVertex = new SimpleVertexMain[m_vertexSize];
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

bool DrawBuffer::updateBuffer(const Position2D&surePos, float scale)
{
	if (m_d3dBuffer)
	{
		m_d3dBuffer->Release();
		m_d3dBuffer = nullptr;
	}
	for (size_t i = 0; i < m_vertexSize; i++)
	{
		Position2D pos(m_OriginalVertex[i].Pos.x * scale + surePos.getPositionX(), m_OriginalVertex[i].Pos.y * scale + surePos.getPositionY());
		Position2D d3dPos = VideoManager::getInstance()->ViewPostoD3D(pos);

		m_nowlVertex[i].Pos.x = d3dPos.getPositionX();
		m_nowlVertex[i].Pos.y = d3dPos.getPositionY();
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

//*********************------- DrawRectBuffer ------***********************//

DrawRectBuffer::DrawRectBuffer(const Rect2D& rect, unsigned long rgb)
	:DrawBuffer(5)
{
	Position2D pos1 = rect.getOrigin();
	Position2D pos2 = rect.getOrigin() + Position2D(0, rect.getHeight());
	Position2D pos3 = rect.getOrigin() + Position2D(rect.getWidth(), rect.getHeight());
	Position2D pos4 = rect.getOrigin() + Position2D(rect.getWidth(), 0);
	float r = (rgb >> 24) / 256.0f;
	float g = (rgb >> 16) % 256 / 256.0f;
	float b = (rgb >> 8) % 256 / 256.0f;
	float w = rgb % 256 / 256.0f;

	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.getPositionX(), pos1.getPositionY(), 1.0f, 1.3f);
	m_OriginalVertex[0].Color = XMFLOAT4(r, g, b, w);

	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.getPositionX(), pos2.getPositionY(), 1.0f, 1.5f);
	m_OriginalVertex[1].Color = XMFLOAT4(r, g, b, w);

	m_OriginalVertex[2].Pos = XMFLOAT4(pos3.getPositionX(), pos3.getPositionY(), 1.0f, 1.1f);
	m_OriginalVertex[2].Color = XMFLOAT4(r, g, b, w);

	m_OriginalVertex[3].Pos = XMFLOAT4(pos4.getPositionX(), pos4.getPositionY(), 1.0f, 1.5f);
	m_OriginalVertex[3].Color = XMFLOAT4(r, g, b, w);

	m_OriginalVertex[4] = m_OriginalVertex[0];

	memcpy(m_nowlVertex, m_OriginalVertex, sizeof(SimpleVertexMain)*m_vertexSize);
}

DrawRectBuffer::~DrawRectBuffer()
{}

void DrawRectBuffer::render()
{
	DrawManager::getInstance()->setShaderType(ShaderType::Shader_Normal);
	UINT stride = sizeof(SimpleVertexMain);
	UINT offset = 0;
	getD3DContext()->IASetPrimitiveTopology(m_primitiveTopology);
	getD3DContext()->IASetVertexBuffers(0, 1, &m_d3dBuffer, &stride, &offset);
	getD3DContext()->Draw(m_vertexSize, 0);
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

	Position2D pos1 = rect.getOrigin();
	Position2D pos2 = rect.getOrigin() + Position2D(0, rect.getHeight());
	Position2D pos3 = rect.getOrigin() + Position2D(rect.getWidth(), rect.getHeight());
	Position2D pos4 = rect.getOrigin() + Position2D(rect.getWidth(), 0);

	const Rect2D& textrect = m_image->getRect();

	m_OriginalVertex[0].Pos = XMFLOAT4(pos1.getPositionX(), pos1.getPositionY(), 1.0f, 0.3f);
	m_OriginalVertex[0].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY());

	m_OriginalVertex[1].Pos = XMFLOAT4(pos2.getPositionX(), pos2.getPositionY(), 1.0f, 0.5f);
	m_OriginalVertex[1].Tx0 = XMFLOAT2(textrect.getOriginX(), textrect.getOriginY() + textrect.getHeight());

	m_OriginalVertex[2].Pos = XMFLOAT4(pos3.getPositionX(), pos3.getPositionY(), 1.0f, 0.1f);
	m_OriginalVertex[2].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY() + textrect.getHeight());

	m_OriginalVertex[3].Pos = XMFLOAT4(pos4.getPositionX(), pos4.getPositionY(), 1.0f, 0.5f);
	m_OriginalVertex[3].Tx0 = XMFLOAT2(textrect.getOriginX() + textrect.getWidth(), textrect.getOriginY());

	m_OriginalVertex[4] = m_OriginalVertex[0];

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
	static UINT stride = sizeof(SimpleVertexMain);
	UINT offset = 0;
	TextureBuffer*tb = m_image->getTexture();
	tb->PSSetView(getD3DContext());
	getD3DContext()->IASetPrimitiveTopology(m_primitiveTopology);
	getD3DContext()->IASetVertexBuffers(0, 1, &m_d3dBuffer, &stride, &offset);
	getD3DContext()->Draw(m_vertexSize, 0);
}

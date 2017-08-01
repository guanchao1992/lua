#include "TextureNode.h"
#include "..\manager\DrawManager.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "..\base\Size.h"
#include "..\manager\VideoManager.h"
#include "..\base\NodeList.h"
#include <D3DX11tex.h>
#include "..\manager\TextureManager.h"

using namespace DirectX;


//*************-------TextureNodeRectBuffer--------*************//

TextureNodeRectBuffer::TextureNodeRectBuffer(const std::string&imageName, const Rect2D& rect, D3D11_USAGE usage, UINT bindFlags)
	: m_d3dBuffer(nullptr)
	, m_OriginalVertex(nullptr)
	, m_vertexSize(5)
	, m_Usage(usage)
	, m_BindFlags(bindFlags)
	, m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
{
	m_image = TextureManager::getInstance()->loadImage("2");
	m_OriginalVertex = new SimpleVertexMain[5];
	m_nowlVertex = new SimpleVertexMain[5];

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

TextureNodeRectBuffer::~TextureNodeRectBuffer()
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

void TextureNodeRectBuffer::render()
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

bool TextureNodeRectBuffer::updateBuffer(const Position2D&surePos, float scale)
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

	HRESULT result = getD3DDevice()->CreateBuffer(&bd, &InitData, &m_d3dBuffer);
	if (FAILED(result))
	{
		OutputDebugStringA("error: void TextureNodeRectBuffer::updateBuffer()");
		return false;
	}
	return true;

	for (size_t i = 0; i < m_vertexSize; i++)
	{
		char buf[256];
		sprintf_s(buf, "(%f,%f,%f,%f)\n", m_nowlVertex[i].Pos.x, m_nowlVertex[i].Pos.y, m_nowlVertex[i].Pos.z, m_nowlVertex[i].Pos.w);
		OutputDebugStringA(buf);
	}
}

//*************-------TextureNode--------*************//

TextureNode::TextureNode()
{
}

TextureNode::~TextureNode()
{
	clear();
}

bool TextureNode::init()
{
	//setPosition(Position2D(1.f,1.f));
	DrawRect(Rect2D(-100, 0, 100, 100));
	return true;
}

void TextureNode::render()
{
	if (isRedraw())
	{
		redraw();
	}

	for (auto it : m_vecBuffer)
	{
		it->render();
	}
	for (Node* it : getChildren()->getListNode())
	{
		it->render();
	}
}

void TextureNode::redraw()
{
	__super::redraw();
	updateBuffer();
}

void TextureNode::clear()
{
	for (auto it:m_vecBuffer)
	{
		delete it;
	}
	m_vecBuffer.clear();
}

void TextureNode::DrawRect(const Rect2D&rect)
{
	TextureNodeRectBuffer* db = new TextureNodeRectBuffer("2", rect);
	updateBuffer();
	m_vecBuffer.push_back(db);
}

void TextureNode::updateBuffer()
{
	for (auto &it : m_vecBuffer)
	{
		it->updateBuffer(getSurePosition(), getScale());
	}
}

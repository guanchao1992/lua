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

using namespace DirectX;


TextureNode::TextureNode()
{
}

TextureNode::~TextureNode()
{
	clear();
}

bool TextureNode::init()
{
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(getD3DDevice(), getAccuratePath("image\\2.jpg").c_str(), 0, 0, &colorMap_, 0);

	if (FAILED(result))
	{
		return false;
	}

	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = getD3DDevice()->CreateSamplerState(&colorMapDesc, &colorMapSampler_);


	if (FAILED(result))
	{
		return false;
	}

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

	DrawManager::getInstance()->setShaderType(ShaderType::Shader_Texture);
	UINT stride = sizeof(SimpleVertexMain);
	UINT offset = 0;
	for (auto it : m_vecBuffer)
	{
		getD3DContext()->PSSetShaderResources(0, 1, &colorMap_);
		getD3DContext()->PSSetSamplers(0, 1, &colorMapSampler_);
		getD3DContext()->IASetPrimitiveTopology(it->m_primitiveTopology);
		getD3DContext()->IASetVertexBuffers(0, 1, &it->m_d3dBuffer, &stride, &offset);
		getD3DContext()->Draw(it->m_vertexSize, 0);
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
	Position2D pos1 = rect.getOrigin();
	Position2D pos2 = rect.getOrigin() + Position2D(0, rect.getHeight());
	Position2D pos3 = rect.getOrigin() + Position2D(rect.getWidth(), rect.getHeight());
	Position2D pos4 = rect.getOrigin() + Position2D(rect.getWidth(), 0);

	int vertexSize = 5;
	TextureNodeBuffer* db = new TextureNodeBuffer();
	db->m_OriginalVertex = new SimpleVertexMain[vertexSize];
	db->m_nowlVertex = new SimpleVertexMain[vertexSize];
	db->m_OriginalVertex[0].Pos = XMFLOAT4(pos1.getPositionX(), pos1.getPositionY(), 1.0f, 0.3f);
	db->m_OriginalVertex[0].Tx0 = XMFLOAT2(.0f, .0f);

	db->m_OriginalVertex[1].Pos = XMFLOAT4(pos2.getPositionX(), pos2.getPositionY(), 1.0f, 0.5f);
	db->m_OriginalVertex[1].Tx0 = XMFLOAT2(.0f, 1.0f);

	db->m_OriginalVertex[2].Pos = XMFLOAT4(pos3.getPositionX(), pos3.getPositionY(), 1.0f, 0.1f);
	db->m_OriginalVertex[2].Tx0 = XMFLOAT2(1.0f, 1.0f);

	db->m_OriginalVertex[3].Pos = XMFLOAT4(pos4.getPositionX(), pos4.getPositionY(), 1.0f, 0.5f);
	db->m_OriginalVertex[3].Tx0 = XMFLOAT2(1.0f, .0f);

	db->m_OriginalVertex[4] = db->m_OriginalVertex[0];

	memcpy(db->m_nowlVertex, db->m_OriginalVertex, sizeof(SimpleVertexMain)*vertexSize);
	db->m_primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	db->m_vertexSize = vertexSize;
	updateBuffer();
	m_vecBuffer.push_back(db);
}

void TextureNode::updateBuffer()
{
	for (auto &it : m_vecBuffer)
	{
		if (it->m_d3dBuffer)
		{
			it->m_d3dBuffer->Release();
			it->m_d3dBuffer = nullptr;
		}
		Position2D surePos = getSurePosition();
		for (size_t i = 0; i < it->m_vertexSize; i++)
		{
			Position2D pos(it->m_OriginalVertex[i].Pos.x * getScale() + surePos.getPositionX(), it->m_OriginalVertex[i].Pos.y * getScale() + surePos.getPositionY());
			//Position2D pos(it->m_OriginalVertex[i].Pos.x, it->m_OriginalVertex[i].Pos.y);
			Position2D d3dPos = VideoManager::getInstance()->ViewPostoD3D(pos);

			it->m_nowlVertex[i].Pos.x = d3dPos.getPositionX();
			it->m_nowlVertex[i].Pos.y = d3dPos.getPositionY();
		}
		createBuffer(it->m_nowlVertex, it->m_vertexSize, it->m_Usage, it->m_BindFlags, &it->m_d3dBuffer);

		for (size_t i = 0; i < it->m_vertexSize; i++)
		{
			char buf[256];
			sprintf_s(buf, "(%f,%f,%f,%f)\n", it->m_nowlVertex[i].Pos.x, it->m_nowlVertex[i].Pos.y, it->m_nowlVertex[i].Pos.z, it->m_nowlVertex[i].Pos.w);
			OutputDebugStringA(buf);
		}
	}
}

bool TextureNode::createBuffer(const SimpleVertexMain*vertex,UINT vertexSize,D3D11_USAGE usage,UINT bindFlags,ID3D11Buffer**outBuffer)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = usage;
	bd.ByteWidth = sizeof(SimpleVertexMain) * vertexSize;
	bd.BindFlags = bindFlags;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	HRESULT result = getD3DDevice()->CreateBuffer(&bd, &InitData, outBuffer);
	if (FAILED(result))
	{
		OutputDebugStringA("error: void TextureNode::updateBuffer()");
		return false;
	}
	return true;
}

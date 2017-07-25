#include "DrawNode.h"
#include "..\manager\DrawManager.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "..\base\Size.h"
#include "..\manager\VideoManager.h"

using namespace DirectX;


struct SimpleVertex
{
	XMFLOAT4 Pos;  // Position
	XMFLOAT4 Color;  // Color
};


DrawNode::DrawNode()
{
}

DrawNode::~DrawNode()
{
}

bool DrawNode::init()
{
	/*
	DrawOne(0.3, 0.5);
	DrawOne(0.1, 0.0);
*/
	return true;
}


void DrawNode::render()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	for (auto it : m_vecDrawBuffer)
	{
		getD3DContext()->IASetVertexBuffers(0, 1, &it, &stride, &offset);
		getD3DContext()->Draw(6, 0);
	}
}

void DrawNode::DrawOne(float x, float y)
{
	const Size& viewSize = VideoManager::getInstance()->getViewSize();
	float fx = 50.f / viewSize.getWidth();
	float fy = 50.f / viewSize.getHeight();
	SimpleVertex vertices[] =
	{
		XMFLOAT4(fx + x, fy + y, 0.5f,0.3f),
		XMFLOAT4(0.1f, 0.4f, 0.9f,0.3f),
		XMFLOAT4(fx + x, -fy + y, 0.5f,0.5f),
		XMFLOAT4(0.2f, 0.8f, 0.9f,0.3f),
		XMFLOAT4(-fx + x, -fy + y, 0.5f,1.0f),
		XMFLOAT4(0.5f, 0.1f, 0.9f,0.3f),

		XMFLOAT4(fx + x, fy + y, 0.5f,0.1f),
		XMFLOAT4(0.2f, 0.1f, 0.3f,0.3f),
		XMFLOAT4(-fx + x, -fy + y, 0.5f,0.5f),
		XMFLOAT4(0.5f, 0.9f, 0.1f,0.3f),
		XMFLOAT4(-fx + x, fy + y, 0.5f,0.2f),
		XMFLOAT4(0.1f, 0.1f, 0.9f,0.3f),
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	ID3D11Buffer *pVertexBuffer = nullptr;
	HRESULT result = getD3DDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(result))
	{
		return;
	}
	m_vecDrawBuffer.push_back(pVertexBuffer);
}


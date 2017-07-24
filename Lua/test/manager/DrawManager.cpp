#include "DrawManager.h"
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <directxcolors.h>
#include <windows.h>
#include "VideoManager.h"
#include "..\config.h"

using namespace DirectX;

SingletonClaseCpp(DrawManager);
DrawManager::DrawManager()
{
}

DrawManager::~DrawManager()
{
}

void DrawManager::Init()
{
	ID3DBlob* pVSBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;

	HRESULT result = 0;
	result = D3DCompileFromFile(getAccuratePathW(L"fx\\drawManager.fx").c_str(), NULL, NULL, "VS_Main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pVSBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != 0)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	result = getD3DDevice()->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pDrawVertexLayout);
	if (FAILED(result))
	{
		return;
	}

	result = getD3DDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pDrawVertexShader);
	if (FAILED(result))
	{
		return;
	}

	pVSBlob->Release();
	result = D3DCompileFromFile(getAccuratePathW(L"fx\\drawManager.fx").c_str(), NULL, NULL, "PS_Main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pVSBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != 0)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return;
	}

	result = getD3DDevice()->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pDrawPixelShader);
	if (FAILED(result))
	{
		return;
	}

	pVSBlob->Release();
}

void DrawManager::Cleanup()
{
	m_pDrawVertexShader->Release();
	m_pDrawPixelShader->Release();
	m_pDrawVertexLayout->Release();
}

struct SimpleVertex
{
	XMFLOAT3 Pos;  // Position
};

void DrawManager::DrawOne(float x,float y)
{
	const Size& viewSize = VideoManager::getInstance()->getViewSize();
	float fx = 50.f / viewSize.getWidth();
	float fy = 50.f / viewSize.getHeight();
	SimpleVertex vertices[] =
	{
		XMFLOAT3(fx + x, fy + y, 0.5f),
		XMFLOAT3(fx + x, -fy + y, 0.5f),
		XMFLOAT3(-fx + x, -fy + y, 0.5f),

		XMFLOAT3(fx + x, fy + y, 0.5f),
		XMFLOAT3(-fx + x, -fy + y, 0.5f),
		XMFLOAT3(-fx + x, fy + y, 0.5f),
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

void DrawManager::RenderDraw()
{
	getD3DContext()->IASetInputLayout(m_pDrawVertexLayout);
	getD3DContext()->VSSetShader(m_pDrawVertexShader, NULL, 0);
	getD3DContext()->PSSetShader(m_pDrawPixelShader, NULL, 0);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	for (auto it: m_vecDrawBuffer)
	{
		getD3DContext()->IASetVertexBuffers(0, 1, &it, &stride, &offset);
		getD3DContext()->Draw(6, 0);
	}
}

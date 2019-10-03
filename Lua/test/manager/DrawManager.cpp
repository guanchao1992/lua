#include "DrawManager.h"
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <directxcolors.h>
#include <windows.h>
#include "VideoManager.h"
#include "..\config.h"
#include "../base/DrawNode.h"

using namespace DirectX;

// A constant buffer that stores the three basic column-major matrices for composing geometry.
__declspec(align(16)) struct  ModelViewProjectionConstantBuffer
{
	XMFLOAT4X4 model;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

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
	result = D3DCompileFromFile(getAccuratePathW(L"fx\\drawManager.hlsl").c_str(), NULL, NULL, "VS_Main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pVSBlob, &pErrorBlob);
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ORIGIN" ,0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE" ,0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROTATEEE" ,0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR" ,0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	result = D3DCompileFromFile(getAccuratePathW(L"fx\\drawManager.hlsl").c_str(), NULL, NULL, "PS_Main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pVSBlob, &pErrorBlob);
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


	//绑定常量
	ID3D11Buffer *pBuffer = nullptr;
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	getD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pBuffer);
	getD3DContext()->VSSetConstantBuffers(0, 1, &pBuffer);

	auto vsize = VideoManager::getInstance()->getViewSize();
	ModelViewProjectionConstantBuffer constantBufferData;
	constantBufferData.model = XMFLOAT4X4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	constantBufferData.view = XMFLOAT4X4(
		2 / vsize.getWidth(), 0, 0, -1,
		0, -2 / vsize.getHeight(), 0, 1,
		0, 0, 0.1 / vsize.getWidth(), 0,
		0, 0, 0, 1);

	/*
	constantBufferData.view = XMFLOAT4X4(
		0.002, 0, 0, 0,
		0, 0.002, 0, 0,
		0, 0, 0.002, 0,
		0, 0, 0, 1);
*/
	constantBufferData.projection = XMFLOAT4X4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	getD3DContext()->UpdateSubresource(
		pBuffer,
		0,
		NULL,
		&constantBufferData,
		0,
		0
	);//刷新数据

}

void DrawManager::Cleanup()
{
	m_pDrawVertexShader->Release();
	m_pDrawPixelShader->Release();
	m_pDrawVertexLayout->Release();
}

struct SimpleVertex
{
	XMFLOAT3 pos;  // Position
	XMFLOAT3 origin;  // origin
	XMFLOAT3 scale;  // scale
	XMFLOAT3 rotate;  // rotate
	XMFLOAT3 color;	//color
};

void DrawManager::DrawOne(float x,float y)
{
	float fx = 50.f;
	float fy = 50.f;
	
	SimpleVertex vertices[] =
	{
		XMFLOAT3(fx + x, fy + y, 0.5f),		XMFLOAT3(x,y,0.5f),		XMFLOAT3(1,1,1),	XMFLOAT3(0,0,20),
		XMFLOAT3(fx + x, -fy + y, 0.5f),	XMFLOAT3(x,y,0.5f),		XMFLOAT3(1,1,1),	XMFLOAT3(0,0,20),
		XMFLOAT3(-fx + x, -fy + y, 0.5f),	XMFLOAT3(x,y,0.5f),		XMFLOAT3(1,1,1),	XMFLOAT3(0,0,20),
		XMFLOAT3(fx + x, fy + y, 0.5f),		XMFLOAT3(x,y,0.5f),		XMFLOAT3(1,1,1),	XMFLOAT3(0,0,20),
		XMFLOAT3(-fx + x, -fy + y, 0.5f),	XMFLOAT3(x,y,0.5f),		XMFLOAT3(1,1,1),	XMFLOAT3(0,0,20),
		XMFLOAT3(-fx + x, fy + y, 0.5f),	XMFLOAT3(x,y,0.5f),		XMFLOAT3(1,1,1),	XMFLOAT3(0,0,20),
	};

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = sizeof(SimpleVertex);
	InitData.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = InitData.SysMemPitch * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	ID3D11Buffer *pVertexBuffer = nullptr;
	HRESULT result = getD3DDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	//getD3DDevice()->

	if (FAILED(result))
	{
		return;
	}
	m_vecDrawBuffer.push_back(pVertexBuffer);

	auto dn = DrawNode::create(Vector3(x, y, 500), Size(100, 100));
	m_vecNode.push_back(dn);
}

void DrawManager::RenderDraw()
{
	auto context = getD3DContext();
	context->IASetInputLayout(m_pDrawVertexLayout);
	context->VSSetShader(m_pDrawVertexShader, NULL, 0);
	context->PSSetShader(m_pDrawPixelShader, NULL, 0);
	
/*
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	for (auto it: m_vecDrawBuffer)
	{
		context->IASetVertexBuffers(0, 1, &it, &stride, &offset);
		context->Draw(6, 0);
	}
*/
	for (auto it : m_vecNode)
	{
		it->redraw();
		it->reader(context);
	}
}

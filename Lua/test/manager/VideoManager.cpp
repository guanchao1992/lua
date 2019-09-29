#include "VideoManager.h"
#include <directxcolors.h>
#include <windows.h>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <stdio.h>
#include "..\config.h"
#include "DrawManager.h"
//#include "d3dx11effect.h"

using namespace DirectX;
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

SingletonClaseCpp(VideoManager);

ID3D11Device*getD3DDevice()
{
	return VideoManager::getInstance()->m_pd3dDevice;
}

ID3D11DeviceContext*getD3DContext()
{
	return VideoManager::getInstance()->m_pImmediateContext;
}

VideoManager::VideoManager()
	: m_pd3dDevice(nullptr)
	, m_pImmediateContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pVertexLayout(nullptr)
	, m_viewSize(DEFAULT_VIEW_W, DEFAULT_VIEW_H)
{
}


VideoManager::~VideoManager()
{
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT VideoManager::InitDevice(HWND hWnd)
{
	m_hWnd = hWnd;
	HRESULT hr = S_OK;

	/*
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
*/
	UINT width = 1000;
	UINT height = 800;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, &m_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, &m_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);

	LoadContent();

	return S_OK;
}


struct SimpleVertex
{
	XMFLOAT3 Pos;  // Position
};

void VideoManager::LoadContent()
{
	ID3DBlob* pVSBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;

	HRESULT result = 0;
	result = D3DCompileFromFile(getAccuratePathW(L"fx\\Squance.hlsl").c_str(), NULL, NULL, "VS_Main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pVSBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != 0)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			assert((char*)pErrorBlob->GetBufferPointer());
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
	result = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(result)) 
	{
		return;
	}
	
	result = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(result))
	{
		return;
	}
	pVSBlob->Release();


	result = D3DCompileFromFile(getAccuratePathW(L"fx\\Squance.hlsl").c_str(), NULL, NULL, "PS_Main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pVSBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != 0)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return;
	}

	result = m_pd3dDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	if (FAILED(result))
	{
		return;
	}
	pVSBlob->Release();

	// Create vertex buffer
	SimpleVertex vertices[6];
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

	result = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);


	SimpleVertex verticeschange[] =
	{
		XMFLOAT3(0.7f, 0.7f, 0.5f),
		XMFLOAT3(0.7f, 0.0f, 0.5f),
		XMFLOAT3(0.0f, 0.7f, 0.5f),

		XMFLOAT3(-0.8f, -0.7f, 0.5f),
		XMFLOAT3(-0.7f, 0.0f, 0.5f),
		XMFLOAT3(0.0f, -0.7f, 0.5f)
	};
	//
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, verticeschange, 0, 0);

	SimpleVertex verticeschange1[] =
	{
		XMFLOAT3(0.9f, 0.9f, 0.5f),
		XMFLOAT3(0.7f, 0.0f, 0.5f),
		XMFLOAT3(0.0f, 0.7f, 0.5f)
	};
	m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, 0, nullptr, verticeschange1, 0, 1);

	if (FAILED(result))
	{
		return;
	}
}



//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void VideoManager::CleanupDevice()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();

}

//--------------------------------------------------------------------------------------
// Render the frame
//--------------------------------------------------------------------------------------
void VideoManager::Render()
{

	//m_pImmediateContext->PSSetShaderResources(0, 1, &colorMap_);
	//m_pImmediateContext->PSSetSamplers(0, 1, &colorMapSampler_);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//m_pImmediateContext->PSSetConstantBuffers


	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);


	// Just clear the backbuffer
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::Black);


	m_pImmediateContext->Draw(6, 0);

	DrawManager::getInstance()->RenderDraw();

	//ÂíÉÏÊä³ö
	m_pSwapChain->Present(0, 0);
}

void VideoManager::setViewSize(Size size)
{
	if (m_viewSize == size)
	{
		return;
	}
	SetWindowPos(m_hWnd, NULL, 0, 0, size.getWidth(), size.getHeight(), SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW);
	m_viewSize = size;
}

Size VideoManager::getViewSize()
{
	return m_viewSize;
}

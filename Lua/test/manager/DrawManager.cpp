#include "DrawManager.h"
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <directxcolors.h>
#include <windows.h>
#include "VideoManager.h"
#include "..\config.h"
#include "..\draw\DrawLayout.h"
#include "ObjectManager.h"
#include "..\draw\DrawNode.h"
#include "..\base\NodeList.h"

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT4 Pos;  // Position
	XMFLOAT4 Color;  // Color
};

SingletonClaseCpp(DrawManager);
DrawManager::DrawManager()
	:m_pDrawVertexShader(nullptr)
	, m_pDrawPixelShader(nullptr)
	, m_pDrawGeometryShader(nullptr)
	//, m_pDrawVertexLayout(nullptr)
{
	m_listVertexLayout = NodeList::create();
	m_listVertexLayout->retain();
}

DrawManager::~DrawManager()
{
	m_listVertexLayout->release();
}

void DrawManager::Init()
{
	ID3DBlob *pVSBlob1 = loadID3DBlob(getAccuratePathW(L"fx\\drawManager.fx").c_str(), Blob_VS_Main, Blob_VS_Target);
	ID3DBlob *pVSBlob2 = loadID3DBlob(getAccuratePathW(L"fx\\drawManager.fx").c_str(), Blob_PS_Main, Blob_PS_Target);
	
	HRESULT result = getD3DDevice()->CreateVertexShader(pVSBlob1->GetBufferPointer(), pVSBlob1->GetBufferSize(), NULL, &m_pDrawVertexShader);
	if (FAILED(result))
	{
		return;
	}
	result = getD3DDevice()->CreatePixelShader(pVSBlob2->GetBufferPointer(), pVSBlob2->GetBufferSize(), NULL, &m_pDrawPixelShader);
	if (FAILED(result))
	{
		return;
	}

	DrawLayout *dl = createLayout(0);
	addLayout(dl);
}

void DrawManager::Cleanup()
{
	if(m_pDrawVertexShader)
		m_pDrawVertexShader->Release();
	if (m_pDrawPixelShader)
		m_pDrawPixelShader->Release();
	if (m_pDrawGeometryShader)
		m_pDrawGeometryShader->Release();
}

ID3DBlob* DrawManager::loadID3DBlob(const wchar_t*fxFile, const char*entryPoint, const char*target)
{
	ID3DBlob* pBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;

	HRESULT result = 0;
	result = D3DCompileFromFile(fxFile, NULL, NULL, entryPoint, target, D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != 0)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		assert(1);
		return nullptr;
	}

	m_mapID3DBlob[entryPoint] = pBlob;
	return pBlob;
}

ID3DBlob* DrawManager::getID3DBlob(const char*entryPoint)
{
	if (m_mapID3DBlob.find(entryPoint) == m_mapID3DBlob.end())
	{
		return nullptr;
	}
	return m_mapID3DBlob[entryPoint];
}

void DrawManager::DrawOne(float x,float y)
{
	DrawLayout*layout = getLayout(0);
	if (layout)
	{
		DrawNode*dn = DrawNode::create();
		layout->addChild(dn);
		dn->DrawOne(x, y);
	}
}

void DrawManager::RenderDraw()
{
	//getD3DContext()->IASetInputLayout(m_pDrawVertexLayout);
	getD3DContext()->VSSetShader(m_pDrawVertexShader, NULL, 0);
	getD3DContext()->PSSetShader(m_pDrawPixelShader, NULL, 0);

	for (auto it : m_listVertexLayout->getListNode())
	{
		DrawLayout*layout = (DrawLayout*)it;
		layout->render();
	}
}

DrawLayout* DrawManager::createLayout(int order)
{
	DrawLayout* layout = DrawLayout::create();
	layout->setOrder(order);
	return layout;
}

void DrawManager::addLayout(DrawLayout*layout)
{
	m_listVertexLayout->PushBack(layout);
}

DrawLayout* DrawManager::getLayout(int index)
{
	return dynamic_cast<DrawLayout*>(m_listVertexLayout->getNodeAtIndex(index));
}

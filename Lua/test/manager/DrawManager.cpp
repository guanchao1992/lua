#include "DrawManager.h"
#include <windows.h>
#include "VideoManager.h"
#include "..\config.h"
#include "..\draw\DrawLayout.h"
#include "ObjectManager.h"
#include "..\draw\DrawNode.h"
#include "..\base\NodeList.h"

#include <D3DX11.h>
#include <d3dx10async.h>
#include <D3Dcompiler.h>

using namespace DirectX;


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
	loadID3DBlob(getAccuratePath("fx\\drawManager.fx").c_str(), Blob_VS_Normal, Blob_VS_Target);
	loadID3DBlob(getAccuratePath("fx\\drawManager.fx").c_str(), Blob_PS_Normal, Blob_PS_Target);
	loadID3DBlob(getAccuratePath("fx\\drawManager.fx").c_str(), Blob_VS_Texture, Blob_VS_Target);
	loadID3DBlob(getAccuratePath("fx\\drawManager.fx").c_str(), Blob_PS_Texture, Blob_PS_Target);

	ID3DBlob *pVSBlob_Normal = getID3DBlob(Blob_VS_Normal);
	ID3DBlob *pPSBlob_Normal = getID3DBlob(Blob_PS_Normal);

	ID3DBlob *pVSBlob_Texture = getID3DBlob(Blob_VS_Normal);
	ID3DBlob *pPSBlob_Texture = getID3DBlob(Blob_PS_Normal);

	HRESULT result = getD3DDevice()->CreateVertexShader(pVSBlob_Normal->GetBufferPointer(), pVSBlob_Normal->GetBufferSize(), NULL, &m_pNormalVertexShader);
	if (FAILED(result))
	{
		return;
	}
	result = getD3DDevice()->CreatePixelShader(pPSBlob_Normal->GetBufferPointer(), pPSBlob_Normal->GetBufferSize(), NULL, &m_pNormalPixelShader);
	if (FAILED(result))
	{
		return;
	}

	HRESULT result = getD3DDevice()->CreateVertexShader(pVSBlob_Texture->GetBufferPointer(), pVSBlob_Texture->GetBufferSize(), NULL, &m_pTextureVertexShader);
	if (FAILED(result))
	{
		return;
	}
	result = getD3DDevice()->CreatePixelShader(pPSBlob_Texture->GetBufferPointer(), pPSBlob_Texture->GetBufferSize(), NULL, &m_pTexturePixelShader);
	if (FAILED(result))
	{
		return;
	}

	DrawLayout *dl = createLayout(0);
	addLayout(dl);
	//dl->addChild(DrawNode::create());
}

void DrawManager::Cleanup()
{
	m_listVertexLayout->Clear();
	if(m_pNormalVertexShader)
		m_pNormalVertexShader->Release();
	if (m_pNormalPixelShader)
		m_pNormalPixelShader->Release();
	if(m_pTextureVertexShader)
		m_pTextureVertexShader->Release();
	if (m_pTexturePixelShader)
		m_pTexturePixelShader->Release();

	if (m_pDrawGeometryShader)
		m_pDrawGeometryShader->Release();
}

ID3DBlob* DrawManager::loadID3DBlob(const char*fxFile, const char*entryPoint, const char*target)
{
	ID3DBlob* pBlob = NULL;
	ID3DBlob* pErrorBlob = NULL;

	HRESULT result = 0;
	//result = D3DCompileFromFile(fxFile, NULL, NULL, entryPoint, target, D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS, NULL, &pBlob, &pErrorBlob);
	result = D3DX11CompileFromFile(fxFile, NULL, NULL, entryPoint, target, D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS, NULL, NULL, &pBlob, &pErrorBlob, NULL);

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
		dn->setPosition(Position2D(x, y));
	}
}

void DrawManager::setShaderType(ShaderType type)
{
	switch (type)
	{
	case Shader_Normal:
		getD3DContext()->VSSetShader(m_pNormalVertexShader, NULL, 0);
		getD3DContext()->PSSetShader(m_pNormalPixelShader, NULL, 0);
		break;
	case Shader_Texture:
		getD3DContext()->VSSetShader(m_pTextureVertexShader, NULL, 0);
		getD3DContext()->PSSetShader(m_pTexturePixelShader, NULL, 0);
		break;
	default:
		break;
	}
}

void DrawManager::RenderDraw()
{
	//getD3DContext()->IASetInputLayout(m_pDrawVertexLayout);
	//getD3DContext()->VSSetShader(m_pDrawVertexShader, NULL, 0);
	//getD3DContext()->PSSetShader(m_pDrawPixelShader, NULL, 0);

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

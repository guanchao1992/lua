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
#include "..\..\MyTool\tool_log.h"
#include <d3d11.h>
#include "..\draw\DrawNode.h"

using namespace DirectX;


SingletonClaseCpp(DrawManager);
DrawManager::DrawManager()
	: m_pDrawGeometryShader(nullptr)
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
	struct  ShaderS
	{
		ShaderType type;
		const char* vsEntryPoint;
		const char* psEntryPoint;
	};
	const ShaderS shader[] = {
		Shader_Main, Blob_VS_Main ,Blob_PS_Main ,
		Shader_Normal, Blob_VS_Normal ,Blob_PS_Normal ,
		Shader_Texture, Blob_VS_Texture ,Blob_PS_Texture ,
	};
	for (int i = 0; i < sizeof(shader) / sizeof(*shader); ++i)
	{
		ID3DBlob *pVSBlob = loadID3DBlob(getAccuratePath("fx\\drawManager.fx").c_str(), shader[i].vsEntryPoint, Blob_VS_Target);
		ID3DBlob *pPSBlob = loadID3DBlob(getAccuratePath("fx\\drawManager.fx").c_str(), shader[i].psEntryPoint, Blob_PS_Target);
		if (pVSBlob == nullptr || pPSBlob == nullptr)
		{
			LOG_E("CreateVertexShader failed.:pVSBlob or pPSBlob load failed.", );
			assert(1);
			return;
		}

		HRESULT result = getD3DDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_mapVertexShader[shader[i].type]);
		if (FAILED(result))
		{
			LOG_E("CreateVertexShader failed.:%s", shader[i].vsEntryPoint);
			assert(1);
			return;
		}
		result = getD3DDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_mapPixelShader[shader[i].type]);
		if (FAILED(result))
		{
			LOG_E("CreateVertexShader failed.:%s", shader[i].psEntryPoint);
			assert(1);
			return;
		}
	}

	DrawLayout *dl = createLayout(0);
	addLayout(dl);
	//dl->addChild(DrawNode::create());
}

void DrawManager::Cleanup()
{
	m_listVertexLayout->Clear();

	for (auto it:m_mapID3DBlob)
	{
		it.second->Release();
	}
	m_mapID3DBlob.clear();
	for (auto it : m_mapVertexShader)
	{
		it.second->Release();
	}
	m_mapVertexShader.clear();
	for (auto it : m_mapPixelShader)
	{
		it.second->Release();
	}
	m_mapPixelShader.clear();

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
		/*
		DrawNode*dn = DrawNode::create();
		layout->addChild(dn);
		dn->setPosition(Position2D(x, y));
*/
		DrawNode*tn = DrawNode::create();
		layout->addChild(tn);
		tn->setPosition(Position2D(x, y));
	}
}

void DrawManager::setShaderType(ShaderType type)
{
	if (m_mapVertexShader.find(type) != m_mapVertexShader.end())
	{
		getD3DContext()->VSSetShader(m_mapVertexShader[type], NULL, 0);
	}
	if (m_mapPixelShader.find(type) != m_mapPixelShader.end())
	{
		getD3DContext()->PSSetShader(m_mapPixelShader[type], NULL, 0);
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

#include "DrawManager.h"
#include <windows.h>
#include "VideoManager.h"
#include "..\config.h"
#include "..\draw\DrawLayout.h"
#include "ObjectManager.h"
#include "..\draw\DrawNode.h"
#include "..\base\RefList.h"

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
}

DrawManager::~DrawManager()
{
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

	DrawLayout *dl = DrawLayout::create(0);
	addLayout(dl, "DrawManager");
}

void DrawManager::Cleanup()
{
	for (auto &it : m_listVertexLayout)
	{
		it.second->release();
	}
	m_listVertexLayout.clear();

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
	DrawLayout*layout = getLayout("DrawManager");
	if (layout)
	{
		/*
		DrawNode*dn = DrawNode::create();
		layout->addChild(dn);
		dn->setPosition(Vector2(x, y));
*/
		DrawNode*tn = DrawNode::create();
		layout->addChild(tn);
		tn->setPosition(Vector2(x, y));
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

void DrawManager::RenderDraw(const Matrix4& transform)
{
	for (auto &it : m_listVertexLayout)
	{
		DrawLayout*layout = (DrawLayout*)it.second;
		layout->render(transform);
	}
}

void DrawManager::addLayout(DrawLayout*layout, const std::string& layoutName)
{
	for (auto &it : m_listVertexLayout)
	{
		if (it.first == layoutName)
		{
			LOG_E("´íÎó");
			return;
		}
	}
	layout->retain();
	m_listVertexLayout.push_back(std::pair<std::string, DrawLayout*>(layoutName, layout));
	updateLayoutOrder();
}

void DrawManager::removeLayout(const std::string&layoutName)
{
	for (auto it = m_listVertexLayout.begin(); it != m_listVertexLayout.end(); ++it)
	{
		if (it->first == layoutName)
		{
			it->second->release();
			m_listVertexLayout.erase(it);
			return;
		}
	}
}

static bool sort_layout(const std::pair<std::string, DrawLayout*>& a, const std::pair<std::string, DrawLayout*>& b)
{
	if (a.second->getOrder() > b.second->getOrder())
	{
		return true;
	}
	return false;
}

void DrawManager::updateLayoutOrder()
{
	std::sort(m_listVertexLayout.begin(), m_listVertexLayout.end(), sort_layout);
}

DrawLayout* DrawManager::getLayout(const std::string&layoutName)
{
	for (auto &it : m_listVertexLayout)
	{
		if (it.first == layoutName)
		{
			return it.second;
		}
	}
	return nullptr;
}

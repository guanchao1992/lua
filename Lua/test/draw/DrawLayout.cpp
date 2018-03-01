#include "DrawLayout.h"
#include "..\manager\DrawManager.h"
#include <d3d11.h>
#include "..\manager\VideoManager.h"
#include "..\base\NodeList.h"
#include <d3dcommon.h>
#include "..\manager\GameTime.h"

DrawLayout::DrawLayout()
	: m_pDrawVertexLayout(nullptr)
	, m_order(0)
{
}

DrawLayout::~DrawLayout()
{
	if (m_pDrawVertexLayout)
	{
		m_pDrawVertexLayout->Release();
	}
}

bool DrawLayout::init()
{
	ID3DBlob* pBlob = DrawManager::getInstance()->getID3DBlob(Blob_VS_Main);
	if (pBlob == nullptr)
	{
		return false;
	}
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	// Create the input layout
	HRESULT result = getD3DDevice()->CreateInputLayout(layout, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pDrawVertexLayout);

	if (FAILED(result))
	{
		assert(1);
		return false;
	}

	return true;
}

void DrawLayout::render()
{
	getD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	getD3DContext()->IASetInputLayout(m_pDrawVertexLayout);

	for (Node* it : getChildren()->getListNode())
	{
		it->render();
	}
}


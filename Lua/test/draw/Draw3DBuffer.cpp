#include "Draw3DBuffer.h"
#include "..\manager\DrawManager.h"
#include "..\manager\VideoManager.h"
#include "..\manager\TextureManager.h"


//*********************------- DrawCubeBuffer ------***********************//

DrawCubeBuffer::DrawCubeBuffer(const Vector3& size, const Color4F& color)
	: DrawBuffer(8, color)
	, m_fill(false)
{
	m_OriginalVector3[0] = Vector3(-size.x / 2, -size.y / 2, -size.z / 2);
	m_OriginalVector3[1] = Vector3(size.x / 2, -size.y / 2, -size.z / 2);
	m_OriginalVector3[2] = Vector3(size.x / 2, size.y / 2, -size.z / 2);
	m_OriginalVector3[3] = Vector3(-size.x / 2, size.y / 2, -size.z / 2);

	m_OriginalVector3[4] = Vector3(-size.x / 2, -size.y / 2, size.z / 2);
	m_OriginalVector3[5] = Vector3(size.x / 2, -size.y / 2, size.z / 2);
	m_OriginalVector3[6] = Vector3(size.x / 2, size.y / 2, size.z / 2);
	m_OriginalVector3[7] = Vector3(-size.x / 2, size.y / 2, size.z / 2);

	// 01 03 04 12 15 23 26 37 45 47 56 67
}

DrawCubeBuffer::~DrawCubeBuffer()
{
}

bool DrawCubeBuffer::bindIndex()
{
	if (m_d3dIndexBuffer)
	{
		return true;
	}
	//0123
	//4567
	//0145
	//2367
	//0347
	//1256

	// 01 03 04 12 15 23 26 37 45 47 56 67
	UINT indices[60] = {
		//面
		0, 2, 1,  0, 3, 2,
		4, 5, 6,  4, 6, 7,
		0, 1, 4,  1, 5, 4,
		2, 3, 6,  3, 7, 6,
		0, 4, 3,  3, 4, 7,
		1, 2, 5,  2, 6, 5,
		//边界
		0,1, 0,3, 0,4, 1,2, 1,5, 2,3,
		2,6, 3,7, 4,5, 4,7, 5,6, 6,7
	};

	// 要创建的索引的描述
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT) * 60;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	// 设定用于初始化索引缓冲的数据
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	HRESULT result = S_OK;
	// 创建索引缓冲
	if (m_d3dIndexBuffer == nullptr)
	{
		result = getD3DDevice()->CreateBuffer(&bd, &InitData, &m_d3dIndexBuffer);
	}
	if (FAILED(result))
	{
		OutputDebugStringA("error: void DrawBuffer::bindIndex()");
		return false;
	}
	return true;
}

void DrawCubeBuffer::render()
{
	updateRender();
	if (m_fill)
	{
		getD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		getD3DContext()->DrawIndexed(36, 0, 0);
	}
	else
	{
		getD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		getD3DContext()->DrawIndexed(24, 36, 0);
	}
}
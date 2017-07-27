#include "DrawNode.h"
#include "..\manager\DrawManager.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "..\base\Size.h"
#include "..\manager\VideoManager.h"
#include "..\base\NodeList.h"

using namespace DirectX;


DrawNode::DrawNode()
{
}

DrawNode::~DrawNode()
{
	clear();
}

bool DrawNode::init()
{
	DrawRect(Rect2D(0, 0, 100, 105));
	//setPosition(Position2D(1.f,1.f));
	return true;
}

void DrawNode::render()
{
	if (isRedraw())
	{
		redraw();
	}
	UINT stride = sizeof(DrawNodeSimpleVertex);
	UINT offset = 0;
	for (auto it : m_vecBuffer)
	{
		getD3DContext()->IASetPrimitiveTopology(it->m_primitiveTopology);
		getD3DContext()->IASetVertexBuffers(0, 1, &it->m_d3dBuffer, &stride, &offset);
		getD3DContext()->Draw(it->m_vertexSize, 0);
	}
	for (Node* it : getChildren()->getListNode())
	{
		it->render();
	}
}

void DrawNode::redraw()
{
	__super::redraw();
	updateBuffer();
}

void DrawNode::clear()
{
	for (auto it:m_vecBuffer)
	{
		delete it;
	}
	m_vecBuffer.clear();
}

void DrawNode::DrawOne(float x, float y)
{
	const Size& viewSize = VideoManager::getInstance()->getViewSize();
	float fx = 50.f / viewSize.getWidth();
	float fy = 50.f / viewSize.getHeight();

	int vertexSize = 6;
	DrawBuffer* db = new DrawBuffer();
	db->m_OriginalVertex = new DrawNodeSimpleVertex[vertexSize];
	db->m_nowlVertex = new DrawNodeSimpleVertex[vertexSize];
	db->m_OriginalVertex[0].Pos = XMFLOAT4(fx + x, fy + y, 0.5f, 0.3f);
	db->m_OriginalVertex[0].Color = XMFLOAT4(0.1f, 0.4f, 0.9f, 0.3f);
	db->m_OriginalVertex[1].Pos = XMFLOAT4(fx + x, -fy + y, 0.5f, 0.5f);
	db->m_OriginalVertex[1].Color = XMFLOAT4(0.2f, 0.8f, 0.9f, 0.3f);
	db->m_OriginalVertex[2].Pos = XMFLOAT4(-fx + x, -fy + y, 0.5f, 1.0f);
	db->m_OriginalVertex[2].Color = XMFLOAT4(0.5f, 0.1f, 0.9f, 0.3f);

	db->m_OriginalVertex[3].Pos = XMFLOAT4(fx + x, fy + y, 0.5f, 0.1f);
	db->m_OriginalVertex[3].Color = XMFLOAT4(0.2f, 0.1f, 0.3f, 0.3f);
	db->m_OriginalVertex[4].Pos = XMFLOAT4(-fx + x, -fy + y, 0.5f, 0.5f);
	db->m_OriginalVertex[4].Color = XMFLOAT4(0.5f, 0.9f, 0.1f, 0.3f);
	db->m_OriginalVertex[5].Pos = XMFLOAT4(-fx + x, fy + y, 0.5f, 0.2f);
	db->m_OriginalVertex[5].Color = XMFLOAT4(0.1f, 0.1f, 0.9f, 0.3f);
	db->m_vertexSize = vertexSize;
	updateBuffer();

	m_vecBuffer.push_back(db);
}

void DrawNode::DrawRect(const Rect2D&rect)
{
	/*
	Position2D pos1 = VideoManager::getInstance()->ViewPostoD3D(rect.getOrigin() + Position2D(0, rect.getHeight()));
	Position2D pos2 = VideoManager::getInstance()->ViewPostoD3D(rect.getOrigin());
	Position2D pos3 = VideoManager::getInstance()->ViewPostoD3D(rect.getOrigin() + Position2D(rect.getWidth(), 0));
	Position2D pos4 = VideoManager::getInstance()->ViewPostoD3D(rect.getOrigin() + Position2D(rect.getWidth(), rect.getHeight()));
*/

	Position2D pos1 = rect.getOrigin() + Position2D(0, rect.getHeight());
	Position2D pos2 = rect.getOrigin();
	Position2D pos3 = rect.getOrigin() + Position2D(rect.getWidth(), 0);
	Position2D pos4 = rect.getOrigin() + Position2D(rect.getWidth(), rect.getHeight());

	int vertexSize = 5;
	DrawBuffer* db = new DrawBuffer();
	db->m_OriginalVertex = new DrawNodeSimpleVertex[vertexSize];
	db->m_nowlVertex = new DrawNodeSimpleVertex[vertexSize];
	db->m_OriginalVertex[0].Pos = XMFLOAT4(pos1.getPositionX(), pos1.getPositionY(), 1.0f, 0.3f);
	db->m_OriginalVertex[0].Color = XMFLOAT4(0.1f, 0.4f, 0.9f, 0.3f);
	db->m_OriginalVertex[1].Pos = XMFLOAT4(pos2.getPositionX(), pos2.getPositionY(), 1.0f, 0.5f);
	db->m_OriginalVertex[1].Color = XMFLOAT4(0.2f, 0.8f, 0.9f, 0.3f);
	db->m_OriginalVertex[2].Pos = XMFLOAT4(pos3.getPositionX(), pos3.getPositionY(), 1.0f, 0.1f);
	db->m_OriginalVertex[2].Color = XMFLOAT4(0.2f, 0.1f, 0.3f, 0.3f);
	db->m_OriginalVertex[3].Pos = XMFLOAT4(pos4.getPositionX(), pos4.getPositionY(), 1.0f, 0.5f);
	db->m_OriginalVertex[3].Color = XMFLOAT4(0.2f, 0.8f, 0.9f, 0.3f);
	db->m_OriginalVertex[4].Pos = db->m_OriginalVertex[0].Pos;
	db->m_OriginalVertex[4].Color = db->m_OriginalVertex[0].Color;
	memcpy(db->m_nowlVertex, db->m_OriginalVertex, sizeof(DrawNodeSimpleVertex)*vertexSize);
	db->m_primitiveTopology = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP;
	db->m_vertexSize = vertexSize;
	updateBuffer();
	//createBuffer(db->m_OriginalVertex, db->m_vertexSize, db->m_Usage, db->m_BindFlags, &db->m_d3dBuffer);
	m_vecBuffer.push_back(db);
}

void DrawNode::updateBuffer()
{
	for (auto &it : m_vecBuffer)
	{
		if (it->m_d3dBuffer)
		{
			it->m_d3dBuffer->Release();
			it->m_d3dBuffer = nullptr;
		}
		Position2D surePos = getSurePosition();
		for (size_t i = 0; i < it->m_vertexSize; i++)
		{
			Position2D pos(it->m_OriginalVertex[i].Pos.x * getScale() + surePos.getPositionX(), it->m_OriginalVertex[i].Pos.y * getScale() + surePos.getPositionY());
			//Position2D pos(it->m_OriginalVertex[i].Pos.x, it->m_OriginalVertex[i].Pos.y);
			Position2D d3dPos = VideoManager::getInstance()->ViewPostoD3D(pos);

			it->m_nowlVertex[i].Pos.x = d3dPos.getPositionX();
			it->m_nowlVertex[i].Pos.y = d3dPos.getPositionY();
		}
		createBuffer(it->m_nowlVertex, it->m_vertexSize, it->m_Usage, it->m_BindFlags, &it->m_d3dBuffer);

		for (size_t i = 0; i < it->m_vertexSize; i++)
		{
			char buf[256];
			sprintf_s(buf, "(%f,%f,%f,%f)\n", it->m_nowlVertex[i].Pos.x, it->m_nowlVertex[i].Pos.y, it->m_nowlVertex[i].Pos.z, it->m_nowlVertex[i].Pos.w);
			OutputDebugStringA(buf);
		}
	}
}

bool DrawNode::createBuffer(const DrawNodeSimpleVertex*vertex,UINT vertexSize,D3D11_USAGE usage,UINT bindFlags,ID3D11Buffer**outBuffer)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = usage;
	bd.ByteWidth = sizeof(DrawNodeSimpleVertex) * vertexSize;
	bd.BindFlags = bindFlags;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	HRESULT result = getD3DDevice()->CreateBuffer(&bd, &InitData, outBuffer);
	if (FAILED(result))
	{
		OutputDebugStringA("error: void DrawNode::updateBuffer()");
		return false;
	}
	return true;
}

#include "DrawNode.h"
#include "..\manager\VideoManager.h"
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


struct SimpleVertex
{
	XMFLOAT3 pos;  // Position
	XMFLOAT3 origin;  // origin
	XMFLOAT3 scale;  // scale
	XMFLOAT3 rotate;  // rotate
	XMFLOAT3 color;		//color
};

DrawNode* DrawNode::create(Vector3 origin, Size size)
{
	DrawNode *ret = new DrawNode();
	ret->origin = origin;
	ret->size = size;
	ret->scale = Vector3(1, 1, 1);
	ret->Init();
	return ret;
}

DrawNode::DrawNode()
{
}

DrawNode::~DrawNode()
{
}

void DrawNode::Init()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	HRESULT result = getD3DDevice()->CreateBuffer(&bd, nullptr, &_drawBuffer);
}

void DrawNode::redraw()
{
	_buffVec.clear();

	_buffVec.push_back(origin + Vector3(size.getWidth(), -size.getHeight(), 0));
	_buffVec.push_back(origin + Vector3(size.getWidth(), size.getHeight(), 0));
	_buffVec.push_back(origin + Vector3(-size.getWidth(), -size.getHeight(), 0));

	_buffVec.push_back(origin + Vector3(size.getWidth(), size.getHeight(), 0));
	_buffVec.push_back(origin + Vector3(-size.getWidth(), -size.getHeight(), 0));
	_buffVec.push_back(origin + Vector3(-size.getWidth(), size.getHeight(), 0));

	//auto orf = VideoManager::getInstance()->pos2fPos(origin);

	auto viewSize = VideoManager::getInstance()->getViewSize();
	SimpleVertex vertices[] =
	{
		XMFLOAT3(origin.getX() + size.getWidth() / 2, origin.getY() - size.getHeight() / 2, origin.getZ()),XMFLOAT3(1,0,0),		XMFLOAT3(origin.getX(),origin.getY(),origin.getZ()),		XMFLOAT3(scale.getX(),scale.getY(),scale.getZ()),	XMFLOAT3(0,0,0),
		XMFLOAT3(origin.getX() + size.getWidth() / 2, origin.getY() + size.getHeight() / 2, origin.getZ()),XMFLOAT3(0,1,0),		XMFLOAT3(origin.getX(),origin.getY(),origin.getZ()),		XMFLOAT3(scale.getX(),scale.getY(),scale.getZ()),	XMFLOAT3(0,0,0),
		XMFLOAT3(origin.getX() - size.getWidth() / 2, origin.getY() - size.getHeight() / 2, origin.getZ()),XMFLOAT3(1,0,1),		XMFLOAT3(origin.getX(),origin.getY(),origin.getZ()),		XMFLOAT3(scale.getX(),scale.getY(),scale.getZ()),	XMFLOAT3(0,0,0),

		XMFLOAT3(origin.getX() + size.getWidth() / 2, origin.getY() + size.getHeight() / 2, origin.getZ()),XMFLOAT3(1,0,0),		XMFLOAT3(origin.getX(),origin.getY(),origin.getZ()),		XMFLOAT3(scale.getX(),scale.getY(),scale.getZ()),	XMFLOAT3(0,0,0),
		XMFLOAT3(origin.getX() - size.getWidth() / 2, origin.getY() + size.getHeight() / 2, origin.getZ()),XMFLOAT3(0,0,1),		XMFLOAT3(origin.getX(),origin.getY(),origin.getZ()),		XMFLOAT3(scale.getX(),scale.getY(),scale.getZ()),	XMFLOAT3(0,0,0),
		XMFLOAT3(origin.getX() - size.getWidth() / 2, origin.getY() - size.getHeight() / 2, origin.getZ()),XMFLOAT3(0,1,0),		XMFLOAT3(origin.getX(),origin.getY(),origin.getZ()),		XMFLOAT3(scale.getX(),scale.getY(),scale.getZ()),	XMFLOAT3(0,0,0),
	};
	//	re += 1;

	getD3DContext()->UpdateSubresource(_drawBuffer, 0, nullptr, vertices, 0, 1);
}

void DrawNode::reader(ID3D11DeviceContext *context)
{
	static UINT stride = sizeof(SimpleVertex);
	static UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &_drawBuffer, &stride, &offset);
	context->Draw(6, 0);
}

#pragma once
#include "..\base\Singleton.h"
#include <vector>
#include <list>
#include <map>
#include <d3dcommon.h>

#define Blob_VS_Main "VS_Main"
#define Blob_PS_Main "PS_Main"

#define Blob_VS_Target "vs_5_0"
#define Blob_PS_Target "ps_5_0"


class ID3D11DeviceContext;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11GeometryShader;
class ID3D11InputLayout;
class ID3D11Buffer;
class DrawLayout;
class NodeList;

class DrawManager
{
	SingletonClase(DrawManager);
	DrawManager();
public:
	~DrawManager();
	
	void Init();
	void Cleanup();
	ID3DBlob* loadID3DBlob(const wchar_t*fxFile, const char*entryPoint, const char*target);//使用entryPoint作为唯一标识
	ID3DBlob* getID3DBlob(const char*entryPoint);

	void DrawOne(float x,float y);

	DrawLayout* createLayout(int order);
	void addLayout(DrawLayout*layout);
	DrawLayout* getLayout(int index);

	void RenderDraw();
private:
	NodeList*							m_listVertexLayout;
	std::map<const char*, ID3DBlob*>	m_mapID3DBlob;

	ID3D11VertexShader*					m_pDrawVertexShader;
	ID3D11PixelShader*					m_pDrawPixelShader;
	ID3D11GeometryShader*				m_pDrawGeometryShader;
	//ID3D11InputLayout*					m_pDrawVertexLayout;
};


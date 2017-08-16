#pragma once
#include "..\base\Singleton.h"
#include <vector>
#include <list>
#include <map>
#include <d3dcommon.h>
#include "..\dxconfig.h"


struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
struct DrawLayout;
struct NodeList;
class Matrix4;

class DrawManager
{
	SingletonClase(DrawManager);
	DrawManager();
public:
	~DrawManager();
	
	void Init();
	void Cleanup();
	ID3DBlob* loadID3DBlob(const char*fxFile, const char*entryPoint, const char*target);//使用entryPoint作为唯一标识
	ID3DBlob* getID3DBlob(const char*entryPoint);

	void setShaderType(ShaderType type);

	void DrawOne(float x,float y);

	template <typename T>
	static T* createLayout(int order);

	void addLayout(DrawLayout*layout);
	DrawLayout* getLayout(int index);

	void RenderDraw(const Matrix4& transform);
private:
	NodeList*									m_listVertexLayout;
	std::map<const char*, ID3DBlob*>			m_mapID3DBlob;
	std::map<ShaderType, ID3D11VertexShader*>	m_mapVertexShader;
	std::map<ShaderType, ID3D11PixelShader*>	m_mapPixelShader;

	ID3D11GeometryShader*				m_pDrawGeometryShader;
	//ID3D11InputLayout*					m_pDrawVertexLayout;
};


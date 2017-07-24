#pragma once
#include "..\base\Singleton.h"
#include <vector>
#include <list>

class ID3D11DeviceContext;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11GeometryShader;
class ID3D11InputLayout;
class ID3D11Buffer;

class DrawLayout;

struct VertexLayout
{
	DrawLayout*	layout;
	ID3D11InputLayout*	m_pDrawVertexLayout;
	int order;
};

class DrawManager
{
	SingletonClase(DrawManager);
	DrawManager();
public:
	~DrawManager();
	
	void Init();
	void Cleanup();

	void DrawOne(float x,float y);

	void addLayout(int order);

	void RenderDraw();
private:
	std::vector<ID3D11Buffer*>		m_vecDrawBuffer;
	std::list<VertexLayout>			m_listVertexLayout;

	ID3D11VertexShader*				m_pDrawVertexShader;
	ID3D11PixelShader*				m_pDrawPixelShader;
	ID3D11GeometryShader*			m_pDrawGeometryShader;
	ID3D11InputLayout*				m_pDrawVertexLayout;
};


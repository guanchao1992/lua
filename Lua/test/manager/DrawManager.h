#pragma once
#include "..\base\Singleton.h"
#include <vector>

#include "../base/DrawNode.h"

class ID3D11DeviceContext;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11InputLayout;
class ID3D11Buffer;

class DrawManager
{
	SingletonClase(DrawManager);
	DrawManager();
public:
	~DrawManager();
	
	void Init();
	void Cleanup();

	void DrawOne(float x,float y);

	void RenderDraw();
private:
	std::vector<ID3D11Buffer*> m_vecDrawBuffer;
	std::list<DrawNode*> m_vecNode;

	ID3D11VertexShader*		m_pDrawVertexShader;
	ID3D11PixelShader*		m_pDrawPixelShader;
	ID3D11InputLayout*		m_pDrawVertexLayout;

};


#pragma once
#include "base\Singleton.h"
#include <windows.h>
#include "config.h"

struct shaderData
{
	BYTE *shaderCode;
	UINT size;
};

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11Buffer;
class ID3D11InputLayout;

ID3D11Device*			getD3DDevice();
ID3D11DeviceContext*	getD3DContext();

class VideoManager
{
	friend ID3D11Device* getD3DDevice();
	friend ID3D11DeviceContext*	getD3DContext();
	SingletonClase(VideoManager);
public:
	VideoManager();
	~VideoManager();
public:
	HRESULT InitDevice(HWND hWnd);
	void CleanupDevice();

	void LoadContent();
	struct shaderData *GetShaderByteCode(const char *compiledShader, SIZE_T &outlen);

	void Render();
	void RenderUI();
	
	void setViewSize(Size size);
	Size getViewSize();
private:
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*    m_pImmediateContext;
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;

	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11InputLayout*		m_pVertexLayout;

	Size					m_viewSize;
	HWND					m_hWnd;
};

#pragma once
#include "..\base\Singleton.h"
#include <windows.h>
#include "..\config.h"

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
IDXGISwapChain*			getSwapChain();

class VideoManager
{
	friend ID3D11Device* getD3DDevice();
	friend ID3D11DeviceContext*	getD3DContext();
	friend IDXGISwapChain* getSwapChain();
	SingletonClase(VideoManager);
public:
	VideoManager();
	~VideoManager();
public:
	HRESULT InitDevice(HWND hWnd);
	void CleanupDevice();
	struct shaderData *GetShaderByteCode(const char *compiledShader, SIZE_T &outlen);

	void ClearTargetView();
	void Present();
	

	//将d3d的坐标转换成view坐标
	Position2D D3DtoViewPos(const Position2D&pos);
	//将view坐标转换成d3d的坐标
	Position2D ViewPostoD3D(const Position2D&pos);

	//将window屏幕坐标转化成view坐标
	Position2D mousetoViewPos(LONG lparam);
	void setViewSize(Size size);
	Size getViewSize();
private:
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*    m_pImmediateContext;
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;

	Size					m_viewSize;
	HWND					m_hWnd;
};


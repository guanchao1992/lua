#pragma once
#include "..\base\Singleton.h"
#include <windows.h>
#include "..\config.h"
#include <DirectXMath.h>

struct shaderData
{
	BYTE *shaderCode;
	UINT size;
};

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D10EffectMatrixVariable;

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
	Vector2 D3DtoViewPos(const Vector2&pos);
	//将view坐标转换成d3d的坐标
	Vector2 ViewPostoD3D(const Vector2&pos);

	//将window屏幕坐标转化成view坐标
	Vector2 mousetoViewPos(LONG lparam);
	void setViewSize(Size size);
	Size getViewSize();

	void updateWorldTransform();
	void CreateWindowSizeDependentResources();

	void updateGame(float t);
private:
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*    m_pImmediateContext;
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D10EffectMatrixVariable *m_pFxWorldViewProj;

	Size					m_viewSize;
	HWND					m_hWnd;

};


#pragma once
#include "base\Singleton.h"
#include <windows.h>

struct shaderData
{
	BYTE *shaderCode;
	UINT size;
};

class VideoManager
{
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

private:
};


#pragma once
#include "base\Singleton.h"
#include <windows.h>


class VideoManager
{
	SingletonClase(VideoManager);
public:
	VideoManager();
	~VideoManager();
public:
	HRESULT InitDevice(HWND hWnd);
	void CleanupDevice();

	void Render();
	void RenderUI();

	void testDraw();
private:
};


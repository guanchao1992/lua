#pragma once

#include "base\Singleton.h"
#include <wtypes.h>

class GameApp
{
	SingletonClase(GameApp);
private:
	GameApp();
public:
	~GameApp();
public:
	void Init();
	void Close();
	void RenderUI();
	LRESULT WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
public:
	static GameApp* theGameApp;
};


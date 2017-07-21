#pragma once

#include "base\Singleton.h"
#include "config.h"
#include <wtypes.h>

class GameApp
{
	SingletonClase(GameApp);
private:
	GameApp();
public:
	~GameApp();
public:
	HRESULT Init(HWND hWnd);
	void Close();
	void Render();
	Position2D pos2fPos(HWND hWnd,LONG_PTR lParam);
	LRESULT WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
public:
	static GameApp* theGameApp;
};


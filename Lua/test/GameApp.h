#pragma once

#include "base\Singleton.h"
#include "config.h"
#include <wtypes.h>

class EventArgs;
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
	LRESULT WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);

	void mouseEvent(const EventArgs*args);
	void keyEvent(const EventArgs*args);
private:

	void OnEnter();
public:
	static GameApp* theGameApp;
};


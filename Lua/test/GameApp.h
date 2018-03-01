#pragma once

#include "base\Singleton.h"
#include "config.h"
#include <wtypes.h>

class EventArgs;
class GameLayout;

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
	void Update(float t);
	//屏幕像素坐标转换成窗口相对坐标
	Position2D pos2fPos(HWND hWnd,LONG_PTR lParam);
	LRESULT WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);

	void mouseEvent(const EventArgs*args);
	void keyEvent(const EventArgs*args);
public:
	static GameApp* theGameApp;
	GameLayout* _gl;
};


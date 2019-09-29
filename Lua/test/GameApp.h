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
	//��Ļ��������ת���ɴ����������
	Vector2 pos2fPos(HWND hWnd,LONG_PTR lParam);
	LRESULT WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);

	void mouseEvent(const EventArgs*args);
	void keyEvent(const EventArgs*args);
public:
	static GameApp* theGameApp;
};


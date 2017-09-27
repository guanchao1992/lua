#pragma once

#include "base\Singleton.h"
#include "config.h"
#include <wtypes.h>
#include "aircraft\Map.h"
#include "base\Matrix4.h"

class DrawNode;
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
	void Update(float t);
	//��Ļ��������ת���ɴ����������
	Vector2 pos2fPos(HWND hWnd,LONG_PTR lParam);
	LRESULT WndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);

	void mouseEvent(const EventArgs*args);

	void updateCameraTransform();

	void testKeyManager();
public:
	static GameApp* theGameApp;
	DrawNode*		m_drawnode;
	aircraft::Map*	m_aircraftMap;
};


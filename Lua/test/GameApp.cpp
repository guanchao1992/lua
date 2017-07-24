#include "GameApp.h"
#include "..\MyTool\tool_log.h"
#include "..\MyLua\ctolua.h"
#include "manager\ScriptsManager.h"
#include "ClassToLua.h"
#include "manager\VideoManager.h"
#include "manager\DrawManager.h"

SingletonClaseCpp(GameApp);
GameApp* GameApp::theGameApp = NULL;

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

int test_regFunc()
{
	LOG_D_F("main", "register ctolua function <testF>");
	lua_State*L = GetLuaState();
	RegLuaFunctionStart(L, "testF");
	cout << "--------------" << lua_get<int>(GetLuaState(), 1) << endl;
	RegLuaFunctionEnd();
	luaL_dostring(GetLuaState(), "testF(1231231)");
	return 1;
}

HRESULT GameApp::Init(HWND hWnd)
{
	theGameApp = this;
	log_init();
	ScriptsManager::getInstance()->Init();
	if (FAILED(VideoManager::getInstance()->InitDevice(hWnd)))
	{
		VideoManager::getInstance()->CleanupDevice();
		return S_FALSE;
	}
	DrawManager::getInstance()->Init();

	regAllClass();

	ScriptsManager::getInstance()->doFile("init.lua");
}

void GameApp::Close()
{
	lua_close(GetLuaState());

	DrawManager::getInstance()->Cleanup();
	VideoManager::getInstance()->CleanupDevice();

	log_close();
}

void GameApp::Render()
{
	VideoManager::getInstance()->Render();
	DrawManager::getInstance()->RenderDraw();
}

Position2D GameApp::pos2fPos(HWND hWnd,LONG_PTR lParam)
{
	UINT x = LOWORD(lParam);
	UINT y = HIWORD(lParam);
	const Size &size= VideoManager::getInstance()->getViewSize();
	Position2D pos((x / size.getWidth() - 0.5f) * 2, (0.5f - (y / size.getHeight())) * 2);
	return pos;
}

LRESULT GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	switch (message)
	{
	case WM_KEYDOWN:
	{
		UINT controlKey = LOWORD(lParam);
		UINT virtualKey = HIWORD(lParam);
		if (wParam == VK_A)
		{
			VideoManager::getInstance()->setViewSize(Size(1000,800));
		}
	}
		break;
	case WM_LBUTTONDOWN:
	{
		UINT x = LOWORD(lParam);
		UINT y = HIWORD(lParam);
		GetWindowRect(hWnd, &rect);
		Position2D pos = pos2fPos(hWnd, lParam);
		DrawManager::getInstance()->DrawOne(pos.getPositionX(),pos.getPositionY());
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
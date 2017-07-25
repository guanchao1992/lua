#include "GameApp.h"
#include "..\MyTool\tool_log.h"
#include "..\MyLua\ctolua.h"
#include "manager\ScriptsManager.h"
#include "ClassToLua.h"
#include "manager\VideoManager.h"
#include "manager\DrawManager.h"
#include "manager\EventManager.h"
#include "manager\ObjectManager.h"

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

	//EventManager::getInstance()->regEvent(EventRegType_Mouse, "gameapp", std::bind(&GameApp::mouseEvent, this, std::placeholders::_1));
	RegEvent(EventRegType_Mouse, "gameapp", GameApp::mouseEvent, 0);
	RegEvent(EventRegType_Key, "gameapp", GameApp::keyEvent, 0);
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
	VideoManager::getInstance()->ClearTargetView();
	DrawManager::getInstance()->RenderDraw();
	VideoManager::getInstance()->Present();
}

void GameApp::Update()
{
	ObjectManager::getInstance()->checkDelete();
	Render();
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
		EventManager::getInstance()->fireEvent(new KeyEventArgs(KeyEventArgs::KeyDown, wParam, LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_KEYUP:
		EventManager::getInstance()->fireEvent(new KeyEventArgs(KeyEventArgs::KeyUp, wParam, LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_LBUTTONDOWN:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(pos2fPos(hWnd, lParam), MouseEventArgs::LBMouseDown));
		break;
	case WM_LBUTTONUP:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(pos2fPos(hWnd, lParam), MouseEventArgs::LBMouseUp));
		break;
	case WM_RBUTTONDOWN:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(pos2fPos(hWnd, lParam), MouseEventArgs::RBMouseDown));
		break;
	case WM_RBUTTONUP:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(pos2fPos(hWnd, lParam), MouseEventArgs::RBMouseUp));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void GameApp::mouseEvent(const EventArgs*args)
{
	MouseEventArgs * e = (MouseEventArgs *)args;
	if (e->mouseType == MouseEventArgs::LBMouseDown)
	{
		DrawManager::getInstance()->DrawOne(e->viewPos.getPositionX(), e->viewPos.getPositionY());
		DrawManager::getInstance()->RenderDraw();
	}
}

void GameApp::keyEvent(const EventArgs*args)
{
	KeyEventArgs * e = (KeyEventArgs*)args;
	if (e->key == VK_A)
	{
		VideoManager::getInstance()->setViewSize(Size(1000, 800));
	}
}

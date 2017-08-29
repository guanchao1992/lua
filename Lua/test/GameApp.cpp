#include "GameApp.h"
#include "..\MyTool\tool_log.h"
#include "..\MyLua\ctolua.h"
#include "manager\ScriptsManager.h"
#include "ClassToLua.h"
#include "manager\VideoManager.h"
#include "manager\DrawManager.h"
#include "manager\EventManager.h"
#include "manager\ObjectManager.h"
#include "manager\GameTime.h"
#include <xercesc\util\PlatformUtils.hpp>
#include "manager\TextureManager.h"
#include "draw\DrawNode.h"
#include "draw\DrawLayout.h"
#include "base\RefList.h"
#include "base\NodeVector.h"
#include <D3DX10math.h>
#include "draw\DrawNode.h"
#include "manager\KeyManager.h"
#include "manager\b2DrawManager.h"

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
	xercesc_3_1::XMLPlatformUtils::Initialize();
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

	TextureManager::getInstance()->init();

	//EventManager::getInstance()->regEvent(EventRegType_Mouse, "gameapp", std::bind(&GameApp::mouseEvent, this, std::placeholders::_1));
	RegEvent(EventRegType_Mouse, "gameapp", GameApp::mouseEvent, 0);
	RegEvent(EventRegType_Key, "gameapp", GameApp::keyEvent, 0);

	KeyManager::getInstance()->Init();

	b2DrawManager::getInstance()->Init();

	DrawLayout*layout = DrawLayout::create(0);
	DrawManager::getInstance()->addLayout(layout, "gameapp_layout");

	/*
	m_drawnode = DrawNode::create();
	Rect2D rect = Rect2D(0.f, 0.f, 100.f, 100.f);
	m_drawnode->DrawRect(rect, 0xef2f00ff);
	layout->addChild(m_drawnode);
	testKeyManager();
*/

	updateCameraTransform();

	m_aircraftMap = new aircraft::Map();
	m_aircraftMap->startGame();
	return S_OK;
}

void GameApp::testKeyManager()
{
	KeyManager::getInstance()->RegKey('A', "左飘", KeyManager::Down | KeyManager::Loop, []() {
		auto pos = GameApp::getInstance()->m_drawnode->getPosition();
		GameApp::getInstance()->m_drawnode->setPosition(pos + Vector3(-20, 0, 0));
	}, 0.1f);
	KeyManager::getInstance()->RegKey('D', "右飘", KeyManager::Down | KeyManager::Loop, []() {
		auto pos = GameApp::getInstance()->m_drawnode->getPosition();
		GameApp::getInstance()->m_drawnode->setPosition(pos + Vector3(20, 0, 0));
	}, 0.1f);
	KeyManager::getInstance()->RegKey('W', "上飘", KeyManager::Down | KeyManager::Loop, []() {
		auto pos = GameApp::getInstance()->m_drawnode->getPosition();
		GameApp::getInstance()->m_drawnode->setPosition(pos + Vector3(0, 20, 0));
	}, 0.1f);
	KeyManager::getInstance()->RegKey('S', "下飘", KeyManager::Down | KeyManager::Loop, []() {
		auto pos = GameApp::getInstance()->m_drawnode->getPosition();
		GameApp::getInstance()->m_drawnode->setPosition(pos + Vector3(0, -20, 0));
	}, 0.1f);
}

void GameApp::Close()
{
	m_aircraftMap->clearGame();
	delete m_aircraftMap;
	m_aircraftMap = nullptr;

	lua_close(GetLuaState());

	DrawManager::getInstance()->Cleanup();
	TextureManager::getInstance()->releaseAllTexture();
	//最后检查一次引用计数
	ObjectManager::getInstance()->checkDelete();
	VideoManager::getInstance()->CleanupDevice();
	log_close();
	xercesc_3_1::XMLPlatformUtils::Terminate();
}

void GameApp::Render()
{
	VideoManager::getInstance()->ClearTargetView();
	//pD3DDEV->SetStreamSource

	DrawManager::getInstance()->RenderDraw(m_camera_transform);
	VideoManager::getInstance()->Present();

}

void GameApp::updateCameraTransform()
{
	Size winSize = VideoManager::getInstance()->getViewSize();
	m_camera_transform.set(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	/*
	auto eye = XMVectorSet(0, 0, -1000, 0);
	auto at = XMVectorSet(0, 0, 0, 0);
	auto up = XMVectorSet(0, 1, 0, 0);
	auto viewMatrix = XMMatrixLookAtLH(eye, at, up);
	m_camera_transform = viewMatrix;
*/
}

void GameApp::Update(float t)
{
	ObjectManager::getInstance()->checkDelete();
	KeyManager::getInstance()->Update(t);
	VideoManager::getInstance()->updateGame(t);

	if (m_aircraftMap)
	{
		m_aircraftMap->updateMap(t);
	}
}

Vector2 GameApp::pos2fPos(HWND hWnd,LONG_PTR lParam)
{
	UINT x = LOWORD(lParam);
	UINT y = HIWORD(lParam);
	const Size &size= VideoManager::getInstance()->getViewSize();
	Vector2 pos((x / size.getWidth() - 0.5f) * 2, (0.5f - (y / size.getHeight())) * 2);
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
		EventManager::getInstance()->fireEvent(new MouseEventArgs(VideoManager::getInstance()->mousetoViewPos(lParam), MouseEventArgs::LBMouseDown));
		break;
	case WM_LBUTTONUP:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(VideoManager::getInstance()->mousetoViewPos(lParam), MouseEventArgs::LBMouseUp));
		break;
	case WM_RBUTTONDOWN:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(VideoManager::getInstance()->mousetoViewPos(lParam), MouseEventArgs::RBMouseDown));
		break;
	case WM_RBUTTONUP:
		EventManager::getInstance()->fireEvent(new MouseEventArgs(VideoManager::getInstance()->mousetoViewPos(lParam), MouseEventArgs::RBMouseUp));
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
		DrawManager::getInstance()->DrawOne(e->viewPos.x, e->viewPos.y);
	}
}

void GameApp::keyEvent(const EventArgs*args)
{
	KeyEventArgs * e = (KeyEventArgs*)args;
	switch (e->keyType)
	{
	case KeyEventArgs::KeyDown:
		KeyManager::getInstance()->KeyDown(e->key);
		break;
	case KeyEventArgs::KeyUp:
		KeyManager::getInstance()->KeyUp(e->key);
		break;
	}
}

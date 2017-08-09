#//--------------------------------------------------------------------------------------
// File: Tutorial01.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729718.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11_1.h>
#include <directxcolors.h>
#include "resource.h"
#include "GameApp.h"
#include <tchar.h>
#include "manager\VideoManager.h"
#include "..\..\log4cplus-1.2.1-rc1\include\log4cplus\helpers\sleep.h"
#include <time.h>
#include "manager\EventManager.h"
#include "manager\GameTime.h"

using namespace DirectX;


HINSTANCE               g_hInst = nullptr;
HWND                    g_hWnd = nullptr;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK  WndProc(HWND, UINT, WPARAM, LPARAM);


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(GameApp::getInstance()->Init(g_hWnd)))
	{
		return 0;
	}

	// Main message loop
	DWORD lasttime = clock();
	GameTime::getInstance()->setGameStartTime(lasttime / 1000.f);
	GameTime::getInstance()->updateFrameTime(lasttime / 1000.f);
	int numFPS = 60;
	float intervaltime = 1000.f / numFPS;
	float offInter = 0;
	MSG msg = { 0 };

	LARGE_INTEGER nFreq;
	QueryPerformanceFrequency(&nFreq);
	LARGE_INTEGER nStartTime;
	QueryPerformanceCounter(&nStartTime);

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		EventManager::getInstance()->runEvent();


		LARGE_INTEGER nEndTime;
		QueryPerformanceCounter(&nEndTime);


		//GameTime::getInstance()->updateFrameTime(clock() / 1000.f);
		GameTime::getInstance()->updateFrameTime(((double)nEndTime.QuadPart - nStartTime.QuadPart) / (double)nFreq.QuadPart);
		GameApp::getInstance()->Render();
		CLOCKS_PER_SEC;

		DWORD newtime = clock();
		float interval = newtime - lasttime;	//执行上面的代码所耗费时间

		if (interval < intervaltime)
		{
			Sleep(intervaltime - interval);
		}
		lasttime = newtime;
		/*
		int temp = interval * numFPS;
		if (temp < 1000)
		{
			float tempInterval = intervaltime + offInter - interval;
			DWORD tempIntervalDWORD = tempInterval;
			offInter = tempInterval - tempIntervalDWORD;
			Sleep(tempIntervalDWORD * 2);
		}*/

		if (newtime > 10000)
		{
			//break;
		}
	}
	GameApp::getInstance()->Close();
	return (int)msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _T("TutorialWindowClass");
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, DEFAULT_VIEW_W, DEFAULT_VIEW_H };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(_T("TutorialWindowClass"), _T("Direct3D 11 Tutorial 1: Direct3D 11 Basics"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GameApp::theGameApp->WndProc(hWnd, message, wParam, lParam);
}


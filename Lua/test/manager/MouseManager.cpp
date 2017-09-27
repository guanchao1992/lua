#include "MouseManager.h"
#include "..\base\RList.h"
#include "EventManager.h"


SingletonClaseCpp(MouseManager);

MouseManager::MouseManager()
	: m_bLBDown(false)
	, m_bRBDown(false)
	, m_bLever(false)
{
}

MouseManager::~MouseManager()
{
}

bool MouseManager::Init()
{
	return true;
}

void MouseManager::Clear()
{
}

void MouseManager::mouseEvent(const Vector2&viewPos, MouseEventArgs::MouseType mouseType)
{
	m_mousePos = viewPos;
	switch (mouseType)
	{
	case MouseEventArgs::LBMouseDown:
		LBDown();
		break;
	case MouseEventArgs::LBMouseUp:
		LBUp();
		break;
	case MouseEventArgs::RBMouseDown:
		RBDown();
		break;
	case MouseEventArgs::RBMouseUp:
		RBUp();
		break;
	case MouseEventArgs::MouseMove:
		Move();
		break;
	case MouseEventArgs::MouseLever:
		Lever();
		break;
	}
	EventManager::getInstance()->fireEvent(new MouseEventArgs(viewPos, mouseType));
}

void MouseManager::LBDown()
{
	m_bLBDown = true;
}

void MouseManager::LBUp()
{
	m_bLBDown = false;
}

void MouseManager::RBDown()
{
	m_bRBDown = true;
}

void MouseManager::RBUp()
{
	m_bRBDown = false;
}

void MouseManager::Move()
{
	if (m_bLever)
	{
		m_bLever = false;
	}
}

void MouseManager::Lever()
{
	m_bLever = true;
}

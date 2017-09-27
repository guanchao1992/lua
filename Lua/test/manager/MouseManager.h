#pragma once
#include "..\base\Singleton.h"
#include "..\base\Ref.h"
#include <list>
#include <map>
#include "EventManager.h"
#include <wtypes.h>
#include "..\base\Vector3.h"


class MouseManager
{
	SingletonClase(MouseManager);
public:
	MouseManager();
	~MouseManager();
public:
	bool Init();
	void Clear();
	void mouseEvent(const Vector2&viewPos, MouseEventArgs::MouseType mouseType);

	void LBDown();
	void LBUp();
	void RBDown();
	void RBUp();
	void Move();
	void Lever();

	inline bool isLBDown() { return m_bLBDown; }
	inline bool isRBDown() { return m_bRBDown; }
	inline bool isLever() { return m_bLever; }

	const Vector2&getMousePos() { return m_mousePos; }
private:
	Vector2			m_mousePos;		//在屏幕上的点
	bool			m_bLBDown;
	bool			m_bRBDown;
	bool			m_bLever;
};

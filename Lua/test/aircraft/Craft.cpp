#include "Craft.h"
#include <minwindef.h>
#include "..\base\NodeList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"
#include "..\manager\KeyManager.h"

#define MaxSpeed 500
#define RESISTANCE_NUM  1000
#define CHANGE_SPEED 3000

namespace aircraft
{

	Craft::Craft()
		:m_speed(0, 0, 0)
	{
	}

	Craft::~Craft()
	{
	}

	Craft* Craft::create()
	{
		Craft* ret = new Craft();
		ret->init();
		ret->autorelease();
		return ret;
	}

	bool Craft::init()
	{
		DrawSolidRect(Rect2D(0,0,200,200),0x000000ff);
		return true;
	}

	void Craft::updateTime(float t)
	{
		if (KeyManager::getInstance()->IsKeyDown(VK_A))
		{
			left(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_D))
		{
			right(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_W))
		{
			up(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_S))
		{
			down(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_E))
		{
			setRotateX(getRotateX()+1);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_Q))
		{
			setRotateY(getRotateY() + 1);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_Z))
		{
			setRotateZ(getRotateZ() + 1);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_SPACE))
		{
			setRotate(Vector3(0, 0, 0));
		}
		move(t);
		resistance(t);
	}

	void Craft::resistance(float t)
	{
		if (m_speed.x > RESISTANCE_NUM * t)
		{
			m_speed.x -= RESISTANCE_NUM * t;
		}
		else if (m_speed.x < -RESISTANCE_NUM * t)
		{
			m_speed.x += RESISTANCE_NUM * t;
		}
		else
		{
			m_speed.x = 0;
		}
		if (m_speed.y > RESISTANCE_NUM * t)
		{
			m_speed.y -= RESISTANCE_NUM * t;
		}
		else if (m_speed.y < -RESISTANCE_NUM * t)
		{
			m_speed.y += RESISTANCE_NUM * t;
		}
		else
		{
			m_speed.y = 0;
		}
	}

	void Craft::move(float t)
	{
		setPosition(getPosition() + m_speed*t);
		if (m_positoin.x > 800) {
			m_positoin.x = 800;
			if (m_speed.x > 0)
			{
				m_speed.x > 0;
			}
		}
		if (m_positoin.x < 0) {
			m_positoin.x = 0;
			if (m_speed.x < 0)
			{
				m_speed.x = 0;
			}
		}
		if (m_positoin.y > 600) {
			m_positoin.y = 600;
			if (m_speed.y > 0)
			{
				m_speed.y = 0;
			}
		}
		if (m_positoin.y < 0) {
			m_positoin.y = 0;
			if (m_speed.y < 0)
			{
				m_speed.y = 0;
			}
		}

	}

	void Craft::left(float t)
	{
		m_speed.x -= CHANGE_SPEED*t;
		if (m_speed.x < -MaxSpeed)
		{
			m_speed.x = -MaxSpeed;
		}
	}

	void Craft::right(float t)
	{
		m_speed.x += CHANGE_SPEED*t;
		if (m_speed.x > MaxSpeed)
		{
			m_speed.x = MaxSpeed;
		}
	}

	void Craft::up(float t)
	{
		m_speed.y += CHANGE_SPEED*t;
		if (m_speed.y > MaxSpeed)
		{
			m_speed.y = MaxSpeed;
		}
	}

	void Craft::down(float t)
	{
		m_speed.y += -CHANGE_SPEED*t;
		if (m_speed.y < -MaxSpeed)
		{
			m_speed.y = -MaxSpeed;
		}
	}

}

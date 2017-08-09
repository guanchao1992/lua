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
		:m_speed(0, 0)
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
		DrawRect(Rect2D(0,0,50,50),0x00ee22ff);
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
		move(t);
		resistance(t);
	}

	void Craft::resistance(float t)
	{
		if (m_speed.getPositionX() > RESISTANCE_NUM * t)
		{
			m_speed.offsetX(-RESISTANCE_NUM * t);
		}
		else if (m_speed.getPositionX() < -RESISTANCE_NUM * t)
		{
			m_speed.offsetX(RESISTANCE_NUM * t);
		}
		else
		{
			m_speed.setPositionX(0);
		}
		if (m_speed.getPositionY() > RESISTANCE_NUM * t)
		{
			m_speed.offsetY(-RESISTANCE_NUM * t);
		}
		else if (m_speed.getPositionY() < -RESISTANCE_NUM * t)
		{
			m_speed.offsetY(RESISTANCE_NUM * t);
		}
		else
		{
			m_speed.setPositionY(0);
		}
	}

	void Craft::move(float t)
	{
		setPosition(getPosition() + m_speed*t);
	}

	void Craft::left(float t)
	{
		m_speed.offsetX(-CHANGE_SPEED*t);
		if (m_speed.getPositionX() < -MaxSpeed)
		{
			m_speed.setPositionX(-MaxSpeed);
		}
	}

	void Craft::right(float t)
	{
		m_speed.offsetX(CHANGE_SPEED*t);
		if (m_speed.getPositionX() > MaxSpeed)
		{
			m_speed.setPositionX(MaxSpeed);
		}
	}

	void Craft::up(float t)
	{
		m_speed.offsetY(CHANGE_SPEED*t);
		if (m_speed.getPositionY() > MaxSpeed)
		{
			m_speed.setPositionY(MaxSpeed);
		}
	}

	void Craft::down(float t)
	{
		m_speed.offsetY(-CHANGE_SPEED*t);
		if (m_speed.getPositionY() < -MaxSpeed)
		{
			m_speed.setPositionY(-MaxSpeed);
		}
	}

}

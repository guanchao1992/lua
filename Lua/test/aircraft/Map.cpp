#include "Map.h"
#include <minwindef.h>
#include "..\base\NodeList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"
#include "..\manager\KeyManager.h"

namespace aircraft
{
	Map::Map()
		: m_startGame(false)
		, m_pause(false)
		, m_layout(nullptr)
		, m_bgDraw(nullptr)
		, m_controlCraft(nullptr)
		, m_leftD(false)
		, m_rightD(false)
		, m_upD(false)
		, m_downDn(false)
	{
	}

	Map::~Map()
	{
	}

	void Map::startGame()
	{
		if (m_layout == nullptr)
		{
			m_layout = DrawManager::createLayout<DrawLayout>(0);
			DrawManager::getInstance()->addLayout(m_layout);

			m_bgDraw = DrawNode::create();
			m_layout->addChild(m_bgDraw, -100);
		}
		m_startGame = true;
		drawBG();
		KeyManager::getInstance()->RegKey(VK_A, "aircraft_map_left", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_D, "aircraft_map_right", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_S, "aircraft_map_down", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_W, "aircraft_map_up", KeyManager::KeyEventType::Down);

		m_controlCraft = Craft::create();
		m_layout->addChild(m_controlCraft);
	}

	void Map::clearGame()
	{
		if (m_layout == nullptr)
		{
			m_layout->removeFromeParent();
		}
		m_startGame = false;

		KeyManager::getInstance()->ClearKey(VK_A, "aircraft_map_left");
		KeyManager::getInstance()->ClearKey(VK_D, "aircraft_map_right");
		KeyManager::getInstance()->ClearKey(VK_W, "aircraft_map_up");
		KeyManager::getInstance()->ClearKey(VK_S, "aircraft_map_down");
	}

	void Map::updateMap(float t)
	{
		if (m_startGame == false)
		{
			return;
		}
		if (m_pause)
		{
			return;
		}
		if (m_controlCraft)
		{
			m_controlCraft->updateTime(t);
		}
	}

	void Map::drawBG()
	{
		m_bgDraw->clear();
		m_bgDraw->DrawRect(Rect2D(0, 0, 100 * BOXSIZE, 100 * BOXSIZE), 0xeeeeeeff);
	}
}

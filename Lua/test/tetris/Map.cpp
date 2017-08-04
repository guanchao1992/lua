#include "Map.h"
#include <minwindef.h>
#include "..\base\NodeList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"

namespace tetris
{

	Map::Map(UINT width,UINT height)
		: m_startGame(false)
		, m_pause(false)
		, m_nowBlock(nullptr)
		, m_speed(500)
		, m_time(0.f)
		, m_layout(nullptr)
		, m_bgDraw(nullptr)
		, m_leftDown(false)
		, m_rightDown(false)
		, m_downDown(false)
		, m_upDown(false)
	{
		m_nextBlock = NodeList::create();
		m_nextBlock->retain();
		resetSize(width, height);
		m_timer = GameTime::getInstance()->addTimer("MapEventKeyTimer", 0.15f, -1, std::bind(&Map::keyEventTimer, this, std::placeholders::_1));
	}

	Map::~Map()
	{
		m_nextBlock->release();
		m_nextBlock = nullptr;
		m_timer->stop();
		m_timer = nullptr;
	}

	void Map::resetSize(UINT width, UINT height)
	{
		m_width = width;
		m_height = height;
		m_boxMap.resize(width * height);
		for (auto &it : m_boxMap)
		{
			it.m_type = Box_Null;
		}
	}

	void Map::startGame()
	{
		if (m_layout == nullptr)
		{
			m_layout = DrawManager::createLayout(0);
			DrawManager::getInstance()->addLayout(m_layout);

			m_bgDraw = DrawNode::create();
			m_layout->addChild(m_bgDraw, -100);

		}
		m_startGame = true;
		drawBG();
	}
	
	void Map::clearGame()
	{
		if (m_layout == nullptr)
		{
			m_layout->removeFromeParent();
		}
		m_startGame = false;
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
		if (m_disLines.size() != 0)
		{
			disingLine();
			return;
		}
		m_time += t;
		
		float df = 100.f / (m_speed + 100);
		if (m_time > df)
		{
			m_time -= df;
			down();
		}
		if (m_nowBlock == nullptr)
		{
			popNowBlock();
		}

	}

	void Map::popNowBlock()
	{
		if (m_nowBlock && m_nowBlock == m_nextBlock->Front())
		{
			m_layout->removeChild(m_nextBlock->Front());
			m_nextBlock->PopFront();
		}

		for (int i = 0; i < 10; ++i)
		{
			if (m_nextBlock->getCount() < 10)
			{
				auto b = Block::create((BlockType)(rand() % BlockType_Max), BoxType(rand() % Box_Max), Size(BOXSIZE, BOXSIZE));
				m_nextBlock->PushBack(b);
				m_layout->addChild(b);
			}
		}

		for (int i = 0; i < m_nextBlock->getCount(); ++i)
		{
			auto b = m_nextBlock->getNodeAtIndex(i);
			if (b)
			{
				b->setPosition(Position2D((m_width + 4 + 3 * i)*BOXSIZE, (m_height)*BOXSIZE));
			}
		}

		m_nowBlock = dynamic_cast<Block*>(m_nextBlock->Front());
		m_nowBlockX = m_width / 2;
		m_nowBlockY = m_height + 2;
		updateNowBlockPos();
	}

	bool Map::collision(const Block*b, int blockX, int blockY)
	{
		for (int i = 0; i < b->getBoxCount(); ++i)
		{
			BlockBoxPos bp = b->getBlockBoxPos(i);
			int x = POSTOX(bp) + blockX;
			int y = POSTOY(bp) + blockY;
			if (x < 0 || x >= m_width || y < 0)
			{
				return true;
			}
			UINT index = x + y*m_width;
			if (index < m_width * m_height && m_boxMap[index].m_type != Box_Null)
			{
				return true;
			}
		}
		return false;
	}

	void Map::BlockGOMap(const Block*b, int blockX, int blockY)
	{
		for (int i = 0; i < b->getBoxCount(); ++i)
		{
			BlockBoxPos bp = b->getBlockBoxPos(i);
			int x = POSTOX(bp) + blockX;
			int y = POSTOY(bp) + blockY;
			if (x >= 0 && x < m_width && y >= 0 && y < m_height)
			{
				UINT index = x + y*m_width;
				m_boxMap[index] = b->getBoxAtt();
			}
		}
		drawBG();
	}

	void Map::disLine()
	{
		for (int h = 0; h < m_height; ++h)
		{
			bool isDis = true;
			for (int w = 0; w < m_width; ++w)
			{
				if (m_boxMap[h*m_width + w].m_type == Box_Null)
				{
					isDis = false;
					break;
				}
			}
			if (isDis)
			{
				m_disLines.push_back(h);
			}
		}
		m_numDis = 0;
	}

	void Map::disingLine()
	{
		if (m_numDis >= m_width)
		{
			m_disLines.clear();
			return;
		}
		for (auto line : m_disLines)
		{
			UINT index = line* m_width + m_numDis;
			if (index < m_boxMap.size())
			{
				m_boxMap[index].m_type = Box_Null;
			}
		}
		if (m_numDis > m_width)
		{
			m_disLines.clear();
		}
	}

	void Map::updateNowBlockPos()
	{
		if (m_nowBlock)
		{
			m_nowBlock->setPosition(Position2D(m_nowBlockX* BOXSIZE, m_nowBlockY*BOXSIZE));
		}
	}

	void Map::drawBG()
	{
		m_bgDraw->clear();
		for (int i = 0; i < m_boxMap.size(); ++i)
		{
			Rect2D rect((i % m_width)*BOXSIZE, (i / m_width) * BOXSIZE, BOXSIZE, BOXSIZE);
			if (m_boxMap[i].m_type != Box_Null)
			{
				m_bgDraw->DrawImage(m_boxMap[i].getBoxShow(), rect);
			}
		}
	}

	void Map::keyDown(UINT key)
	{
		if (!m_leftDown && !m_rightDown && !m_downDown && !m_upDown)
		{
			m_timer->restart();
		}
		switch (key)
		{
		case VK_A:
			left();
			keyLeft(true);
			keyRight(false);
			break;
		case VK_D:
			right();
			keyRight(true);
			keyLeft(false);
			break;
		case VK_S:
			down();
			keyDown(true);
			break;
		case VK_W:
			rotateLeft();
			keyUp(true);
			break;
		}
	}

	void Map::keyUp(UINT key)
	{
		switch (key)
		{
		case VK_A:
			keyLeft(false);
			break;
		case VK_D:
			keyRight(false);
			break;
		case VK_S:
			keyDown(false);
			break;
		case VK_W:
			keyUp(false);
			break;
		}
	}

	void Map::keyEventTimer(float t)
	{
		if (!m_startGame || m_pause || m_nowBlock == nullptr)
		{
			return;
		}
		if (m_leftDown)
		{
			left();
		}else if (m_rightDown)
		{
			right();
		}

		if (m_downDown)
		{
			down();
		}
		if (m_upDown)
		{
			rotateLeft();
		}
	}

	void Map::left()
	{
		if (m_nowBlock == nullptr)
			return;
		--m_nowBlockX;
		if (collision(m_nowBlock, m_nowBlockX, m_nowBlockY))
		{
			++m_nowBlockX;
			return ;
		}
		updateNowBlockPos();
	}

	void Map::right()
	{
		if (m_nowBlock == nullptr)
			return;
		++m_nowBlockX;
		if (collision(m_nowBlock, m_nowBlockX, m_nowBlockY))
		{
			--m_nowBlockX;
			return;
		}
		updateNowBlockPos();
	}

	void Map::down()
	{
		if (m_nowBlock == nullptr)
			return;
		--m_nowBlockY;
		if (collision(m_nowBlock, m_nowBlockX, m_nowBlockY))
		{
			++m_nowBlockY;
			BlockGOMap(m_nowBlock, m_nowBlockX, m_nowBlockY);
			popNowBlock();
			return;
		}
		updateNowBlockPos();
	}

	void Map::rotateLeft()
	{
		if (m_nowBlock == nullptr)
			return;
		m_nowBlock->rotateLeft();
		if (collision(m_nowBlock, m_nowBlockX, m_nowBlockY))
		{
			m_nowBlock->rotateRight();
			return;
		}
		updateNowBlockPos();
	}

	void Map::rotateRight()
	{
		if (m_nowBlock == nullptr)
			return;
		m_nowBlock->rotateRight();
		if (collision(m_nowBlock, m_nowBlockX, m_nowBlockY))
		{
			m_nowBlock->rotateLeft();
			return;
		}
		updateNowBlockPos();
	}
}

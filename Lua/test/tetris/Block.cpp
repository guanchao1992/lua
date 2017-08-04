#include "Block.h"
#include <minwindef.h>

namespace tetris
{

	Block::Block(BlockType type, BoxType boxType, const Size&boxSize)
		: m_blockType(type)
		, m_boxAtt(boxType)
		, m_boxSize(boxSize)
	{
	}

	Block::~Block()
	{
	}

	Block* Block::create(BlockType type, BoxType boxType, const Size&boxSize)
	{
		Block* block = new Block(type, boxType, boxSize);
		block->autorelease();
		block->init();
		return block;
	}

	void Block::init()
	{
		reset();
	}

	void Block::reset()
	{
		m_vecBox.clear();
		switch (m_blockType)
		{
		case tetris::BlockType_1:
			m_vecBox.push_back(MAKELONG(-1, -1));
			m_vecBox.push_back(MAKELONG(-1, 0));
			m_vecBox.push_back(MAKELONG(0, 0));
			m_vecBox.push_back(MAKELONG(0, 1));
			m_boxAtt.m_type = Box_1;
			break;
		case tetris::BlockType_2:
			m_vecBox.push_back(MAKELONG(-1, 1));
			m_vecBox.push_back(MAKELONG(-1, 0));
			m_vecBox.push_back(MAKELONG(0, 0));
			m_vecBox.push_back(MAKELONG(0, -1));
			m_boxAtt.m_type = Box_2;
			break;
		case tetris::BlockType_3:
			m_vecBox.push_back(MAKELONG(-1, -1));
			m_vecBox.push_back(MAKELONG(-1, 0));
			m_vecBox.push_back(MAKELONG(-1, 1));
			m_vecBox.push_back(MAKELONG(0, 0));
			m_boxAtt.m_type = Box_3;
			break;
		case tetris::BlockType_4:
			m_vecBox.push_back(MAKELONG(-1, -1));
			m_vecBox.push_back(MAKELONG(-1, 0));
			m_vecBox.push_back(MAKELONG(-1, 1));
			m_vecBox.push_back(MAKELONG(0, 1));
			m_boxAtt.m_type = Box_4;
			break;
		case tetris::BlockType_5:
			m_vecBox.push_back(MAKELONG(-1, -1));
			m_vecBox.push_back(MAKELONG(-1, 0));
			m_vecBox.push_back(MAKELONG(-1, 1));
			m_vecBox.push_back(MAKELONG(0, -1));
			m_boxAtt.m_type = Box_5;
			break;
		case tetris::BlockType_6:
			m_vecBox.push_back(MAKELONG(-1, -1));
			m_vecBox.push_back(MAKELONG(-1, 0));
			m_vecBox.push_back(MAKELONG(0, -1));
			m_vecBox.push_back(MAKELONG(0, 0));
			m_boxAtt.m_type = Box_6;
			break;
		case tetris::BlockType_7:
			m_vecBox.push_back(MAKELONG(0, -1));
			m_vecBox.push_back(MAKELONG(0, 0));
			m_vecBox.push_back(MAKELONG(0, 1));
			m_vecBox.push_back(MAKELONG(0, 2));
			m_boxAtt.m_type = Box_7;
			break;
		default:
			m_vecBox.push_back(MAKELONG(0, 0));
			m_boxAtt.m_type = Box_1;
			break;
		}
	}

	UINT Block::getBoxCount() const
	{
		return m_vecBox.size();
	}

	BlockBoxPos Block::getBlockBoxPos(UINT index) const
	{
		if (index >= m_vecBox.size())
		{
			return NULL;
		}
		return m_vecBox[index];
	}

	void Block::rotateLeft()
	{
		for (auto &it : m_vecBox)
		{
			int x = POSTOX(it);
			int y = POSTOY(it);
			it = MAKELONG(y, -x);
		}
		doRedraw();
	}

	void Block::rotateRight()
	{
		for (auto &it : m_vecBox)
		{
			int x = POSTOX(it);
			int y = POSTOY(it);
			it = MAKELONG(-y, x);
		}
		doRedraw();
	}

	void Block::redraw()
	{
		clear();
		std::string color = m_boxAtt.getBoxShow();
		for (auto it : m_vecBox)
		{
			float x = (POSTOX(it)) * m_boxSize.getHeight();
			float y = (POSTOY(it)) * m_boxSize.getWidth();
			DrawImage(color, Rect2D(x, y, m_boxSize.getWidth(), m_boxSize.getHeight()));
		}
		__super::redraw();
	}
}

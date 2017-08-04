#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "Block.h"
#include "..\draw\DrawLayout.h"
#include "..\manager\GameTime.h"

#define BOXSIZE	15.f

namespace tetris
{
	class Map
	{
	public:
		Map(UINT width,UINT height);
		~Map();

		void resetSize(UINT width, UINT height);
		void startGame();
		void clearGame();
		void updateMap(float t);
		bool collision(const Block*b, int blockX, int blockY);
		void BlockGOMap(const Block*b, int blockX, int blockY);
		void disLine();
		void disingLine();

		void popNowBlock();

		void updateNowBlockPos();

		void drawBG();

		void keyDown(UINT key);
		void keyUp(UINT key);

		void keyLeft(bool isdown) { m_leftDown = isdown; }
		void keyRight(bool isdown) { m_rightDown = isdown; }
		void keyDown(bool isdown) { m_downDown = isdown; }
		void keyUp(bool isdown) { m_upDown = isdown; }

		void keyEventTimer(float t);
		void left();
		void right();
		void down();
		void rotateLeft();
		void rotateRight();

	private:
		UINT					m_width;
		UINT					m_height;
		std::vector<BoxAtt>		m_boxMap;

		bool					m_startGame;
		bool					m_pause;

		DrawLayout*				m_layout;
		DrawNode*				m_bgDraw;
		//std::list<Block>		m_nextBlock;			//包括正在下落的
		NodeList*				m_nextBlock;
		Block*					m_nowBlock;
		int						m_speed;
		float					m_time;

		int						m_nowBlockX;
		int						m_nowBlockY;

		std::vector<int>		m_disLines;
		int						m_numDis;

		bool			m_leftDown;
		bool			m_rightDown;
		bool			m_downDown;
		bool			m_upDown;
		Timer*			m_timer;
	};
}

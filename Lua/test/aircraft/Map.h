#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "..\draw\DrawLayout.h"
#include "..\draw\DrawNode.h"
#include "..\manager\GameTime.h"
#include "Craft.h"

#define BOXSIZE	15.f

namespace aircraft
{
	class Map
	{
	public:
		Map();
		~Map();

		void startGame();
		void clearGame();
		void updateMap(float t);
		void drawBG();

	private:
		bool					m_leftD;
		bool					m_rightD;
		bool					m_upD;
		bool					m_downDn;

		bool					m_startGame;
		bool					m_pause;

		DrawLayout*				m_layout;
		DrawNode*				m_bgDraw;

		Craft*					m_controlCraft;
	};
}

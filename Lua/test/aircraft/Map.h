#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "..\draw\DrawLayout.h"
#include "..\draw\DrawNode.h"
#include "..\manager\GameTime.h"
#include "Craft.h"

#define BOXSIZE	15.f

class b2DrawDebug;
class b2World;
class b2Body;

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

		void initBox2D();
		b2Body* createCircleBox2D(const Vector2& pos, float radius);

		Craft* addCraft(const Vector2& pos);

	private:
		bool					m_leftD;
		bool					m_rightD;
		bool					m_upD;
		bool					m_downDn;

		bool					m_startGame;
		bool					m_pause;

		//DrawLayout*				m_layout;
		DrawNode*				m_bgDraw;

		Craft*					m_controlCraft;
		RefList*				m_listCraft;

		b2World*				m_b2World;
		b2DrawDebug*			m_drawDebug;
	};
}

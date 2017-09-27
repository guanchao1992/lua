#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "Craft.h"
#include "..\base\RList.h"
#include "ContactListener.h"

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

		void RegAllEvent();
		void ClearAllEvent();

		template<class CRAFT>
		CRAFT* addCraft(const Vector2& pos, UINT maskBits);

		inline b2World*getb2World() { return m_b2World; }
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
		RList<Craft>*			m_listCraft;

		b2World*				m_b2World;
		b2DrawDebug*			m_drawDebug;
		ContactListener*		m_contactListener;
	};

	template<class CRAFT>
	CRAFT* Map::addCraft(const Vector2& pos,UINT maskBits)
	{
		CRAFT* ret = new CRAFT(this);
		ret->init(maskBits);
		ret->autorelease();
		ret->setPosition(pos);
		m_listCraft->PushBack(ret);
		return ret;
	}
}

#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "..\draw\DrawLayout.h"
#include "..\draw\DrawNode.h"
#include "..\manager\GameTime.h"

#define BOXSIZE	15.f

namespace aircraft
{
	class Craft : public DrawNode
	{
	protected:
		Craft();
	public:
		~Craft();

		static Craft* create();
		bool init();

		void updateTime(float t);
		void resistance(float t);
		void move(float t);
		void left(float t);
		void right(float t);
		void up(float t);
		void down(float t);
	private:
		Position2D	m_speed;
	};
}

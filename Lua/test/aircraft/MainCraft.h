#pragma once
#include "Craft.h"

class b2World;
class b2Body;
class DrawNode;

namespace aircraft
{
	class MainCraft : public Craft
	{
	protected:
		friend class Map;
		MainCraft(Map*map);
	public:
		~MainCraft();
		virtual void initBody(UINT maskBits) override;
		virtual void initDraw() override;
		virtual void updateTime(float t) override;

		virtual	void contactCraft(const Craft*other) override;

		virtual void bullet(const Vector2&des) override;
	};
}

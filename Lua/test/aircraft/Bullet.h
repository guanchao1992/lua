#pragma once
#include "Craft.h"

class b2World;
class b2Body;
class DrawNode;

namespace aircraft
{
	class Bullet : public Craft
	{
	protected:
		friend class Map;
		Bullet(Map*map);
	public:
		~Bullet();
		virtual void initBody() override;
		virtual void initDraw() override;
		virtual void updateTime(float t) override;

		virtual	void contactCraft(const Craft*other) override;
	};
}

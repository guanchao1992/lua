#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "..\base\Ref.h"
#include "..\base\Vector2.h"
#include "aircraft_config.h"

#define BOXSIZE		15.f
class b2World;
class b2Body;
class DrawNode;

#define BULLET_SPEED_NUM	20

enum CollisionMake
{
	CollisionMake_Craft		= 0x0001,
	CollisionMake_MyCraft	= 0x0002,
	CollisionMake_Bullet	= 0x0010,
	CollisionMake_ALL		= 0xffff
};

namespace aircraft
{
	class Map;
	class Craft : public Ref
	{
	protected:
		friend class Map;
		Craft(Map* map);
	public:
		~Craft();
		bool init(UINT maskBits);
		virtual void initBody(UINT maskBits);
		virtual void initDraw();
		virtual void updateTime(float t);

		virtual void bullet(const Vector2&des);

		void setTransform(const b2Vec2&pos, float angle);
		void setPosition(const Vector2&pos);
		void setAngle(float angle);//角度
		void ApplyLinearImpulseToCenter(const Vector2&vec);

		Vector2 getPosition();

		void updateTransform();

		inline CraftType getFuncType() { return m_funcType; }

		//和另一个实体接触
		virtual	void contactCraft(const Craft*other);

		inline void setDel(bool del) { m_delete = del; }
		inline bool isDel() { return m_delete; }
	protected:
		void delThis();
	protected:
		Map*			m_map;
		bool			m_delete;
		b2Body*			m_body;
		DrawNode*		m_drawNode;
		CraftType		m_funcType;
	};
}

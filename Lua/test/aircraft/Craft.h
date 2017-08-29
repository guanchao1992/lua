#pragma once
#include <wtypes.h>
#include <vector>
#include <list>
#include "..\base\Ref.h"
#include "..\base\Vector2.h"

#define BOXSIZE		15.f
class b2World;
class b2Body;
class DrawNode;

namespace aircraft
{
	class Craft : public Ref
	{
	protected:
		Craft();
	public:
		~Craft();
		static Craft* create(b2World* world);
		bool init(b2World* world);
		virtual void initBody(b2World* world);
		virtual void initDraw();
		virtual void updateTime(float t);

		void setPosition(const Vector2&pos);
		void setAngle(float angle);//½Ç¶È

		void updateTransform();
		void updatePosition();
		void updateAngle();	
	private:
		b2Body*			m_body;
		DrawNode*		m_drawNode;
	};
}

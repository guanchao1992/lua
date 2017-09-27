#include "Bullet.h"
#include <minwindef.h>
#include "..\base\RList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"
#include "..\manager\KeyManager.h"
#include "..\draw\Draw3DBuffer.h"
#include <Box2D\Box2d.h>
#include <D3DX10math.h>
#include "..\draw\DrawNode.h"
#include "..\draw\DrawLayout.h"
#include <Box2D\Dynamics\b2Body.h>
#include "aircraft_config.h"
#include "Map.h"

#define LINEARIMPULSE_NUM	8000
#define ANGULARIMPULSE_NUM	10	

namespace aircraft
{

	Bullet::Bullet(Map*map)
		: Craft(map)
		, m_remainingTime(5.f)
	{
		m_funcType = CraftType_Bullet;
	}

	Bullet::~Bullet()
	{
	}

	void Bullet::initBody(UINT maskBits)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0 * BOX2D_LENTH_RATIO_RE, 0 * BOX2D_LENTH_RATIO_RE);
		bodyDef.bullet = true;
		m_body = m_map->getb2World()->CreateBody(&bodyDef);
		m_body->SetUserData(this);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(20 / 2 * BOX2D_LENTH_RATIO_RE, 20 / 2 * BOX2D_LENTH_RATIO_RE);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		fixtureDef.filter.categoryBits = CollisionMake_Bullet;
		fixtureDef.filter.maskBits = maskBits;
		fixtureDef.filter.groupIndex = 0;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 0.2f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		fixtureDef.restitution = 0.1;

		// Add the shape to the body.
		m_body->CreateFixture(&fixtureDef);

		m_body->SetLinearDamping(0.0);
		m_body->SetAngularDamping(0.0);
	}

	void Bullet::initDraw()
	{
		m_drawNode = DrawNode::create();
		m_drawNode->DrawSolidCircle(Vector3(0, 0, 0), 10, 0x2222ffff);
		DrawLayout* layout = DrawManager::getInstance()->getLayout(Aircraft_Layout);
		layout->addChild(m_drawNode);
	}

	void Bullet::updateTime(float t)
	{
		if (isDel())
		{
			return;
		}
		m_remainingTime -= t;
		if (m_remainingTime <= 0)
		{
			setDel(true);
		}
	}

	void Bullet::contactCraft(const Craft*other)
	{
		setDel(true);
	}
}

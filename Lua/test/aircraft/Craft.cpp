#include "Craft.h"
#include <minwindef.h>
#include "..\base\RefList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"
#include "..\manager\KeyManager.h"
#include "..\draw\Draw3DBuffer.h"
#include <Box2D\Box2d.h>
#include <D3DX10math.h>
#include "..\draw\DrawNode.h"
#include "..\draw\DrawLayout.h"
#include "aircraft_config.h"
#include <Box2D\Dynamics\b2Body.h>

#define LINEARIMPULSE_NUM	8000
#define ANGULARIMPULSE_NUM	10	

namespace aircraft
{

	Craft::Craft()
		:m_body(nullptr)
	{
	}

	Craft::~Craft()
	{
		if (m_body)
		{
			m_body->SetUserData(nullptr);
		}
	}

	Craft* Craft::create(b2World* world)
	{
		Craft* ret = new Craft();
		ret->init(world);
		ret->autorelease();
		return ret;
	}

	bool Craft::init(b2World* world)
	{
		initBody(world);
		initDraw();
		return true;
	}

	void Craft::initBody(b2World* world)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0 * BOX2D_LENTH_RATIO_RE, 0 * BOX2D_LENTH_RATIO_RE);
		m_body = world->CreateBody(&bodyDef);
		m_body->SetUserData(this);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(40 / 2 * BOX2D_LENTH_RATIO_RE, 40 / 2 * BOX2D_LENTH_RATIO_RE);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		fixtureDef.filter.categoryBits = 0x0001;
		fixtureDef.filter.maskBits = 0xFFFF;
		fixtureDef.filter.groupIndex = 0;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 0.2f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		fixtureDef.restitution = 0.1;

		// Add the shape to the body.
		m_body->CreateFixture(&fixtureDef);

		m_body->SetLinearDamping(5.0);
		m_body->SetAngularDamping(20.0);
	}

	void Craft::initDraw()
	{
		m_drawNode = DrawNode::create();
		m_drawNode->DrawSolidRect(Rect2D(-40 / 2, -40 / 2, 40, 40), 0xff3f32ff);
		DrawLayout* layout = DrawManager::getInstance()->getLayout(Aircraft_Layout);
		layout->addChild(m_drawNode);
	}

	void Craft::updateTime(float t)
	{
		if (KeyManager::getInstance()->IsKeyDown(VK_A))
		{
			m_body->ApplyForceToCenter(Vector2(-LINEARIMPULSE_NUM * t, 0).toBox2d(), true);
			//left(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_D))
		{
			m_body->ApplyForceToCenter(Vector2(LINEARIMPULSE_NUM* t, 0).toBox2d(), true);
			//right(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_W))
		{
			m_body->ApplyForceToCenter(Vector2(0, LINEARIMPULSE_NUM* t).toBox2d(), true);
			//up(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_S))
		{
			m_body->ApplyForceToCenter(Vector2(0, -LINEARIMPULSE_NUM* t).toBox2d(), true);
			//down(t);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_E))
		{
			m_body->ApplyAngularImpulse(ANGULARIMPULSE_NUM * BOX2D_LENTH_RATIO_RE*t, true);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_Q))
		{
			m_body->ApplyAngularImpulse(-ANGULARIMPULSE_NUM* BOX2D_LENTH_RATIO_RE*t, true);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_SPACE))
		{
			m_body->SetTransform(Vector2(0, 200).toBox2d(), 0);
			m_body->SetAwake(true);
		}
	}

	void Craft::setPosition(const Vector2&pos)
	{
		m_body->SetTransform(Vector2(0, 200).toBox2d(), m_body->GetAngle());
		m_body->SetAwake(true);
	}

	void Craft::setAngle(float angle)
	{
		m_body->SetTransform(m_body->GetPosition(), -angle * D3DX_PI / 180);
		m_body->SetAwake(true);
	}

	void Craft::updateTransform()
	{
		m_drawNode->setPosition(Vector2(m_body->GetPosition()));
		m_drawNode->setRotateZ(-m_body->GetAngle() / D3DX_PI * 180);
	}

	void Craft::updatePosition()
	{
		m_drawNode->setPosition(Vector2(m_body->GetPosition()));
	}

	void Craft::updateAngle()
	{
		m_drawNode->setRotateZ(-m_body->GetAngle() / D3DX_PI * 180);
	}

}

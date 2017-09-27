#include "MainCraft.h"
#include <minwindef.h>
#include "..\base\RList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\KeyManager.h"
#include "..\draw\Draw3DBuffer.h"
#include <Box2D\Box2d.h>
#include <D3DX10math.h>
#include "..\draw\DrawNode.h"
#include "..\draw\DrawLayout.h"
#include <Box2D\Dynamics\b2Body.h>
#include "aircraft_config.h"
#include "Map.h"
#include "Bullet.h"

#define LINEARIMPULSE_NUM	80
#define ANGULARIMPULSE_NUM	10	

namespace aircraft
{

	MainCraft::MainCraft(Map*map)
		:Craft(map)
	{
		m_funcType = CraftType_MainCraft;
	}

	MainCraft::~MainCraft()
	{
	}

	void MainCraft::initBody(UINT maskBits)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0 * BOX2D_LENTH_RATIO_RE, 0 * BOX2D_LENTH_RATIO_RE);
		m_body = m_map->getb2World()->CreateBody(&bodyDef);
		m_body->SetUserData(this);

		// Define another box shape for our dynamic body.
		/*
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(50 / 2 * BOX2D_LENTH_RATIO_RE, 50 / 2 * BOX2D_LENTH_RATIO_RE);
*/

		b2CircleShape dynamicBox;
		dynamicBox.m_p = b2Vec2(0, 0);
		dynamicBox.m_radius = 25 * BOX2D_LENTH_RATIO_RE;

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		fixtureDef.filter.categoryBits = CollisionMake_MyCraft;
		fixtureDef.filter.maskBits = maskBits;
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

	void MainCraft::initDraw()
	{
		m_drawNode = DrawNode::create();
		//m_drawNode->DrawSolidRect(Rect2D(-50 / 2, -50 / 2, 50, 50), 0x003f32ff);
		m_drawNode->DrawSolidCircle(Vector3(0, 0, 0), 25, 0x003f32ff);
		DrawLayout* layout = DrawManager::getInstance()->getLayout(Aircraft_Layout);
		layout->addChild(m_drawNode);
	}

	void MainCraft::updateTime(float t)
	{
		if (m_body == nullptr)
		{
			return;
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_A))
		{
			m_body->ApplyLinearImpulseToCenter(Vector2(-LINEARIMPULSE_NUM * t, 0).toBox2d(), true);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_D))
		{
			m_body->ApplyLinearImpulseToCenter(Vector2(LINEARIMPULSE_NUM* t, 0).toBox2d(), true);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_W))
		{
			m_body->ApplyLinearImpulseToCenter(Vector2(0, LINEARIMPULSE_NUM* t).toBox2d(), true);
		}
		if (KeyManager::getInstance()->IsKeyDown(VK_S))
		{
			m_body->ApplyLinearImpulseToCenter(Vector2(0, -LINEARIMPULSE_NUM* t).toBox2d(), true);
		}
		Craft::updateTime(t);
	}

	void MainCraft::contactCraft(const Craft*other)
	{
	}

	void MainCraft::bullet(const Vector2&des)
	{
		Bullet*bullet = m_map->addCraft<Bullet>(Vector2(m_body->GetPosition()), CollisionMake_Craft);
		bullet->setTransform(m_body->GetPosition(), m_body->GetAngle());
		Vector2 toPos = des;
		toPos.normalization(BULLET_SPEED_NUM);
		bullet->ApplyLinearImpulseToCenter(toPos);
		bullet->updateTransform();
	}
}

#include "Map.h"
#include <minwindef.h>
#include "..\base\NodeList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"
#include "..\manager\KeyManager.h"
#include <D3DX10math.h>

namespace aircraft
{
	Map::Map()
		: m_startGame(false)
		, m_pause(false)
		, m_layout(nullptr)
		, m_bgDraw(nullptr)
		, m_controlCraft(nullptr)
		, m_leftD(false)
		, m_rightD(false)
		, m_upD(false)
		, m_downDn(false)
		, m_b2World(nullptr)
		, m_b2BoxDraw(nullptr)
	{
	}

	Map::~Map()
	{
		if (m_b2World)
		{
			delete m_b2World;
			m_b2World = nullptr;
		}
	}

	void Map::startGame()
	{
		if (m_layout == nullptr)
		{
			m_layout = DrawManager::createLayout<DrawLayout>(0);
			DrawManager::getInstance()->addLayout(m_layout);

			m_bgDraw = DrawNode::create();
			m_layout->addChild(m_bgDraw, -100);

			m_b2BoxDraw = DrawNode::create();
			m_layout->addChild(m_b2BoxDraw,100);
		}
		m_startGame = true;
		drawBG();
		KeyManager::getInstance()->RegKey(VK_A, "aircraft_map_left", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_D, "aircraft_map_right", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_S, "aircraft_map_down", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_W, "aircraft_map_up", KeyManager::KeyEventType::Down);

		m_controlCraft = Craft::create();
		m_layout->addChild(m_controlCraft);

		initBox2D();
	}

	void Map::initBox2D()
	{
		// Define the gravity vector.
		b2Vec2 gravity(0.0f, -1000.0f);

		// Construct a world object, which will hold and simulate the rigid bodies.
		m_b2World = new b2World(gravity);

		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, 100.0f);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = m_b2World->CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(1000.0f, 40.0f);
		auto groundNode = DrawNode::create();
		groundNode->DrawRect(Rect2D(-1000, -40, 2000, 80), 0x00ff00ff);
		m_layout->addChild(groundNode);
		groundBody->SetUserData(groundNode);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);

	
		for (int i = 0; i < 10; ++i)
		{
			createBox2D(Vector2(400 + i % 10 * 20 + rand() % 20, 400 + i / 10 * 25), Size(20, 20));
		}
	}

	b2Body* Map::createBox2D(const Vector2& pos, const Size& size)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x, pos.y);
		b2Body* body = m_b2World->CreateBody(&bodyDef);

		auto bodyNode = DrawNode::create();
		bodyNode->DrawSolidRect(Rect2D(-size.getWidth() / 2, -size.getHeight() / 2, size.getHeight(), size.getHeight()), 0xff3f32ff);
		m_layout->addChild(bodyNode);
		body->SetUserData(bodyNode);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(size.getWidth() / 2, size.getHeight() / 2);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.1f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		return body;
	}

	void Map::clearGame()
	{
		if (m_layout == nullptr)
		{
			m_layout->removeFromeParent();
		}
		m_startGame = false;

		KeyManager::getInstance()->ClearKey(VK_A, "aircraft_map_left");
		KeyManager::getInstance()->ClearKey(VK_D, "aircraft_map_right");
		KeyManager::getInstance()->ClearKey(VK_W, "aircraft_map_up");
		KeyManager::getInstance()->ClearKey(VK_S, "aircraft_map_down");
	}

	void Map::updateMap(float t)
	{
		if (m_startGame == false)
		{
			return;
		}
		if (m_pause)
		{
			return;
		}
		m_b2BoxDraw->clear();
		if (m_b2World)
		{
			m_b2World->Step(t, 6, 2);
			auto bodyTemp = m_b2World->GetBodyList();
			while (bodyTemp)
			{
				auto userData = static_cast<Node*>(bodyTemp->GetUserData());
				if (userData)
				{
					userData->setPosition(Vector2(bodyTemp->GetPosition().x, bodyTemp->GetPosition().y));
					userData->setRotateZ(-bodyTemp->GetAngle() / D3DX_PI * 180);
				}
				bodyTemp = bodyTemp->GetNext();
			}
		}
		if (m_controlCraft)
		{
			m_controlCraft->updateTime(t);
		}
	}

	void Map::drawBG()
	{
		m_bgDraw->clear();
		//m_bgDraw->DrawRect(Rect2D(0, 0, 100 * BOXSIZE, 100 * BOXSIZE), 0xffff00ff);
		for (int i = 0; i < 100; ++i)
		{
			m_bgDraw->DrawLine(Vector3(0 + 50 * i, 0, 0), Vector3(45 + 50 * i, 0, 0), 0xaaaaaaff);
			m_bgDraw->DrawLine(Vector3(0, 0 + 50 * i, 0), Vector3(0, 45 + 50 * i, 0), 0xaaaaaaff);
			m_bgDraw->DrawLine(Vector3(0, 0, 0 + 50 * i), Vector3(0, 0, 45 + 50 * i), 0xaaaaaaff);
		}
	}
}

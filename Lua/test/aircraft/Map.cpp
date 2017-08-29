#include "Map.h"
#include <minwindef.h>
#include "..\base\RefList.h"
#include "..\manager\DrawManager.h"
#include "..\config.h"
#include "..\manager\GameTime.h"
#include "..\manager\KeyManager.h"
#include <Box2D\Box2d.h>
#include <Box2D\Dynamics\b2Body.h>
#include "b2DrawDebug.h"
#include "..\manager\VideoManager.h"
#include "..\base\Node.h"
#include "aircraft_config.h"


namespace aircraft
{

#define MAP_LAYOUT_INDEX	123321

	Map::Map()
		: m_startGame(false)
		, m_pause(false)
		, m_bgDraw(nullptr)
		, m_controlCraft(nullptr)
		, m_leftD(false)
		, m_rightD(false)
		, m_upD(false)
		, m_downDn(false)
		, m_b2World(nullptr)
		, m_drawDebug(nullptr)
	{
		m_listCraft = RefList::create();
		m_listCraft->retain();
	}

	Map::~Map()
	{
		if (m_b2World)
		{
			delete m_b2World;
			m_b2World = nullptr;
		}
		if (m_drawDebug)
		{
			delete m_drawDebug;
			m_drawDebug = nullptr;
		}
		m_listCraft->release();
		DrawManager::getInstance()->removeLayout(Aircraft_Layout);
	}

	void Map::startGame()
	{
		if (DrawManager::getInstance()->getLayout(Aircraft_Layout) == nullptr)
		{
			DrawLayout* layout = DrawLayout::create(0);
			DrawManager::getInstance()->addLayout(layout, Aircraft_Layout);
			m_bgDraw = DrawNode::create();
			layout->addChild(m_bgDraw, -100);
		}
		m_startGame = true;
		initBox2D();
		drawBG();
		KeyManager::getInstance()->RegKey(VK_A, "aircraft_map_left", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_D, "aircraft_map_right", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_S, "aircraft_map_down", KeyManager::KeyEventType::Down);
		KeyManager::getInstance()->RegKey(VK_W, "aircraft_map_up", KeyManager::KeyEventType::Down);

		m_controlCraft = addCraft(Vector2(0, 300));
	}

	void Map::initBox2D()
	{
		DrawLayout * layout = DrawManager::getInstance()->getLayout("aircraft_layout");
		// Define the gravity vector.
		b2Vec2 gravity(0.0f, 0.0f);

		// Construct a world object, which will hold and simulate the rigid bodies.
		m_b2World = new b2World(gravity);
		m_drawDebug = new b2DrawDebug();
		//m_drawDebug->SetFlags(0xffffffff);
		//m_b2World->SetDebugDraw(m_drawDebug);
		//m_b2World->setfriction
		//m_b2World->SetAutoClearForces(true);

		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(-10.0f* BOX2D_LENTH_RATIO_RE, -10.0f* BOX2D_LENTH_RATIO_RE);

		b2Body* groundBody = m_b2World->CreateBody(&groundBodyDef);
		b2PolygonShape groundBox;
		groundBox.SetAsBox(1200.0f * BOX2D_LENTH_RATIO_RE, 10.0f * BOX2D_LENTH_RATIO_RE);
		groundBody->CreateFixture(&groundBox, 0.0f);
		groundBox.SetAsBox(10.0f * BOX2D_LENTH_RATIO_RE, 1000.0f * BOX2D_LENTH_RATIO_RE);
		groundBody->CreateFixture(&groundBox, 0.0f);

		/*
		auto groundNode = DrawNode::create();
		groundNode->DrawRect(Rect2D(-1200, -10, 2400, 20), 0x00ff00ff);
		groundNode->DrawRect(Rect2D(-10, -1000, 20, 2000), 0x00ff00ff);
		layout->addChild(groundNode);
		groundBody->SetUserData(groundNode);
*/

		Size videoSize = VideoManager::getInstance()->getViewSize();
		b2BodyDef groundBodyDef1;
		groundBodyDef1.position.Set((videoSize.getWidth() + 10.0f)* BOX2D_LENTH_RATIO_RE, (videoSize.getHeight() + 10.0f)* BOX2D_LENTH_RATIO_RE);

		b2Body* groundBody1 = m_b2World->CreateBody(&groundBodyDef1);
		b2PolygonShape groundBox1;
		groundBox1.SetAsBox(1200.0f * BOX2D_LENTH_RATIO_RE, 10.0f * BOX2D_LENTH_RATIO_RE);
		groundBody1->CreateFixture(&groundBox1, 0.0f);
		groundBox1.SetAsBox(10.0f * BOX2D_LENTH_RATIO_RE, 1200.0f * BOX2D_LENTH_RATIO_RE);
		groundBody1->CreateFixture(&groundBox1, 0.0f);

		/*
		auto groundNode1 = DrawNode::create();
		groundNode1->DrawRect(Rect2D(-1200, -10, 2400, 20), 0x00ff00ff);
		groundNode1->DrawRect(Rect2D(-10, -1000, 20, 2000), 0x00ff00ff);
		layout->addChild(groundNode1);
		groundBody1->SetUserData(groundNode1);
*/

		for (int i = 0; i < 10; ++i)
		{
			auto box = Craft::create(m_b2World);
			m_listCraft->PushBack(box);
		}
	}

	b2Body* Map::createCircleBox2D(const Vector2& pos, float radius)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x * BOX2D_LENTH_RATIO_RE, pos.y * BOX2D_LENTH_RATIO_RE);
		b2Body* body = m_b2World->CreateBody(&bodyDef);

		b2CircleShape dynamicBox;
		dynamicBox.m_p = b2Vec2(0, 0);
		dynamicBox.m_radius = radius* BOX2D_LENTH_RATIO_RE;
		dynamicBox.m_type;

		b2FixtureDef fixtureDef;

		fixtureDef.filter.categoryBits = 0x0001;
		fixtureDef.filter.maskBits = 0xFFFF;
		fixtureDef.filter.groupIndex = 0;

		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 0.2f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = 1;
		body->CreateFixture(&fixtureDef);

		body->SetLinearDamping(0.5);
		body->SetAngularDamping(0.5);
		return body;
	}

	Craft* Map::addCraft(const Vector2& pos)
	{
		//b2Body *body = createPolygonBox2D(pos, Size(50, 50));
		auto craft = Craft::create(m_b2World);
		m_listCraft->PushBack(craft);
		//m_layout->addChild(craft);
		return craft;
	}

	void Map::clearGame()
	{
		m_listCraft->Clear();
		/*
		if (m_layout == nullptr)
		{
			m_layout->removeFromeParent();
		}
*/
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
		if (m_b2World)
		{
			m_b2World->ClearDrawDebug();
			m_b2World->Step(t, 6, 2);
			auto bodyTemp = m_b2World->GetBodyList();
			while (bodyTemp)
			{
				auto userData = static_cast<Craft*>(bodyTemp->GetUserData());
				if (userData)
				{
					userData->updatePosition();
					userData->updateAngle();
				}
				bodyTemp = bodyTemp->GetNext();
			}
			m_b2World->DrawDebugData();
		}
		if (m_controlCraft)
		{
			m_controlCraft->updateTime(t);
		}
	}

	void Map::drawBG()
	{
		m_bgDraw->clearBuffer();
		for (int i = 0; i < 11; ++i)
		{
			m_bgDraw->DrawLine(Vector3(0, i * 100, 0), Vector3(1000, i * 100, 0), 0xaaaaaaff);
			m_bgDraw->DrawLine(Vector3(i * 100, 0, 0), Vector3(i * 100, 1000, 0), 0xaaaaaaff);
		}
	}
}

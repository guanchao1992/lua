#include "GameLayout.h"
#include "..\manager\EventManager.h"
#include "..\draw\TextureNode.h"
#include "..\GameApp.h"
#include "..\manager\GameTime.h"



GameLayout::GameLayout()
{
	RegEvent(EventRegType_Mouse, "GameLayout", GameLayout::mouseEvent, 0);
	RegEvent(EventRegType_Key, "GameLayout", GameLayout::keyEvent, 0);
}

GameLayout::~GameLayout()
{
}

bool GameLayout::init()
{
	_node = TextureNode::create();
	_node->DrawImage("1", Rect2D(-100, -80, 200, 160));
	addChild(_node);

	return Node::init();
}

void GameLayout::mouseEvent(const EventArgs*args)
{
	MouseEventArgs * e = (MouseEventArgs *)args;
	if (e->mouseType == MouseEventArgs::MouseMove)
	{
		_node->setPosition(e->viewPos);
	}
}

void GameLayout::keyEvent(const EventArgs*args)
{
	KeyEventArgs *e = (KeyEventArgs*)args;
	if (e->key)
	{
	}
}

void GameLayout::update(float dt)
{

}

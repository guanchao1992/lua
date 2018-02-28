#pragma once
#include "..\draw\DrawLayout.h"

class TextureNode;
class EventArgs;

class GameLayout :public DrawLayout
{
	STATIC_NODE_CREATE(GameLayout);
private:
	GameLayout();
public:
	virtual	~GameLayout();

	bool init();

	void mouseEvent(const EventArgs*args);
	void keyEvent(const EventArgs*args);
private:
	TextureNode *_node;
};


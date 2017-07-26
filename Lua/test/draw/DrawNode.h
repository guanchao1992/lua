#pragma once
#include "..\base\Node.h"
#include <vector>

class ID3D11Buffer;
class DrawNode :public Node
{
	STATIC_NODE_CREATE(DrawNode);
private:
	DrawNode();
public:
	virtual	~DrawNode();
public:
	bool	init();
	virtual void render() ;
	void	DrawOne(float x, float y);
	void	clear();
private:
	std::vector<ID3D11Buffer*>		m_vecDrawBuffer;
};
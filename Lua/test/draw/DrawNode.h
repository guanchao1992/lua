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
	~DrawNode();
public:
	bool	init();
	virtual void render() ;
	void	DrawOne(float x, float y);
private:
	std::vector<ID3D11Buffer*>		m_vecDrawBuffer;
};
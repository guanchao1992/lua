#ifndef ___DrawNode__H__
#define ___DrawNode__H__

#include "Vector3.h"


class DrawNode
{
	DrawNode();
	~DrawNode();

	
	
	Vector3 pos;  // Position
	Vector3 origin;  // origin
	Vector3 scale;  // scale
	Vector3 rotate;  // rotate
};


#endif // !___DrawNode__H__
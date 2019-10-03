#ifndef ___DrawNode__H__
#define ___DrawNode__H__

#include "Vector3.h"
#include <list>
#include "Size.h"


class ID3D11Buffer;
class ID3D11DeviceContext;
//����һ������
class DrawNode
{
public:
	static DrawNode* create(Vector3 origin, Size size);
private:
	DrawNode();
	~DrawNode();

	void Init();
public:
	void redraw();
	void reader(ID3D11DeviceContext *context);
private:
	Vector3 origin;  // origin
	Vector3 scale;  // scale
	Vector3 rotate;  // rotate
	Size size;

	std::list<Vector3> _buffVec;	//�����ζ��㶨λ

	ID3D11Buffer* _drawBuffer = nullptr;
};


#endif // !___DrawNode__H__
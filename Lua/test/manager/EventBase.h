#ifndef ____EventBase_H____
#define ____EventBase_H____


#include <list>
#include <functional>
#include "..\base\Vector2.h"

typedef unsigned long EventType;

enum RegEventType
{
	EventRegType_Null = 0,
	EventRegType_Mouse,
	EventRegType_Key,
	EventRegType_Time,


	EventRegType_Max,
};

class EventArgs
{
public:
	EventArgs(EventType type_) 
		:type(type_)
	{ }
public:
	EventType type;
};

class MouseEventArgs :public EventArgs
{
public:
	enum MouseType
	{
		LBMouseDown,
		LBMouseUp,
		RBMouseDown,
		RBMouseUp,
	};
	MouseEventArgs(const Vector2& viewPos_, MouseType mouseType_) :
		EventArgs(EventRegType_Mouse),
		viewPos(viewPos_),
		mouseType(mouseType_)
	{ }
public:
	Vector2	viewPos;
	MouseType	mouseType;
};

class KeyEventArgs :public EventArgs
{
public:
	enum KeyType
	{
		KeyDown,
		KeyUp,
	};
	KeyEventArgs(KeyType keyType_,unsigned int key_, unsigned int controlKey_, unsigned int virtualKey_) :
		EventArgs(EventRegType_Key),
		keyType(keyType_),
		key(key_),
		controlKey(controlKey_),
		virtualKey(virtualKey_)
	{ }
public:
	unsigned int key;
	unsigned int keyType;
	unsigned int controlKey;	// VK_SHIFT;
	unsigned int virtualKey;
};


class TimeEventArgs : public EventArgs
{
public:
	TimeEventArgs(float dt, bool bfrist, unsigned int num) :
		EventArgs(EventRegType_Time),
		_dt(dt),
		_bfrist(bfrist),
		_num(num),
		_nowNum(0)
	{

	}

public:
	float _dt;
	bool _bfrist;
	unsigned int _num;
	unsigned int _nowNum;
};


#endif
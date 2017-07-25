#pragma once

#define STATIC_NODE_CREATE(ClassType) public:\
static ClassType*create() \
{ ClassType* ret = new ClassType();\
if(ret->init()) { ret->autorelease(); }\
else { delete ret; return nullptr; } return ret; }\

class Ref
{
	friend class ObjectManager;
protected:
	Ref();
public:
	~Ref();
public:
	virtual void retain();
	virtual void release();
	void autorelease();
private:
	int		m_countReference;
	bool	m_delete;
};


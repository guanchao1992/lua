#pragma once

#define STATIC_NODE_CREATE(ClassType) public:\
static ClassType*create() \
{ ClassType* ret = new ClassType();\
if(ret->init()) { ret->autorelease(); }\
else { delete ret; return nullptr; } return ret; }\

//引用计数初始化为1，new出来对象的要手动调用release释放
//autorelease不会改变引用计数
//对象管理器会在引用计数为1时删除对象
class Ref
{
	friend class ObjectManager;
protected:
	Ref();
public:
	virtual ~Ref();
public:
	virtual void retain();
	virtual void release();
	void autorelease();
private:
	int		m_countReference;
	bool	m_delete;
};


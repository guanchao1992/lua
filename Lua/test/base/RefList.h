#pragma once
#include <list>
#include "Ref.h"
#include <functional>

typedef std::function<void(Ref*, bool&outDel,bool&outEnd)> FUNC_ListErgodic;
//这个是会自动排序
class RefList : public Ref
{
protected:
	RefList();
public:
	virtual ~RefList();
public:
	STATIC_NODE_CREATE(RefList);
	bool		init() { return true; };
	void		PushBack(Ref*ref);
	void		PushFront(Ref*ref);
	void		PopBack();
	void		PopFront();
	Ref*		Front();
	Ref*		Back();
	std::list<Ref*>::iterator Begin();
	std::list<Ref*>::iterator End();
	void		Insert(unsigned int index, Ref*ref);

	Ref*		removeFromTag(int tag);
	Ref*		removeFromIndex(unsigned int index);
	void		removeRef(Ref*ref);
	void		Clear();

	Ref*		getRefAtIndex(int index);
	const std::list<Ref*>&	getListRef() { return m_listRef; };
	size_t		getCount() { return m_listRef.size(); };
	void		ergodicFunc(FUNC_ListErgodic func);
private:
	std::list<Ref*> m_listRef;
};


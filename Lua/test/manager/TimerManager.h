#ifndef ____TimerManager_H____
#define ____TimerManager_H____

#include "..\base\Singleton.h"
#include <functional>
#include <map>


typedef std::function<void()> TIMERCALLBACK;

struct Timer
{
	Timer() 
	{ }
	Timer(int timerID_, float intervalTime_, int cumulativeNumber_, int total_, TIMERCALLBACK func_) :
		timerID(timerID),
		intervalTime(intervalTime),
		cumulativeNumber(cumulativeNumber),
		total(total),
		func(func),
		isdelete(false)			
	{}
	int timerID = -1;				//计时器id
	float intervalTime = 1.0f;		//每次执行间隔
	int cumulativeNumber = 0;		//累计执行次数
	int total = 1;					//执行总数
	float cumulativeTimer = 0.f;	//累计执行时间
	bool isdelete = false;			//标记删除
	TIMERCALLBACK func;				//计时器到时回调函数
};

class TimerManager
{
	SingletonClase(TimerManager);
private:
	TimerManager();
public:
	~TimerManager();
public:
	void Init();
	void update(float dt);

	void addTimer(int timerID, const Timer&t);
	void removeTimer(int timerID);
private:
	float _accumulatedTime;//累计时间

	std::map<int, Timer> _mapTimers;
};


#endif
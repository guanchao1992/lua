#include "TimerManager.h"
#include "..\..\MyTool\tool_log.h"
#include "..\config.h"
#include "..\..\MyLua\ctolua.h"

SingletonClaseCpp(TimerManager);


TimerManager::TimerManager()
	:_accumulatedTime(0.f)
{
}


TimerManager::~TimerManager()
{
}

void TimerManager::Init()
{

}

void TimerManager::update(float dt)
{
	_accumulatedTime += dt;
	for (auto it = _mapTimers.begin(); it != _mapTimers.end(); )
	{
		Timer &t = it->second;;
		if (t.isdelete)
		{
			it = _mapTimers.erase(it);
		}
		else
		{

			t.cumulativeTimer += dt;
			if (t.cumulativeTimer > t.intervalTime)
			{
				t.func();
				if (++t.cumulativeNumber >= t.total)
				{
					t.isdelete = true;
				}
			}
			it++;
		}
	}
	static float ttt = 0.f;;
	ttt += dt;
	//printf("¼ÆÊ±:%f\n", ttt);
}

void TimerManager::addTimer(int timerID, const Timer&t)
{
	if (timerID == -1)
	{
		for (size_t i = -2; i < 10000; i--)
		{
			if (_mapTimers.find(i) == _mapTimers.end())
			{
				_mapTimers[i] = t;
				return;
			}
		}
	}
	else
	{
		_mapTimers[timerID] = t;
	}
}

void TimerManager::removeTimer(int timerID)
{
	_mapTimers.erase(timerID);
}


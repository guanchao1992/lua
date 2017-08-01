#include "GameTime.h"
#include "..\base\NodeList.h"
#include <directxcolors.h>
#include <tchar.h>
#include "..\..\log4cplus-1.2.1-rc1\include\log4cplus\helpers\sleep.h"
#include <time.h>
#include <windows.h>


Timer* Timer::create(int id, float span, int loop, TIMER_FUNC func)
{
	Timer* ret = new Timer();
	 ret->autorelease(); 
	 ret->m_id = id;
	 ret->m_span = span;
	 ret->m_loop = loop;
	 ret->m_func = func;
	 return ret;
}

void Timer::stop()
{
	m_stop = true;
}

void Timer::pause(bool p)
{
	m_pause = p;
}

SingletonClaseCpp(GameTime);

GameTime::GameTime()
	: m_startSeconds(.0f)
	, m_seconds(.0f)
	, m_startFrameSeconds(.0f)
	, m_elapsedSeconds(.0f)
{
}

GameTime::~GameTime()
{
}

void GameTime::setGameStartTime(double sec)
{
	m_startFrameSeconds = sec;
}

void GameTime::updateFrameTime(double sec)
{
	float elapsed = sec - m_startFrameSeconds;
	m_startFrameSeconds = sec;
	updateTime(elapsed);

}

float GameTime::getElapsedTime()
{
	m_elapsedSeconds = timeGetTime() / 1000.f - m_startFrameSeconds;
	return m_elapsedSeconds;
}

double GameTime::getGameTime()
{
	m_seconds = timeGetTime() / 1000.f - m_startSeconds;
	return m_seconds;
}

//时间循环函数
void GameTime::updateTime(float f)
{
	for (auto it = m_mapTimers.begin(); it != m_mapTimers.end(); )
	{
		Timer*t = it->second;
		if (t->isStop())
		{
			t->release();
			it = m_mapTimers.erase(it);
		}
		else if (t->isPause())
		{
			//nothing
		}
		else
		{
			t->m_accumulationTime += f;
			if (t->m_accumulationTime >= t->m_span)
			{
				t->m_accumulationTime -= t->m_span;
				t->m_func(f);
				if (t->m_loop != -1 && --t->m_loop == -1)
				{
					t->m_stop = true;
				}
			}
		}
		++it;
	}
}

Timer* GameTime::addTimer(int id, float span, int loop, TIMER_FUNC func)
{
	Timer* t = nullptr;
	if (m_mapTimers.find(id) != m_mapTimers.end())
	{
		t = m_mapTimers[id];
		t->m_id = id;
		t->m_span = span;
		t->m_func = func;
	}
	else
	{
		t = Timer::create(id, span, loop, func);
		t->retain();
		m_mapTimers[id] = t;
	}
	return t;
}

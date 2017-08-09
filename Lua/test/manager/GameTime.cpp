#include "GameTime.h"
#include "..\base\NodeList.h"
#include <directxcolors.h>
#include <tchar.h>
#include "..\..\log4cplus-1.2.1-rc1\include\log4cplus\helpers\sleep.h"
#include <time.h>
#include <windows.h>
#include "..\GameApp.h"


Timer* Timer::create(const std::string&name, float span, int loop, TIMER_FUNC func)
{
	Timer* ret = new Timer();
	 ret->autorelease(); 
	 ret->m_name = name;
	 ret->m_span = span;
	 ret->m_loop = loop;
	 ret->m_func = func;
	 ret->m_endTime = clock() / 1000.f + span;
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

bool Timer::timerCheck(double timeNow)
{
	if (m_pause)
		return false;
	if (timeNow >= m_endTime)
	{
		m_endTime = m_endTime + m_span;
		return true;
	}
	return false;
}

void Timer::restart()
{
	m_endTime = timeGetTime() / 1000.f + m_span;
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
	for (auto it = m_mapTimers.begin(); it != m_mapTimers.end(); )
	{
		Timer*t = it->second;
		if (t->isStop())
		{
			t->release();
			it = m_mapTimers.erase(it);
			continue;
		}
		else{
			if (t->timerCheck(sec))
			{
				t->m_func(t->m_span);
				if (t->m_loop != -1 && --t->m_loop == -1)
				{
					t->m_stop = true;
				}
			}
		}
		++it;
	}

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
	GameApp::getInstance()->Update(f);
}

Timer* GameTime::addTimer(const std::string&timerName, float span, int loop, TIMER_FUNC func)
{
	Timer* t = nullptr;
	if (m_mapTimers.find(timerName) != m_mapTimers.end())
	{
		t = m_mapTimers[timerName];
		t->m_name = timerName;
		t->m_span = span;
		t->m_func = func;
	}
	else
	{
		t = Timer::create(timerName, span, loop, func);
		t->retain();
		m_mapTimers[timerName] = t;
	}
	return t;
}

void GameTime::removeTimer(const std::string&timerName)
{
	auto it = m_mapTimers.find(timerName);
	if (it != m_mapTimers.end())
	{
		m_mapTimers.erase(it);
	}
}

#pragma once
#include "..\base\Singleton.h"
#include "..\base\Ref.h"
#include <map>
#include <functional>


typedef  std::function<void(float)>  TIMER_FUNC;

class Timer : public Ref
{
	friend class GameTime;
private:
	Timer() :m_id(0), m_span(1.f), m_func(nullptr), m_stop(false), m_pause(false), m_loop(-1)
	{ }
public:
	static Timer*	create(int id, float span, int loop, TIMER_FUNC func);
	void			stop();
	void			pause(bool p);
	inline bool		isStop() { return m_stop; };
	inline bool		isPause() { return m_pause; };
	bool			timerCheck(double timesec);
public:
	int				m_id;
	float			m_span;
	TIMER_FUNC		m_func;
	int				m_loop;	//剩余执行次数, -1表示无限
private:
	bool			m_stop;
	bool			m_pause;
	double			m_endTime;
};

class GameTime
{
	SingletonClase(GameTime);
public:
	GameTime();
	~GameTime();
public:	
	//设置程序的开始时间，只在程序启动时调用一次
	void	setGameStartTime(double sec);

	void	updateFrameTime(double sec);

	float	getElapsedTime();

	double	getGameTime();

	//时间循环函数
	void	updateTime(float f);

	//id为唯一标识，重复id会覆盖
	Timer*	addTimer(int id, float span, int loop, TIMER_FUNC func);
private:
	double						m_startSeconds;			//程序开始的时间
	double						m_seconds;				//程序执行的总时间
	double						m_startFrameSeconds;	//当前帧开始的时间
	float						m_elapsedSeconds;		//距离上一帧的时间

	std::map<int, Timer*>		m_mapTimers;
};

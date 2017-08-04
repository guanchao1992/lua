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
	Timer() :m_name(""), m_span(1.f), m_func(nullptr), m_stop(false), m_pause(false), m_loop(-1)
	{ }
public:
	static Timer*	create(const std::string& name, float span, int loop, TIMER_FUNC func);
	void			stop();
	void			pause(bool p);
	void			restart();
	inline bool		isStop() { return m_stop; };
	inline bool		isPause() { return m_pause; };
	bool			timerCheck(double timesec);
public:
	std::string		m_name;
	float			m_span;
	TIMER_FUNC		m_func;
	int				m_loop;	//ʣ��ִ�д���, -1��ʾ����
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
	//���ó���Ŀ�ʼʱ�䣬ֻ�ڳ�������ʱ����һ��
	void	setGameStartTime(double sec);

	void	updateFrameTime(double sec);

	float	getElapsedTime();

	double	getGameTime();

	//ʱ��ѭ������
	void	updateTime(float f);

	//idΪΨһ��ʶ���ظ�id�Ḳ��
	Timer*	addTimer(const std::string&timerName, float span, int loop, TIMER_FUNC func);

	void	removeTimer(const std::string&timerName);
private:
	double						m_startSeconds;			//����ʼ��ʱ��
	double						m_seconds;				//����ִ�е���ʱ��
	double						m_startFrameSeconds;	//��ǰ֡��ʼ��ʱ��
	float						m_elapsedSeconds;		//������һ֡��ʱ��

	std::map<std::string, Timer*>		m_mapTimers;
};

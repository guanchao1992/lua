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
	Timer() :m_id(0), m_span(1.f), m_func(nullptr), m_stop(false), m_pause(false), m_accumulationTime(0.f)
	{ }
public:
	static Timer*	create(int id,float span,TIMER_FUNC func);
	void			stop();
	void			pause(bool p);
	inline bool		isStop() { return m_stop; };
	inline bool		isPause() { return m_pause; };
public:
	int				m_id;
	float			m_span;
	TIMER_FUNC		m_func;
private:
	bool			m_stop;
	bool			m_pause;
	float			m_accumulationTime;
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
	Timer*	addTimer(int id, float span, TIMER_FUNC func);
private:
	double						m_startSeconds;			//����ʼ��ʱ��
	double						m_seconds;				//����ִ�е���ʱ��
	double						m_startFrameSeconds;	//��ǰ֡��ʼ��ʱ��
	float						m_elapsedSeconds;		//������һ֡��ʱ��

	std::map<int, Timer*>		m_mapTimers;
};
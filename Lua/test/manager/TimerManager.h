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
	int timerID = -1;				//��ʱ��id
	float intervalTime = 1.0f;		//ÿ��ִ�м��
	int cumulativeNumber = 0;		//�ۼ�ִ�д���
	int total = 1;					//ִ������
	float cumulativeTimer = 0.f;	//�ۼ�ִ��ʱ��
	bool isdelete = false;			//���ɾ��
	TIMERCALLBACK func;				//��ʱ����ʱ�ص�����
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
	float _accumulatedTime;//�ۼ�ʱ��

	std::map<int, Timer> _mapTimers;
};


#endif
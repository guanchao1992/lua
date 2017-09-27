#pragma once
#include "..\base\Singleton.h"
#include "..\base\Ref.h"
#include <list>
#include <map>
#include "EventManager.h"
#include <wtypes.h>


//要保证已经按下的按键不会再次响应按下，能够重复响应事件
class KeyManager
{
	SingletonClase(KeyManager);
public:
	enum KeyEventType : unsigned int
	{
		Down	= 0x0001,		// 按下时响应回调
		Up		= 0x0002,		// 弹起时响应回调
		Loop	= 0x0004,		// 循环响应回调
	};
	typedef std::function <void()> KEYMANAGER_FUNC_EVENT;

	struct KeyEvent
	{
		std::string				id;		//唯一标识
		UINT					type;	//keyEventType
		float					loopSpan = 1.0f;
		float					dtime = 0.f;
		KEYMANAGER_FUNC_EVENT	functor = nullptr;
	};

	struct KeyMS
	{
		bool								isDown = false;
		std::map<std::string, KeyEvent>		mapKey;
	};
public:
	KeyManager();
	~KeyManager();
public:
	bool Init();

	//注册按键回调
	//对象被析构的时候，一定要注意调用ClearKey
	bool RegKey(unsigned int key, const std::string&id, UINT keyEventType, KEYMANAGER_FUNC_EVENT functor = nullptr, float span = 1.0f);
	void ClearKey(unsigned int key, const std::string&id);
	void Update(float t);

	void keyEvent(KeyEventArgs::KeyType keyType, UINT key, UINT controlKey, UINT virtualKey);

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);

private:
	std::map<unsigned int, KeyMS>	m_mapKeyEvent;
};

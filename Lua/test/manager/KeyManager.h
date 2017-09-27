#pragma once
#include "..\base\Singleton.h"
#include "..\base\Ref.h"
#include <list>
#include <map>
#include "EventManager.h"
#include <wtypes.h>


//Ҫ��֤�Ѿ����µİ��������ٴ���Ӧ���£��ܹ��ظ���Ӧ�¼�
class KeyManager
{
	SingletonClase(KeyManager);
public:
	enum KeyEventType : unsigned int
	{
		Down	= 0x0001,		// ����ʱ��Ӧ�ص�
		Up		= 0x0002,		// ����ʱ��Ӧ�ص�
		Loop	= 0x0004,		// ѭ����Ӧ�ص�
	};
	typedef std::function <void()> KEYMANAGER_FUNC_EVENT;

	struct KeyEvent
	{
		std::string				id;		//Ψһ��ʶ
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

	//ע�ᰴ���ص�
	//����������ʱ��һ��Ҫע�����ClearKey
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

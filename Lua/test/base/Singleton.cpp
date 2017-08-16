#include "Singleton.h"
#include "..\..\MyTool\tool_log.h"

SingletonClaseCpp(Singleton);

Singleton::Singleton()
{
	LOG_D("构造了一个单例");
}

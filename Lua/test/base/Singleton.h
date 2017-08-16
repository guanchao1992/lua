#ifndef __Singleton_H__
#define __Singleton_H__


#define	SingletonClase(ClassType);	public: static ClassType* getInstance() { static ClassType instance; return &instance; }\
									protected: struct Object_Creator##ClassType { Object_Creator##ClassType() { ClassType::getInstance(); } };\
									static Object_Creator##ClassType _object_creator;
#define SingletonClaseCpp(ClassType);	ClassType::Object_Creator##ClassType ClassType::_object_creator;	

/*
//Ê¾Àý
*/
class Singleton
{
	Singleton();
	SingletonClase(Singleton);
};

#endif
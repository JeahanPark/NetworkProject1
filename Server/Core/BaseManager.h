#pragma once

template<class T>
class BaseManager
{
public:
	virtual ~BaseManager();
public:
	static T* GetInstance()
	{
		static T instance;
		return &instance;
	}
};

template<class T>
inline BaseManager<T>::~BaseManager()
{
}

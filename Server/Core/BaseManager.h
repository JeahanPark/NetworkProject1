#pragma once

template<class T>
class BaseManager
{
public:
	static T* GetInstance()
	{
		static T instance;
		return &instance;
	}
};
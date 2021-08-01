#pragma once
#include <mutex>

class Account
{

};

class AccountMananger
{
public:
	static AccountMananger* Instance()
	{
		static AccountMananger instance;
		return &instance;
	}

	Account* GetAccount(int32 id)
	{
		lock_guard<mutex> guard(_mutex);

		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;

};


#include "pch.h"
#include "UserManager.h"
#include "AccountMananger.h"

void UserManager::ProcessSave()
{
	// userLock
	lock_guard<mutex> guard(_mutex);

	// accountLock
	AccountMananger::Instance()->GetAccount(100);
}

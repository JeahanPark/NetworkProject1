#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#pragma region 쓰레드생성
//void HelloThread()
//{
//    cout << "Hello Thread" << endl;
//}
//
//int main()
//{
//    std::thread t;
//
//    auto id1 = t.get_id();// 쓰레드마다 id
//    t = std::thread(HelloThread);
//
//    int32 count = t.hardware_concurrency(); // CPU 코어 갯수
//    auto id2 = t.get_id();// 쓰레드마다 id
//
//    if (t.joinable())
//        t.join();;
//
//    cout << "Hello Main" << endl;
//    
//    
//    t.join();
//}
#pragma endregion

#pragma region Atomic
//atomic<int32> sum = 0;
//void Add()
//{
//	for (int32 i = 0; i < 100'0000; i++)
//	{
//		sum.fetch_add(1);
//		//sum++;
//	}
//}
//
//void Sub()
//{
//	for (int32 i = 0; i < 100'0000; i++)
//	{
//		sum.fetch_add(-1);
//	}
//}
//
//int main()
//{
//	std::thread t1(Add);
//	std::thread t2(Sub);
//
//	t1.join();
//	t2.join();
//
//	cout << sum << endl;
//}
#pragma endregion

#pragma region Lock기초

//vector<int> v;
//
//// mutual Exclusive (상호배타적)
//// 재귀적으로 호출이 불가능하다.
//mutex m;
//
//// 뮤텍스를 쓰게되면 수동으로 락을해주고 언락을해줘야된다.
//// 근데 코드 길어지고 복잡해지면 unlock을 못하는경우가 생긴다.
//// 그래서 수동으로 lock, unlock을 하지않고 아래의 패턴을 쓴다.
//
//// RAII ( Resource Acquisition is Initialization )
//template<typename T>
//class LockGuard
//{
//public:
//	LockGuard(T& m)
//	{
//		_mutex = &m;
//		_mutex->lock();
//	}
//
//	~LockGuard()
//	{
//		_mutex->unlock();
//	}
//
//private:
//	T* _mutex;
//};
//
//// std::lock_guard
// 
//// 시점을 미룰수있다.
//// std::unique_lock
//void Push()
//{
//	for (int32 i = 0; i < 10000; i++)
//	{
//		//std::lock_guard<std::mutex> lockGuard(m);
//		//m.lock();
//		
//		std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
//		uniqueLock.lock();
//
//		v.push_back(i);
//		//m.unlock();
//	}
//}
//
//int main()
//{
//
//	std::thread t1(Push);
//	std::thread t2(Push);
//
//	t1.join();
//	t2.join();
//
//	cout << v.size() << endl;
//}
#pragma endregion

#pragma region DeadLock
//
//#include "AccountMananger.h"
//#include "UserManager.h"
//
//void Func1()
//{
//	for (int32 i = 0; i < 1; ++i)
//	{
//		UserManager::Instance()->ProcessSave();
//	}
//}
//void Func2()
//{
//	for (int32 i = 0; i < 1; ++i)
//	{
//		AccountMananger::Instance()->ProcessLogin();
//	}
//}
//int main()
//{
//	std::thread t1(Func1);
//	std::thread t2(Func2);
//
//	t1.join();
//	t2.join();
//
//	cout << "jobs Done" << endl;
//
//	mutex m1;
//	mutex m2;
//
//	//adopt_lock = 이미 lock된 상태니까 ㄴ나주엥 소멸될 때 풀어주기만 해
//	lock_guard<mutex> g1(m1, std::adopt_lock);
//}

#pragma endregion

#pragma region spinLock
//// 스핀락은 현재 잠겨져있는 메서드를 계속 대기하고
//// 열리면 들어가는것
//// 계속 체크를 하기 때문에 CPU를 계속 낭비한다.
//// 화장실앞 존버 메타
//class SpinLock
//{
//public:
//	void lock()
//	{
//		// CAS(Compare-And-Swap)
//
//		bool expected = false;
//		bool desired = true;
//
//		// CAS 의사 코드
//		//if (_locked == expected)
//		//{
//		//	expected = _locked;
//		//	_locked = desired;
//
//		//	return true;
//		//}
//		//else
//		//{
//		//	expected = _locked;
//		//	return false;
//		//}
//		//
//		while (_locked.compare_exchange_strong(expected, desired) == false)
//		{
//			expected = false;
//		}
//	}
//
//	void unlock()
//	{
//		_locked.store(false);
//	}
//
//private:
//	atomic<bool> _locked = false;
//};
//
//int32 sum = 0;
//mutex m;
//SpinLock  spinLock;
//
//void Add()
//{
//	for (int32 i = 0; i < 10'000; i++)
//	{
//		lock_guard<SpinLock> guard(spinLock);
//		sum++;
//	}
//}
//
//void Sub()
//{
//	for (int32 i = 0; i < 10'000; i++)
//	{
//		lock_guard<SpinLock> guard(spinLock);
//		sum--;
//	}
//}
//
//int main()
//{
//	thread t1(Add);
//	thread t2(Sub);
//
//	t1.join();
//	t2.join();
//
//	cout << sum << endl;
//}

#pragma endregion

#pragma region SLEEP
// 메서드에 진입을 했는데 락이 걸려있을경우
// SpinLock처럼 무한대기가아니라 일정시간 기다렸다가 다시 메서드 진입
// 이렇게 되면 진입후 락에걸려있어 기다리고있는데 락이 해제되어 다른놈이 들어갈수있다.
// 랜덤메타

#pragma endregion

#include "pch.h"
#include <iostream>
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>

int main()
{

}
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
// 
// unique_lock은 선언을햇을때 락객체만 생성해준다.
// 후에 객체로 lock함수를 선언해야지만 락이걸린다.
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

#pragma region spinLock, Sleep
//// 스핀락은 현재 잠겨져있는 메서드를 계속 대기하고
//// 열리면 들어가는것
//// 계속 체크를 하기 때문에 CPU를 계속 낭비한다.
//// 화장실앞 존버 메타
//
//// Sleep
//// 메서드에 진입을 했는데 락이 걸려있을경우
//// SpinLock처럼 무한대기가아니라 일정시간 기다렸다가 다시 메서드 진입
//// 이렇게 되면 진입후 락에걸려있어 기다리고있는데 락이 해제되어 다른놈이 들어갈수있다.
//// 랜덤메타
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
//			//SpinLock
//			//expected = false;
//
//			//Sleep
//			this_thread::sleep_for(100ms);
//			this_thread::yield();
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

#pragma region Event
//#include <Windows.h>
//
//mutex m;
//queue<int32> q;
//HANDLE handle;
//void Producer()
//{
//	while (true)
//	{
//		{
//			unique_lock<mutex> lock(m);
//			q.push(100);
//		}
//
//		::SetEvent(handle);
//
//		this_thread::sleep_for(100ms);
//	}
//}
//
//void Consumer()
//{
//	while (true)
//	{
//		::WaitForSingleObject(handle, INFINITE);
//
//		unique_lock<mutex> lock(m);
//		if (q.empty() == false)
//		{
//			int32 data = q.front();
//			q.pop();
//			cout << data << endl;
//		}
//	}
//}
//
//int main()
//{
//	// 커널 오브젝트
//	/*
//	* Usage Count 몇명이 해당 커널 오브젝트를 사용하는지
//	* Signal, Non-Signal 켜져잇는지 꺼져있는지
//	* Auto / Manual
//	*/
//
//	// handle이란 int형이다. 10, 20 번호가 들어가있다.
//	// 번호표다. 해당 커널이벤트를 실행할때 번호를 넘겨준다?
//	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*수동으로 리셋해줄것이냐 자동으로 해줄것이냐*/, FALSE/*Signal 처음초기 상태*/, NULL);
//
//	thread t1(Producer);
//	thread t2(Consumer);
//
//	t1.join();
//	t2.join();
//
//	::CloseHandle(handle);
//}
#pragma endregion

// future
#pragma region Future, Promise
//#include <future>
//
//int64 calculate()
//{
//	cout << "1" << endl;
//	int64 sum = 0;
//	for (int32 i = 0; i < 100'000'000; i++)
//		sum += 1;
//	return sum;
//}
//
//void promisewoker(std::promise<string>&& promise)
//{
//	promise.set_value("12");
//}
//
//void taskworker(std::packaged_task<int64(void)>&& task)
//{
//	task();
//}
//
//int main()
//{
//	// 1) deferred -> lazy evaluation 지연해서 실행하세요.
//	// 2) async -> 별도의 쓰레드를 만들어서 실행하세요.
//	// 3) deferred | async -> 둘 중 알아서 골라주세요. 
//
//	//std::future<int64> future = std::async(std::launch::deferred, calculate);
//	//int64 sum = future.get();
//
//	std::promise<string> promise;
//	std::future<string> future = promise.get_future();
//
//	thread t(promisewoker, std::move(promise));
//
//	string message = future.get();
//
//
//	std::packaged_task < int64(void) > task(calculate);
//	std::future<int64> future = task.get_future();
//
//	std::thread t(taskworker, std::move(task));
//
//	/*
//	* 결론
//	* mutex, condition_variable깢 가지 않고 단순한 애들을 처리할 수 있는
//	* 1회성으로 한번만 일어나는 상황일경우 future객체를 이용하는게 더 좋을수도있다.
//	* 닭잡는데 소잡는 칼을 쓸 필요 없다!
//	* 1) async
//	* 원하는 함수를 비동기적으로 실행
//	* 
//	* 2) promise
//	* 결과물을 promise를 통해 future로 받아줌
//	* 
//	* 3)  packaged_task
//	* 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌
//	*/
//} 

#pragma endregion

#pragma region Thread local Storage
//#include <mutex>
//#include <Windows.h>
//thread_local int32 LThreadID = 0;
//
//void ThreadMain(int32 threadID)
//{
//	LThreadID = threadID;
//
//	while (true)
//	{
//		cout << "Hi" << endl;
//		this_thread::sleep_for(1s);
//	}
//}
//
//int main()
//{
//	vector<thread> threads;
//	for (int32 i = 0; i < 10; i++)
//	{
//		int32 threadid = i + 1;
//		threads.push_back(thread(ThreadMain, threadid));
//	}
//
//}

#pragma endregion

#pragma once
#include <mutex>

using shared_session = shared_ptr<class Session>;

using LockGuard = std::lock_guard<std::mutex>;
const int BUFSIZE = 1000;
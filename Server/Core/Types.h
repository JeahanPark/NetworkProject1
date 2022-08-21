#pragma once
#include <mutex>

using shared_session = shared_ptr<class Session>;

using LockGuard = std::lock_guard<std::mutex>;
using UniqueLock = std::unique_lock<std::mutex>;
const int BUFSIZE = 1000;
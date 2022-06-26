#pragma once
#include <mutex>

using LockGuard = std::lock_guard<std::mutex>;
const int BUFSIZE = 1000;
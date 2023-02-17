#pragma once
#include <mutex>

using shared_session = shared_ptr<class Session>;

using LockGuard = std::lock_guard<std::mutex>;
using UniqueLock = std::unique_lock<std::mutex>;
const int BUF_MAX_SIZE = 4096;

#define USER_ID_LENGTH 64
#define PASSWORD_LENGTH 64
#define CHATTING_LENGTH 256

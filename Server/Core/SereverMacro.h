#pragma once

// 일부로 충돌을 일으키는 디파인

#define CRASH(cause)						\
{											\
	int* crash = nullptr;					\
	cout << cause << endl;					\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define IF_CRASH(expr)				\
{									\
	if (expr)						\
	{								\
		CRASH("IF_CRASH");			\
		__analysis_assume(expr);	\
	}								\
}
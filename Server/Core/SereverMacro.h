#pragma once

// �Ϻη� �浹�� ����Ű�� ������

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
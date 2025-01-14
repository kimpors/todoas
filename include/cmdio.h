#pragma once
#include <stdio.h>

#ifdef _WIN32
	#define EXPORT __declspec(dllexport)
#elif __linux__
	#define EXPORT
#endif

EXPORT int cokbhit(void);
EXPORT int cogetch(void);
EXPORT void coclear(void);
EXPORT void comove(size_t row, size_t col);

#ifdef _WIN32
#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#else

#include <sys/types.h>
#include <unistd.h>

#endif

// min and max is required by gdi, therefore NOMINMAX won't work
#ifdef max
	#undef max
#endif

#ifdef min
	#undef min
#endif


#include <cassert>
#include <cctype>
#include <csignal>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>
#include <ranges>
#include <span>
#include <sstream>
#include <utility>
#include <vector>

#pragma once

#include <cstdlib>
#include <spdlog/spdlog.h>

// Debug-only logging
#ifdef DEBUG
	#define DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#else
	#define DEBUG_LOG(...) (void)0
#endif

// Assertions with file/line info
#ifdef DEBUG
	#define ASSERT(expr) \
		do { \
			if (!(expr)) { \
				spdlog::error("Assertion failed: {} at {}:{}", #expr, __FILE__, __LINE__); \
				std::abort(); \
			} \
		} while (0)

	#define ASSERT_MSG(expr, msg) \
		do { \
			if (!(expr)) { \
				spdlog::error("Assertion failed: {} at {}:{} | {}", #expr, __FILE__, __LINE__, msg); \
				std::abort(); \
			} \
		} while (0)
#else
	#define ASSERT(expr) (void)0
	#define ASSERT_MSG(expr, msg) (void)0
#endif

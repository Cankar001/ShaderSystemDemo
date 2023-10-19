#pragma once

#include "Logger.h"

#ifdef SHADER_SYSTEM_DEBUG
#define SHADER_SYSTEM_ENABLE_ASSERTS
#endif

#ifdef SHADER_SYSTEM_ENABLE_ASSERTS

// __VA_ARGS__ expansion to get past MSVC "bug"
#define SHADER_SYSTEM_EXPAND_VARGS(x) x

#define SHADER_SYSTEM_ASSERT_NO_MESSAGE(x) { if(!(x)) { __debugbreak(); } }
#define SHADER_SYSTEM_ASSERT_MESSAGE(x, ...) { if(!(x)) { SHADER_SYSTEM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#define SHADER_SYSTEM_ASSERT_RESOLVE(arg1, arg2, macro, ...) macro
#define SHADER_SYSTEM_GET_ASSERT_MACRO(...) SHADER_SYSTEM_EXPAND_VARGS(SHADER_SYSTEM_ASSERT_RESOLVE(__VA_ARGS__, SHADER_SYSTEM_ASSERT_MESSAGE, SHADER_SYSTEM_ASSERT_NO_MESSAGE))

#define SHADER_SYSTEM_ASSERT(...) SHADER_SYSTEM_EXPAND_VARGS( SHADER_SYSTEM_GET_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) )
#else
#define SHADER_SYSTEM_ASSERT(...)
#endif

#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Logging/Log.h"
#include <cassert>

#ifdef ENJIN_BUILD_DEBUG
    #define ENJIN_ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                ENJIN_LOG_FATAL(Core, "Assertion failed: %s", message); \
                assert(condition && message); \
            } \
        } while(0)

    #define ENJIN_ASSERT_MSG(condition, format, ...) \
        do { \
            if (!(condition)) { \
                ENJIN_LOG_FATAL(Core, "Assertion failed: " format, __VA_ARGS__); \
                assert(condition); \
            } \
        } while(0)
#else
    #define ENJIN_ASSERT(condition, message) ((void)0)
    #define ENJIN_ASSERT_MSG(condition, format, ...) ((void)0)
#endif

#define ENJIN_UNREACHABLE() \
    do { \
        ENJIN_LOG_FATAL(Core, "Unreachable code reached at %s:%d", __FILE__, __LINE__); \
        assert(false && "Unreachable code reached"); \
    } while(0)

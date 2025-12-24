#pragma once

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define ENJIN_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
#elif defined(__linux__)
    #ifndef ENJIN_PLATFORM_LINUX
        #define ENJIN_PLATFORM_LINUX
    #endif
#elif defined(__APPLE__)
    #define ENJIN_PLATFORM_MACOS
    #define ENJIN_PLATFORM_POSIX
#else
    #error "Unsupported platform"
#endif

// Compiler detection
#if defined(_MSC_VER)
    #define ENJIN_COMPILER_MSVC
    #define ENJIN_FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define ENJIN_COMPILER_GCC
    #define ENJIN_FORCE_INLINE __attribute__((always_inline)) inline
#else
    #define ENJIN_FORCE_INLINE inline
#endif

// Debug/Release detection
#if defined(NDEBUG) || defined(_RELEASE)
    #define ENJIN_BUILD_RELEASE
#else
    #define ENJIN_BUILD_DEBUG
#endif

// Export macros for DLL
#ifdef ENJIN_BUILD_SHARED
    #ifdef ENJIN_PLATFORM_WINDOWS
        #ifdef ENJIN_BUILD_CORE
            #define ENJIN_API __declspec(dllexport)
        #else
            #define ENJIN_API __declspec(dllimport)
        #endif
    #else
        #define ENJIN_API __attribute__((visibility("default")))
    #endif
#else
    #define ENJIN_API
#endif

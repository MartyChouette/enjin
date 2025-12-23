cmake_minimum_required(VERSION 3.20)

# Core library - Foundation components
file(GLOB_RECURSE CORE_SOURCES
    "src/*.cpp"
    "include/*.h"
    "include/*.hpp"
)

add_library(EnjinCore STATIC ${CORE_SOURCES})

target_include_directories(EnjinCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(EnjinCore PUBLIC cxx_std_20)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(ENJIN_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(ENJIN_PLATFORM_LINUX)
elseif(APPLE)
    target_compile_definitions(ENJIN_PLATFORM_MACOS)
endif()

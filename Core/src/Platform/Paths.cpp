#include "Enjin/Platform/Paths.h"

#include <filesystem>

#if defined(ENJIN_PLATFORM_WINDOWS)
    #include <Windows.h>
#elif defined(ENJIN_PLATFORM_LINUX)
    #include <unistd.h>
    #include <limits.h>
    #include <cerrno>
#elif defined(ENJIN_PLATFORM_MACOS)
    #include <mach-o/dyld.h>
#endif

namespace Enjin::Platform {

static std::string NormalizeDir(std::filesystem::path p) {
    p = p.lexically_normal();
    // parent_path() can be empty if path is relative or unknown
    return p.string();
}

std::string GetExecutablePath() {
#if defined(ENJIN_PLATFORM_WINDOWS)
    char buffer[MAX_PATH]{};
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        return {};
    }
    return std::string(buffer, buffer + len);
#elif defined(ENJIN_PLATFORM_LINUX)
    char buffer[PATH_MAX]{};
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len <= 0) {
        return {};
    }
    buffer[len] = '\0';
    return std::string(buffer);
#elif defined(ENJIN_PLATFORM_MACOS)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    if (size == 0) {
        return {};
    }
    std::string path(size, '\0');
    if (_NSGetExecutablePath(path.data(), &size) != 0) {
        return {};
    }
    // Trim possible trailing nulls
    while (!path.empty() && path.back() == '\0') {
        path.pop_back();
    }
    return path;
#else
    return {};
#endif
}

std::string GetExecutableDirectory() {
    const std::string exePath = GetExecutablePath();
    if (exePath.empty()) {
        return {};
    }
    std::filesystem::path p(exePath);
    auto dir = p.parent_path();
    if (dir.empty()) {
        return {};
    }
    return NormalizeDir(dir);
}

bool SetCurrentWorkingDirectory(const std::string& path) {
    if (path.empty()) {
        return false;
    }
#if defined(ENJIN_PLATFORM_WINDOWS)
    return SetCurrentDirectoryA(path.c_str()) != 0;
#else
    return ::chdir(path.c_str()) == 0;
#endif
}

void SetWorkingDirectoryToExecutableDirectory() {
    const std::string dir = GetExecutableDirectory();
    if (dir.empty()) {
        return;
    }
    (void)SetCurrentWorkingDirectory(dir);
}

} // namespace Enjin::Platform


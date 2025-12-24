#pragma once

#include "Enjin/Platform/Platform.h"
#include <string>

namespace Enjin::Platform {

// Returns absolute path to the current executable, if available.
ENJIN_API std::string GetExecutablePath();

// Returns the directory containing the current executable (no trailing slash).
ENJIN_API std::string GetExecutableDirectory();

// Sets current working directory. Returns true on success.
ENJIN_API bool SetCurrentWorkingDirectory(const std::string& path);

// Best-effort: sets working directory to the executable directory.
// This helps relative paths (logs, shaders, assets) work when launching via double-click.
ENJIN_API void SetWorkingDirectoryToExecutableDirectory();

} // namespace Enjin::Platform


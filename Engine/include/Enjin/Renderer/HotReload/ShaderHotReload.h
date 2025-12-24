#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Renderer/Vulkan/VulkanShader.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <chrono>

namespace Enjin {
namespace Renderer {

// Shader hot-reload system
// DESIGN: Automatically reload shaders when files change
class ENJIN_API ShaderHotReload {
public:
    ShaderHotReload();
    ~ShaderHotReload();

    // Register shader for hot-reload
    void RegisterShader(const std::string& filepath, VulkanShader* shader);
    void UnregisterShader(const std::string& filepath);

    // Check for file changes and reload if needed
    void Update(); // Call this every frame or periodically

    // Enable/disable hot-reload
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

    // Reload callback - called when shader is reloaded
    using ReloadCallback = std::function<void(const std::string& filepath, VulkanShader* shader)>;
    void SetReloadCallback(ReloadCallback callback) { m_ReloadCallback = callback; }

private:
    struct ShaderEntry {
        VulkanShader* shader = nullptr;
        std::chrono::system_clock::time_point lastModified;
        std::string filepath;
    };

    std::unordered_map<std::string, ShaderEntry> m_Shaders;
    bool m_Enabled = true;
    ReloadCallback m_ReloadCallback;
    
    std::chrono::system_clock::time_point GetFileModificationTime(const std::string& filepath);
    bool ReloadShader(ShaderEntry& entry);
};

} // namespace Renderer
} // namespace Enjin

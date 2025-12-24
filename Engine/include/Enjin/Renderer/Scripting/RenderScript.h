#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Renderer/RenderPipeline/RenderPipeline.h"
#include <string>
#include <functional>
#include <unordered_map>

namespace Enjin {
namespace Renderer {

// Render script - allows scripting rendering behavior
// DESIGN: Easy to inject custom rendering logic via scripts
class ENJIN_API RenderScript {
public:
    RenderScript(RenderPipeline* pipeline);
    ~RenderScript();

    // Execute script code (Lua, Python, or custom DSL)
    bool Execute(const std::string& scriptCode);
    bool ExecuteFile(const std::string& filepath);

    // Register C++ functions for script access
    void RegisterFunction(const std::string& name, std::function<void()> func);
    void RegisterFunction(const std::string& name, std::function<void(f32)> func);
    void RegisterFunction(const std::string& name, std::function<f32()> func);

    // Script API - functions available to scripts
    namespace ScriptAPI {
        // Material manipulation
        void SetMaterialParam(const std::string& materialName, const std::string& paramName, f32 value);
        void SetMaterialParam(const std::string& materialName, const std::string& paramName, const Math::Vector4& value);
        void ReloadMaterial(const std::string& materialName);
        
        // Pipeline state
        void SetLineWidth(f32 width);
        void SetDepthTest(bool enable);
        void SetCullMode(const std::string& mode); // "none", "front", "back", "both"
        
        // Hooks
        void RegisterHook(const std::string& eventType, const std::string& hookName);
        
        // Debug
        void EnableWireframe(bool enable);
        void EnableDebugVisualization(bool enable);
    }

private:
    RenderPipeline* m_Pipeline = nullptr;
    std::unordered_map<std::string, std::function<void()>> m_Functions;
    
    // Script engine would be initialized here
    // For now, we provide the interface
};

} // namespace Renderer
} // namespace Enjin

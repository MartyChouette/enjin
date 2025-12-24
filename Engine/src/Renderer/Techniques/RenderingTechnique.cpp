#include "Enjin/Renderer/Techniques/RenderingTechnique.h"
#include "Enjin/Logging/Log.h"
#include <algorithm>
#include <memory>

namespace Enjin {
namespace Renderer {

RenderingTechnique::RenderingTechnique(const std::string& name)
    : m_Name(name) {
}

RenderingTechnique::~RenderingTechnique() {
}

RenderingTechniqueManager::RenderingTechniqueManager() {
}

RenderingTechniqueManager::~RenderingTechniqueManager() {
    if (m_CurrentTechnique) {
        m_CurrentTechnique->Shutdown();
    }
    m_Techniques.clear();
}

bool RenderingTechniqueManager::RegisterTechnique(std::unique_ptr<RenderingTechnique> technique) {
    if (!technique) {
        ENJIN_LOG_ERROR(Renderer, "Cannot register null technique");
        return false;
    }

    const std::string& name = technique->GetName();
    if (m_Techniques.find(name) != m_Techniques.end()) {
        ENJIN_LOG_WARN(Renderer, "Technique '%s' already registered, replacing", name.c_str());
    }

    m_Techniques[name] = std::move(technique);
    ENJIN_LOG_INFO(Renderer, "Registered rendering technique: %s", name.c_str());
    return true;
}

void RenderingTechniqueManager::UnregisterTechnique(const std::string& name) {
    auto it = m_Techniques.find(name);
    if (it != m_Techniques.end()) {
        if (m_CurrentTechnique == it->second.get()) {
            m_CurrentTechnique->Shutdown();
            m_CurrentTechnique = nullptr;
            m_CurrentTechniqueName.clear();
        }
        m_Techniques.erase(it);
        ENJIN_LOG_INFO(Renderer, "Unregistered rendering technique: %s", name.c_str());
    }
}

bool RenderingTechniqueManager::SwitchTechnique(const std::string& name) {
    auto it = m_Techniques.find(name);
    if (it == m_Techniques.end()) {
        ENJIN_LOG_ERROR(Renderer, "Technique '%s' not found", name.c_str());
        return false;
    }

    // Shutdown current technique
    if (m_CurrentTechnique) {
        m_CurrentTechnique->Shutdown();
    }

    // Switch to new technique
    m_CurrentTechnique = it->second.get();
    m_CurrentTechniqueName = name;
    
    ENJIN_LOG_INFO(Renderer, "Switched to rendering technique: %s", name.c_str());
    return true;
}

RenderingTechnique* RenderingTechniqueManager::GetCurrentTechnique() const {
    return m_CurrentTechnique;
}

RenderingTechnique* RenderingTechniqueManager::GetTechnique(const std::string& name) const {
    auto it = m_Techniques.find(name);
    if (it != m_Techniques.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<std::string> RenderingTechniqueManager::GetTechniqueNames() const {
    std::vector<std::string> names;
    names.reserve(m_Techniques.size());
    for (const auto& [name, technique] : m_Techniques) {
        names.push_back(name);
    }
    return names;
}

bool RenderingTechniqueManager::InitializeCurrent(VulkanRenderer* renderer, RenderPipeline* pipeline) {
    if (!m_CurrentTechnique) {
        ENJIN_LOG_ERROR(Renderer, "No current technique selected");
        return false;
    }

    if (!m_CurrentTechnique->Initialize(renderer, pipeline)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to initialize technique '%s'", m_CurrentTechniqueName.c_str());
        return false;
    }

    return true;
}

void RenderingTechniqueManager::Render(f32 deltaTime) {
    if (m_CurrentTechnique && m_CurrentTechnique->IsInitialized()) {
        m_CurrentTechnique->Render(deltaTime);
    }
}

} // namespace Renderer
} // namespace Enjin

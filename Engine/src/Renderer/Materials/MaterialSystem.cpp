#include "Enjin/Renderer/Materials/MaterialSystem.h"
#include "Enjin/Renderer/Vulkan/VulkanPipeline.h"
#include "Enjin/Renderer/Vulkan/VulkanShader.h"
#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"
#include <fstream>
#include <sstream>

namespace Enjin {
namespace Renderer {

MaterialInstance::MaterialInstance() {
}

MaterialInstance::~MaterialInstance() {
    Unload();
}

bool MaterialInstance::Load(const MaterialDefinition& definition) {
    m_Definition = definition;
    
    ENJIN_LOG_INFO(Renderer, "Loading material: %s", definition.name.c_str());
    
    // Copy parameters
    m_Parameters = definition.parameters;
    
    // Note: Shader loading and pipeline creation would require VulkanContext
    // For now, this is a stub that will be completed when integrated
    
    m_Dirty = true;
    return true;
}

void MaterialInstance::Reload() {
    ENJIN_LOG_INFO(Renderer, "Reloading material: %s", m_Definition.name.c_str());
    // Reload shaders and recreate pipeline
    Load(m_Definition);
}

void MaterialInstance::Unload() {
    m_Pipeline.reset();
    m_FragmentShader.reset();
    m_VertexShader.reset();
    m_Parameters.clear();
}

void MaterialInstance::SetParameter(const std::string& name, f32 value) {
    auto it = m_Parameters.find(name);
    if (it != m_Parameters.end()) {
        it->second.floatValue = value;
        m_Dirty = true;
    } else {
        MaterialParam param;
        param.type = MaterialParamType::Float;
        param.floatValue = value;
        m_Parameters[name] = param;
        m_Dirty = true;
    }
}

void MaterialInstance::SetParameter(const std::string& name, const Math::Vector4& value) {
    auto it = m_Parameters.find(name);
    if (it != m_Parameters.end()) {
        it->second.vec4Value = value;
        m_Dirty = true;
    } else {
        MaterialParam param;
        param.type = MaterialParamType::Vector4;
        param.vec4Value = value;
        m_Parameters[name] = param;
        m_Dirty = true;
    }
}

void MaterialInstance::SetTexture(const std::string& name, const std::string& path) {
    m_Definition.textures[name] = path;
    m_Dirty = true;
}

MaterialParam* MaterialInstance::GetParameter(const std::string& name) {
    auto it = m_Parameters.find(name);
    if (it != m_Parameters.end()) {
        return &it->second;
    }
    return nullptr;
}

const MaterialParam* MaterialInstance::GetParameter(const std::string& name) const {
    auto it = m_Parameters.find(name);
    if (it != m_Parameters.end()) {
        return &it->second;
    }
    return nullptr;
}

void MaterialInstance::UpdateUniforms(VkCommandBuffer cmd, u32 frameIndex) {
    (void)cmd;
    (void)frameIndex;
    // Update uniform buffers with material parameters
    // This would be implemented when integrated with renderer
    if (m_Dirty) {
        m_Dirty = false;
    }
}

MaterialSystem::MaterialSystem() {
}

MaterialSystem::~MaterialSystem() {
    Shutdown();
}

u32 MaterialSystem::LoadMaterial(const std::string& filepath) {
    ENJIN_LOG_INFO(Renderer, "Loading material from file: %s", filepath.c_str());
    
    // TODO: Parse JSON file
    // For now, return invalid ID
    ENJIN_LOG_WARN(Renderer, "JSON material loading not yet implemented");
    return UINT32_MAX;
}

u32 MaterialSystem::LoadMaterial(const MaterialDefinition& definition) {
    u32 id = static_cast<u32>(m_Materials.size());
    
    auto material = std::make_unique<MaterialInstance>();
    if (!material->Load(definition)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to load material: %s", definition.name.c_str());
        return UINT32_MAX;
    }
    
    m_Materials.push_back(std::move(material));
    m_MaterialNameMap[definition.name] = id;
    
    ENJIN_LOG_INFO(Renderer, "Loaded material '%s' with ID %u", definition.name.c_str(), id);
    return id;
}

MaterialInstance* MaterialSystem::GetMaterial(u32 id) {
    if (id >= m_Materials.size()) {
        return nullptr;
    }
    return m_Materials[id].get();
}

MaterialInstance* MaterialSystem::GetMaterial(const std::string& name) {
    auto it = m_MaterialNameMap.find(name);
    if (it != m_MaterialNameMap.end()) {
        return GetMaterial(it->second);
    }
    return nullptr;
}

void MaterialSystem::ReloadMaterial(u32 id) {
    MaterialInstance* material = GetMaterial(id);
    if (material) {
        material->Reload();
    }
}

void MaterialSystem::ReloadMaterial(const std::string& name) {
    MaterialInstance* material = GetMaterial(name);
    if (material) {
        material->Reload();
    }
}

void MaterialSystem::ReloadAllMaterials() {
    ENJIN_LOG_INFO(Renderer, "Reloading all materials...");
    for (auto& material : m_Materials) {
        if (material) {
            material->Reload();
        }
    }
}

void MaterialSystem::WatchMaterialFiles(bool enable) {
    m_FileWatching = enable;
    ENJIN_LOG_INFO(Renderer, "Material file watching: %s", enable ? "enabled" : "disabled");
    // File watcher implementation would go here
}

void MaterialSystem::ForEachMaterial(std::function<void(MaterialInstance*)> callback) {
    for (auto& material : m_Materials) {
        if (material) {
            callback(material.get());
        }
    }
}

void MaterialSystem::Shutdown() {
    m_Materials.clear();
    m_MaterialNameMap.clear();
}

} // namespace Renderer
} // namespace Enjin

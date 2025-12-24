#include "Enjin/Renderer/Vulkan/VulkanShader.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"
#include <fstream>
#include <cstring>

namespace Enjin {
namespace Renderer {

VulkanShader::VulkanShader(VulkanContext* context)
    : m_Context(context) {
}

VulkanShader::~VulkanShader() {
    Destroy();
}

bool VulkanShader::LoadFromSPIRV(const u8* data, usize size) {
    if (size % 4 != 0) {
        ENJIN_LOG_ERROR(Renderer, "SPIR-V data size must be multiple of 4");
        return false;
    }

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = size;
    createInfo.pCode = reinterpret_cast<const u32*>(data);

    VkResult result = vkCreateShaderModule(m_Context->GetDevice(), &createInfo, nullptr, &m_Module);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create shader module: %d", result);
        return false;
    }

    return true;
}

bool VulkanShader::CompileFromGLSL(const std::string& source, VkShaderStageFlagBits stage) {
    m_Stage = stage;

    std::vector<u32> spirv;
    if (!ShaderCompiler::CompileGLSL(source, stage, spirv)) {
        return false;
    }

    return LoadFromSPIRV(reinterpret_cast<const u8*>(spirv.data()), spirv.size() * sizeof(u32));
}

bool VulkanShader::LoadFromFile(const std::string& filepath) {
    std::vector<u32> spirv;
    if (!ShaderCompiler::LoadSPIRV(filepath, spirv)) {
        return false;
    }

    return LoadFromSPIRV(reinterpret_cast<const u8*>(spirv.data()), spirv.size() * sizeof(u32));
}

void VulkanShader::Destroy() {
    if (m_Module != VK_NULL_HANDLE) {
        vkDestroyShaderModule(m_Context->GetDevice(), m_Module, nullptr);
        m_Module = VK_NULL_HANDLE;
    }
}

namespace ShaderCompiler {

bool CompileGLSL(const std::string& source, VkShaderStageFlagBits stage, std::vector<u32>& spirv) {
    // For now, we'll use a simple approach:
    // Try to load pre-compiled SPIR-V or use inline shaders
    // Full shaderc integration would go here
    
    // This is a placeholder - in production you'd use shaderc library
    ENJIN_LOG_WARN(Renderer, "GLSL compilation not yet implemented - use pre-compiled SPIR-V");
    return false;
}

bool LoadSPIRV(const std::string& filepath, std::vector<u32>& spirv) {
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to open SPIR-V file: %s", filepath.c_str());
        return false;
    }

    usize fileSize = static_cast<usize>(file.tellg());
    if (fileSize % 4 != 0) {
        ENJIN_LOG_ERROR(Renderer, "SPIR-V file size must be multiple of 4");
        return false;
    }

    spirv.resize(fileSize / sizeof(u32));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(spirv.data()), fileSize);
    file.close();

    return true;
}

bool SaveSPIRV(const std::string& filepath, const std::vector<u32>& spirv) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create SPIR-V file: %s", filepath.c_str());
        return false;
    }

    file.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(u32));
    file.close();

    return true;
}

} // namespace ShaderCompiler

} // namespace Renderer
} // namespace Enjin

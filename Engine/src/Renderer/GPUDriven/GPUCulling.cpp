#include "Enjin/Renderer/GPUDriven/GPUCulling.h"
#include "Enjin/Renderer/Vulkan/VulkanBuffer.h"
#include "Enjin/Renderer/Vulkan/VulkanShader.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"
#include "Enjin/Math/Math.h"
#include "Enjin/Math/Vector.h"
#include <algorithm>
#include <cstring>
#include <cfloat>

namespace Enjin {
namespace Renderer {

GPUCullingSystem::GPUCullingSystem(VulkanContext* context)
    : m_Context(context) {
}

GPUCullingSystem::~GPUCullingSystem() {
    Shutdown();
}

bool GPUCullingSystem::Initialize() {
    ENJIN_LOG_INFO(Renderer, "Initializing GPU Culling System...");

    if (!CreateBuffers()) {
        return false;
    }

    if (!CreateComputePipeline()) {
        return false;
    }

    ENJIN_LOG_INFO(Renderer, "GPU Culling System initialized (max objects: %u)", m_MaxObjects);
    return true;
}

void GPUCullingSystem::Shutdown() {
    if (m_CullPipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(m_Context->GetDevice(), m_CullPipeline, nullptr);
        m_CullPipeline = VK_NULL_HANDLE;
    }

    if (m_PipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(m_Context->GetDevice(), m_PipelineLayout, nullptr);
        m_PipelineLayout = VK_NULL_HANDLE;
    }

    if (m_DescriptorSetLayout != VK_NULL_HANDLE) {
        vkDestroyDescriptorSetLayout(m_Context->GetDevice(), m_DescriptorSetLayout, nullptr);
        m_DescriptorSetLayout = VK_NULL_HANDLE;
    }

    m_ObjectBuffer.reset();
    m_IndirectDrawBuffer.reset();
    m_FrustumBuffer.reset();
    m_VisibilityBuffer.reset();
}

void GPUCullingSystem::SubmitObjects(const std::vector<CullableObject>& objects) {
    if (objects.size() > m_MaxObjects) {
        ENJIN_LOG_WARN(Renderer, "Too many objects (%zu), truncating to %u", objects.size(), m_MaxObjects);
    }

    usize count = std::min(objects.size(), static_cast<usize>(m_MaxObjects));
    usize bufferSize = count * sizeof(CullableObject);

    if (!m_ObjectBuffer->UploadData(objects.data(), bufferSize)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to upload object data for culling");
        return;
    }

    m_Stats.totalObjects = static_cast<u32>(count);
}

bool GPUCullingSystem::ExecuteCulling(
    const Math::Matrix4& viewMatrix,
    const Math::Matrix4& projectionMatrix,
    VkBuffer& outIndirectDrawBuffer,
    u32& outDrawCount
) {
    // Update frustum planes
    Math::Matrix4 viewProj = projectionMatrix * viewMatrix;
    UpdateFrustumPlanes(viewProj);

    // Upload frustum planes
    struct FrustumPlanes {
        Math::Vector4 planes[6]; // left, right, bottom, top, near, far
    } frustum;
    
    // Extract frustum planes from view-projection matrix
    // Left plane
    frustum.planes[0] = Math::Vector4(
        viewProj.m[3] + viewProj.m[0],
        viewProj.m[7] + viewProj.m[4],
        viewProj.m[11] + viewProj.m[8],
        viewProj.m[15] + viewProj.m[12]
    );
    
    // Right plane
    frustum.planes[1] = Math::Vector4(
        viewProj.m[3] - viewProj.m[0],
        viewProj.m[7] - viewProj.m[4],
        viewProj.m[11] - viewProj.m[8],
        viewProj.m[15] - viewProj.m[12]
    );
    
    // Bottom plane
    frustum.planes[2] = Math::Vector4(
        viewProj.m[3] + viewProj.m[1],
        viewProj.m[7] + viewProj.m[5],
        viewProj.m[11] + viewProj.m[9],
        viewProj.m[15] + viewProj.m[13]
    );
    
    // Top plane
    frustum.planes[3] = Math::Vector4(
        viewProj.m[3] - viewProj.m[1],
        viewProj.m[7] - viewProj.m[5],
        viewProj.m[11] - viewProj.m[9],
        viewProj.m[15] - viewProj.m[13]
    );
    
    // Near plane
    frustum.planes[4] = Math::Vector4(
        viewProj.m[3] + viewProj.m[2],
        viewProj.m[7] + viewProj.m[6],
        viewProj.m[11] + viewProj.m[10],
        viewProj.m[15] + viewProj.m[14]
    );
    
    // Far plane
    frustum.planes[5] = Math::Vector4(
        viewProj.m[3] - viewProj.m[2],
        viewProj.m[7] - viewProj.m[6],
        viewProj.m[11] - viewProj.m[10],
        viewProj.m[15] - viewProj.m[14]
    );

    // Normalize planes
    for (u32 i = 0; i < 6; ++i) {
        f32 length = Math::Sqrt(
            frustum.planes[i].x * frustum.planes[i].x +
            frustum.planes[i].y * frustum.planes[i].y +
            frustum.planes[i].z * frustum.planes[i].z
        );
        if (length > Math::EPSILON) {
            frustum.planes[i] = frustum.planes[i] / length;
        }
    }

    m_FrustumBuffer->UploadData(&frustum, sizeof(frustum));

    // TODO: Execute compute shader
    // For now, we'll implement a CPU fallback
    // Full GPU implementation requires compute shader compilation
    
    // Reset indirect draw buffer
    VkDrawIndexedIndirectCommand resetCmd{};
    resetCmd.indexCount = 0;
    resetCmd.instanceCount = 0;
    resetCmd.firstIndex = 0;
    resetCmd.vertexOffset = 0;
    resetCmd.firstInstance = 0;
    
    // CPU fallback culling (temporary until compute shader is ready)
    std::vector<VkDrawIndexedIndirectCommand> drawCommands;
    drawCommands.reserve(m_Stats.totalObjects);
    
    // Read back object data (simplified - in production would use GPU)
    // For now, mark all as visible
    u32 visibleCount = 0;
    for (u32 i = 0; i < m_Stats.totalObjects; ++i) {
        // In real implementation, compute shader would do this
        // For now, accept all objects
        VkDrawIndexedIndirectCommand cmd{};
        cmd.indexCount = 3; // Triangle
        cmd.instanceCount = 1;
        cmd.firstIndex = 0;
        cmd.vertexOffset = 0;
        cmd.firstInstance = i;
        drawCommands.push_back(cmd);
        visibleCount++;
    }

    m_Stats.visibleObjects = visibleCount;
    m_Stats.culledObjects = m_Stats.totalObjects - visibleCount;

    // Upload indirect draw commands
    usize cmdSize = drawCommands.size() * sizeof(VkDrawIndexedIndirectCommand);
    m_IndirectDrawBuffer->UploadData(drawCommands.data(), cmdSize);

    outIndirectDrawBuffer = m_IndirectDrawBuffer->GetBuffer();
    outDrawCount = visibleCount;

    return true;
}

bool GPUCullingSystem::CreateBuffers() {
    // Object buffer (input)
    usize objectBufferSize = m_MaxObjects * sizeof(CullableObject);
    m_ObjectBuffer = std::make_unique<VulkanBuffer>(m_Context);
    if (!m_ObjectBuffer->Create(objectBufferSize, BufferUsage::Storage, true)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create object buffer");
        return false;
    }

    // Indirect draw buffer (output)
    usize indirectBufferSize = m_MaxObjects * sizeof(VkDrawIndexedIndirectCommand);
    m_IndirectDrawBuffer = std::make_unique<VulkanBuffer>(m_Context);
    // Need both storage (for compute shader write) and indirect draw (for vkCmdDrawIndexedIndirect)
    VkBufferUsageFlags indirectUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    if (!m_IndirectDrawBuffer->Create(indirectBufferSize, indirectUsage, true)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create indirect draw buffer");
        return false;
    }

    // Frustum buffer
    struct FrustumPlanes {
        Math::Vector4 planes[6];
    };
    m_FrustumBuffer = std::make_unique<VulkanBuffer>(m_Context);
    if (!m_FrustumBuffer->Create(sizeof(FrustumPlanes), BufferUsage::Uniform, true)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create frustum buffer");
        return false;
    }

    // Visibility buffer (per-object visibility flags)
    usize visibilityBufferSize = m_MaxObjects * sizeof(u32);
    m_VisibilityBuffer = std::make_unique<VulkanBuffer>(m_Context);
    if (!m_VisibilityBuffer->Create(visibilityBufferSize, BufferUsage::Storage, true)) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create visibility buffer");
        return false;
    }

    return true;
}

bool GPUCullingSystem::CreateComputePipeline() {
    // Create descriptor set layout
    std::vector<VkDescriptorSetLayoutBinding> bindings(4);

    // Object buffer (binding 0)
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    // Indirect draw buffer (binding 1)
    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    // Frustum buffer (binding 2)
    bindings[2].binding = 2;
    bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[2].descriptorCount = 1;
    bindings[2].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    // Visibility buffer (binding 3)
    bindings[3].binding = 3;
    bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[3].descriptorCount = 1;
    bindings[3].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<u32>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    VkResult result = vkCreateDescriptorSetLayout(
        m_Context->GetDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create descriptor set layout: %d", result);
        return false;
    }

    // Create pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    result = vkCreatePipelineLayout(
        m_Context->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create pipeline layout: %d", result);
        return false;
    }

    // TODO: Create compute shader and pipeline
    // For now, we'll use CPU fallback
    // Full implementation requires compute shader compilation
    
    ENJIN_LOG_INFO(Renderer, "GPU Culling pipeline layout created (compute shader pending)");
    return true;
}

void GPUCullingSystem::UpdateFrustumPlanes(const Math::Matrix4& viewProj) {
    // Frustum planes are updated in ExecuteCulling
    (void)viewProj;
}

} // namespace Renderer
} // namespace Enjin

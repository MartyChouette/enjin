#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include "Enjin/Math/Matrix.h"
#include "Enjin/Math/Vector.h"
#include "Enjin/Memory/Memory.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <cfloat>

namespace Enjin {
namespace Renderer {

// Bounding box for GPU culling
struct ENJIN_API BoundingBox {
    Math::Vector3 min;
    Math::Vector3 max;
    
    BoundingBox() : min(Math::Vector3(1e30f)), max(Math::Vector3(-1e30f)) {}
    BoundingBox(const Math::Vector3& min, const Math::Vector3& max) : min(min), max(max) {}
    
    Math::Vector3 GetCenter() const {
        return (min + max) * 0.5f;
    }
    
    Math::Vector3 GetExtent() const {
        return max - min;
    }
};

// Object data for GPU culling
struct CullableObject {
    BoundingBox bounds;
    Math::Matrix4 transform;
    u32 meshIndex = 0;
    u32 materialIndex = 0;
    u32 indexCount = 0;
    u32 indexOffset = 0;
    u32 vertexOffset = 0;
};

// GPU frustum culling system
// INNOVATION: Move culling to GPU, reducing CPU overhead
class ENJIN_API GPUCullingSystem {
public:
    GPUCullingSystem(VulkanContext* context);
    ~GPUCullingSystem();

    bool Initialize();
    void Shutdown();

    // Submit objects for culling
    void SubmitObjects(const std::vector<CullableObject>& objects);
    
    // Execute culling on GPU
    // Returns indirect draw commands for visible objects
    bool ExecuteCulling(
        const Math::Matrix4& viewMatrix,
        const Math::Matrix4& projectionMatrix,
        VkBuffer& outIndirectDrawBuffer,
        u32& outDrawCount
    );

    // Get culling statistics
    struct CullingStats {
        u32 totalObjects = 0;
        u32 visibleObjects = 0;
        u32 culledObjects = 0;
    };
    CullingStats GetStats() const { return m_Stats; }

private:
    bool CreateComputePipeline();
    bool CreateBuffers();
    void UpdateFrustumPlanes(const Math::Matrix4& viewProj);

    VulkanContext* m_Context = nullptr;
    
    // Compute pipeline for culling
    VkPipeline m_CullPipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
    
    // Forward declaration
    class VulkanBuffer;
    
    // Buffers
    std::unique_ptr<VulkanBuffer> m_ObjectBuffer;      // Input: Objects to cull
    std::unique_ptr<VulkanBuffer> m_IndirectDrawBuffer; // Output: Indirect draw commands
    std::unique_ptr<VulkanBuffer> m_FrustumBuffer;     // Frustum planes
    std::unique_ptr<VulkanBuffer> m_VisibilityBuffer;   // Per-object visibility
    
    CullingStats m_Stats;
    u32 m_MaxObjects = 100000; // Support up to 100k objects
};

} // namespace Renderer
} // namespace Enjin

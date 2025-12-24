#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

namespace Enjin {
namespace Renderer {

// Bindless resource handle
using BindlessHandle = u32;
constexpr BindlessHandle INVALID_BINDLESS_HANDLE = UINT32_MAX;

// Bindless resource manager
// INNOVATION: Single descriptor set with all resources, access by index
// Eliminates descriptor set binding overhead
class ENJIN_API BindlessResourceManager {
public:
    BindlessResourceManager(VulkanContext* context);
    ~BindlessResourceManager();

    bool Initialize();
    void Shutdown();

    // Register texture (returns handle for shader access)
    BindlessHandle RegisterTexture(VkImageView imageView, VkSampler sampler);
    void UnregisterTexture(BindlessHandle handle);

    // Register buffer (returns handle for shader access)
    BindlessHandle RegisterBuffer(VkBuffer buffer, VkDescriptorType type);
    void UnregisterBuffer(BindlessHandle handle);

    // Update descriptor set (call before rendering or when resources change)
    void UpdateDescriptorSet();
    
    // Rebuild descriptor set (internal, called by UpdateDescriptorSet)
    void RebuildDescriptorSet();

    // Get descriptor set for binding
    VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }
    VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }

    // Get bindless texture handle in shader: textures[handle]
    // Get bindless buffer handle in shader: buffers[handle]

    // Statistics
    u32 GetTextureCount() const { return static_cast<u32>(m_Textures.size()); }
    u32 GetBufferCount() const { return static_cast<u32>(m_Buffers.size()); }

private:
    bool CreateDescriptorSetLayout();
    bool AllocateDescriptorSet();

    VulkanContext* m_Context = nullptr;

    // Descriptor set
    VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
    VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;

    // Resource tracking
    struct TextureEntry {
        VkImageView imageView = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        bool valid = false;
    };
    std::vector<TextureEntry> m_Textures;
    std::vector<BindlessHandle> m_FreeTextureSlots;

    struct BufferEntry {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDescriptorType type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bool valid = false;
    };
    std::vector<BufferEntry> m_Buffers;
    std::vector<BindlessHandle> m_FreeBufferSlots;

    static constexpr u32 MAX_TEXTURES = 1000000; // 1 million textures
    static constexpr u32 MAX_BUFFERS = 100000;    // 100k buffers
    bool m_Dirty = true;
};

} // namespace Renderer
} // namespace Enjin

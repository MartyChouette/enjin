#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Math/Vector.h"
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace Enjin {
namespace Renderer {

// Forward declarations
class RenderGraph;
class RenderPassNode;
class ResourceNode;

// Resource types
enum class ResourceType {
    Texture,
    Buffer,
    Attachment
};

// Resource description
struct ResourceDesc {
    ResourceType type = ResourceType::Texture;
    Math::Vector2 size = Math::Vector2(1920, 1080);
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    u32 mipLevels = 1;
    u32 arrayLayers = 1;
    VkImageUsageFlags usage = 0;
    bool persistent = false; // If false, resource is created/destroyed per frame
};

// Render pass description
struct RenderPassDesc {
    std::string name;
    std::vector<std::string> inputResources;   // Resources this pass reads
    std::vector<std::string> outputResources;   // Resources this pass writes
    std::function<void(VkCommandBuffer, const RenderPassDesc&)> execute;
};

// Render graph - automatic pass ordering and resource management
// INNOVATION: Declarative rendering - describe what you want, graph figures out how
class ENJIN_API RenderGraph {
public:
    RenderGraph(VulkanContext* context);
    ~RenderGraph();

    bool Initialize();
    void Shutdown();

    // Register a resource
    void RegisterResource(const std::string& name, const ResourceDesc& desc);

    // Add a render pass
    void AddRenderPass(const RenderPassDesc& desc);

    // Compile graph (resolve dependencies, create resources)
    bool Compile();

    // Execute graph (run all passes in correct order)
    bool Execute(VkCommandBuffer commandBuffer);

    // Get resource by name
    VkImage GetImage(const std::string& name) const;
    VkImageView GetImageView(const std::string& name) const;
    VkBuffer GetBuffer(const std::string& name) const;

    // Debug visualization
    void DumpGraph(const std::string& filepath) const;

private:
    struct ResourceNode {
        std::string name;
        ResourceDesc desc;
        VkImage image = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        std::vector<RenderPassNode*> readers;
        std::vector<RenderPassNode*> writers;
        u32 refCount = 0;
    };

    struct RenderPassNode {
        RenderPassDesc desc;
        std::vector<ResourceNode*> inputs;
        std::vector<ResourceNode*> outputs;
        u32 executionOrder = 0;
        bool executed = false;
    };

    void ResolveDependencies();
    void TopologicalSort();
    void CreateResources();
    void DestroyResources();
    void InsertBarriers(VkCommandBuffer cmd, RenderPassNode* pass);

    VulkanContext* m_Context = nullptr;
    std::unordered_map<std::string, std::unique_ptr<ResourceNode>> m_Resources;
    std::vector<std::unique_ptr<RenderPassNode>> m_Passes;
    std::vector<RenderPassNode*> m_ExecutionOrder;
    bool m_Compiled = false;
};

} // namespace Renderer
} // namespace Enjin

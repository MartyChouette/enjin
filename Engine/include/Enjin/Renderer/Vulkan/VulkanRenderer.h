#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Platform/Window.h"
#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include "Enjin/Renderer/Vulkan/VulkanSwapchain.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace Enjin {
namespace Renderer {

// Vulkan renderer - main rendering interface
class ENJIN_API VulkanRenderer {
public:
    VulkanRenderer();
    ~VulkanRenderer();

    bool Initialize(Window* window);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    VkCommandBuffer GetCurrentCommandBuffer() const;
    VkRenderPass GetRenderPass() const { return m_RenderPass; }
    VulkanContext* GetContext() const { return m_Context.get(); }
    VulkanSwapchain* GetSwapchain() const { return m_Swapchain.get(); }
    VkExtent2D GetSwapchainExtent() const { return m_Swapchain ? m_Swapchain->GetExtent() : VkExtent2D{0, 0}; }

    void OnWindowResize(u32 width, u32 height);

private:
    bool CreateSurface();
    bool CreateRenderPass();
    bool CreateCommandPool();
    bool CreateCommandBuffers();
    bool CreateSyncObjects();
    void DestroySyncObjects();
    bool AcquireNextImage();
    void SubmitCommandBuffer();

    Window* m_Window = nullptr;
    std::unique_ptr<VulkanContext> m_Context;
    std::unique_ptr<VulkanSwapchain> m_Swapchain;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;

    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    VkCommandPool m_CommandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_CommandBuffers;

    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    std::vector<VkSemaphore> m_RenderFinishedSemaphores;
    std::vector<VkFence> m_InFlightFences;
    std::vector<VkFence> m_ImagesInFlight;

    u32 m_CurrentFrame = 0;
    u32 m_CurrentImageIndex = 0;
    static constexpr u32 MAX_FRAMES_IN_FLIGHT = 2;
};

} // namespace Renderer
} // namespace Enjin

#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Platform/Types.h"
#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace Enjin {
namespace Renderer {

// Swapchain support details
struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

// Vulkan swapchain - manages swapchain images and presentation
class ENJIN_API VulkanSwapchain {
public:
    VulkanSwapchain(VulkanContext* context);
    ~VulkanSwapchain();

    bool Initialize(VkSurfaceKHR surface, u32 width, u32 height);
    void Shutdown();

    void Recreate(u32 width, u32 height);

    VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }
    VkFormat GetImageFormat() const { return m_ImageFormat; }
    VkExtent2D GetExtent() const { return m_Extent; }
    u32 GetImageCount() const { return static_cast<u32>(m_Images.size()); }
    const std::vector<VkImage>& GetImages() const { return m_Images; }
    const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }
    const std::vector<VkFramebuffer>& GetFramebuffers() const { return m_Framebuffers; }

    VkFramebuffer GetFramebuffer(u32 index) const {
        return index < m_Framebuffers.size() ? m_Framebuffers[index] : VK_NULL_HANDLE;
    }

    void SetRenderPass(VkRenderPass renderPass) { m_RenderPass = renderPass; }
    void RecreateFramebuffers();

private:
    SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, u32 width, u32 height);
    bool CreateSwapchain(VkSurfaceKHR surface, u32 width, u32 height);
    bool CreateImageViews();
    void DestroyImageViews();
    void DestroyFramebuffers();

    VulkanContext* m_Context = nullptr;
    VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkFormat m_ImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D m_Extent{};
    std::vector<VkImage> m_Images;
    std::vector<VkImageView> m_ImageViews;
    std::vector<VkFramebuffer> m_Framebuffers;
    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
};

} // namespace Renderer
} // namespace Enjin

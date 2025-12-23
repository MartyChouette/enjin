#include "Enjin/Renderer/Vulkan/VulkanSwapchain.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"
#include <algorithm>

namespace Enjin {
namespace Renderer {

VulkanSwapchain::VulkanSwapchain(VulkanContext* context)
    : m_Context(context) {
}

VulkanSwapchain::~VulkanSwapchain() {
    Shutdown();
}

bool VulkanSwapchain::Initialize(VkSurfaceKHR surface, u32 width, u32 height) {
    m_Surface = surface;
    return CreateSwapchain(surface, width, height) && CreateImageViews();
}

void VulkanSwapchain::Shutdown() {
    DestroyFramebuffers();
    DestroyImageViews();

    if (m_Swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(m_Context->GetDevice(), m_Swapchain, nullptr);
        m_Swapchain = VK_NULL_HANDLE;
    }
}

void VulkanSwapchain::Recreate(u32 width, u32 height) {
    vkDeviceWaitIdle(m_Context->GetDevice());

    DestroyFramebuffers();
    DestroyImageViews();

    if (!CreateSwapchain(m_Surface, width, height) || !CreateImageViews()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to recreate swapchain");
        return;
    }

    if (m_RenderPass != VK_NULL_HANDLE) {
        RecreateFramebuffers();
    }
}

SwapchainSupportDetails VulkanSwapchain::QuerySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapchainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    u32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    // Prefer SRGB with 32-bit color
    for (const auto& format : availableFormats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    // Fallback to first available
    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    // Prefer mailbox (triple buffering) for low latency
    for (const auto& mode : availablePresentModes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    // Fallback to FIFO (guaranteed to be available)
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, u32 width, u32 height) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    VkExtent2D extent;
    extent.width = std::clamp(width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width);
    extent.height = std::clamp(height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height);

    return extent;
}

bool VulkanSwapchain::CreateSwapchain(VkSurfaceKHR surface, u32 width, u32 height) {
    SwapchainSupportDetails support = QuerySwapchainSupport(
        m_Context->GetPhysicalDevice(), surface);

    if (support.formats.empty() || support.presentModes.empty()) {
        ENJIN_LOG_ERROR(Renderer, "Swapchain not supported");
        return false;
    }

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(support.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(support.presentModes);
    VkExtent2D extent = ChooseSwapExtent(support.capabilities, width, height);

    u32 imageCount = support.capabilities.minImageCount + 1;
    if (support.capabilities.maxImageCount > 0 &&
        imageCount > support.capabilities.maxImageCount) {
        imageCount = support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    u32 queueFamilyIndices[] = {
        m_Context->GetGraphicsQueueFamily(),
        m_Context->GetPresentQueueFamily()
    };

    if (m_Context->GetGraphicsQueueFamily() != m_Context->GetPresentQueueFamily()) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = support.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(m_Context->GetDevice(), &createInfo, nullptr, &m_Swapchain);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create swapchain: %d", result);
        return false;
    }

    vkGetSwapchainImagesKHR(m_Context->GetDevice(), m_Swapchain, &imageCount, nullptr);
    m_Images.resize(imageCount);
    vkGetSwapchainImagesKHR(m_Context->GetDevice(), m_Swapchain, &imageCount, m_Images.data());

    m_ImageFormat = surfaceFormat.format;
    m_Extent = extent;

    ENJIN_LOG_INFO(Renderer, "Swapchain created: %dx%d, format: %d, images: %u",
        extent.width, extent.height, m_ImageFormat, imageCount);

    return true;
}

bool VulkanSwapchain::CreateImageViews() {
    m_ImageViews.resize(m_Images.size());

    for (usize i = 0; i < m_Images.size(); ++i) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_Images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_ImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkResult result = vkCreateImageView(m_Context->GetDevice(), &createInfo, nullptr, &m_ImageViews[i]);
        if (result != VK_SUCCESS) {
            ENJIN_LOG_ERROR(Renderer, "Failed to create image view %zu: %d", i, result);
            return false;
        }
    }

    return true;
}

void VulkanSwapchain::DestroyImageViews() {
    for (auto& imageView : m_ImageViews) {
        if (imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(m_Context->GetDevice(), imageView, nullptr);
        }
    }
    m_ImageViews.clear();
}

void VulkanSwapchain::RecreateFramebuffers() {
    if (m_RenderPass == VK_NULL_HANDLE) {
        return;
    }

    DestroyFramebuffers();
    m_Framebuffers.resize(m_ImageViews.size());

    for (usize i = 0; i < m_ImageViews.size(); ++i) {
        VkFramebufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = m_RenderPass;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &m_ImageViews[i];
        createInfo.width = m_Extent.width;
        createInfo.height = m_Extent.height;
        createInfo.layers = 1;

        VkResult result = vkCreateFramebuffer(m_Context->GetDevice(), &createInfo, nullptr, &m_Framebuffers[i]);
        if (result != VK_SUCCESS) {
            ENJIN_LOG_ERROR(Renderer, "Failed to create framebuffer %zu: %d", i, result);
        }
    }
}

void VulkanSwapchain::DestroyFramebuffers() {
    for (auto& framebuffer : m_Framebuffers) {
        if (framebuffer != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(m_Context->GetDevice(), framebuffer, nullptr);
        }
    }
    m_Framebuffers.clear();
}

} // namespace Renderer
} // namespace Enjin

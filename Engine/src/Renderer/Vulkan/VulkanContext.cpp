#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"
#include <set>

namespace Enjin {
namespace Renderer {

#ifdef ENJIN_BUILD_DEBUG
const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};
#endif

VulkanContext::VulkanContext() {
}

VulkanContext::~VulkanContext() {
    Shutdown();
}

bool VulkanContext::Initialize() {
    ENJIN_LOG_INFO(Renderer, "Initializing Vulkan context...");

    if (!CreateInstance()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create Vulkan instance");
        return false;
    }

#ifdef ENJIN_BUILD_DEBUG
    if (!CreateDebugMessenger()) {
        ENJIN_LOG_WARN(Renderer, "Failed to create debug messenger");
    }
#endif

    if (!SelectPhysicalDevice()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to select physical device");
        return false;
    }

    if (!CreateLogicalDevice()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create logical device");
        return false;
    }

    if (!CreateQueues()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create queues");
        return false;
    }

    ENJIN_LOG_INFO(Renderer, "Vulkan context initialized successfully");
    return true;
}

void VulkanContext::Shutdown() {
    if (m_Device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(m_Device);
        vkDestroyDevice(m_Device, nullptr);
        m_Device = VK_NULL_HANDLE;
    }

#ifdef ENJIN_BUILD_DEBUG
    DestroyDebugMessenger();
#endif

    if (m_Instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_Instance, nullptr);
        m_Instance = VK_NULL_HANDLE;
    }
}

bool VulkanContext::CreateInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Enjin Engine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Enjin Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef ENJIN_BUILD_DEBUG
    if (CheckValidationLayerSupport()) {
        createInfo.enabledLayerCount = static_cast<u32>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        ENJIN_LOG_WARN(Renderer, "Validation layers requested but not available");
    }
#else
    createInfo.enabledLayerCount = 0;
#endif

    VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create Vulkan instance: %d", result);
        return false;
    }

    return true;
}

bool VulkanContext::SelectPhysicalDevice() {
    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        ENJIN_LOG_ERROR(Renderer, "No Vulkan-compatible devices found");
        return false;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            m_PhysicalDevice = device;
            
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(device, &properties);
            ENJIN_LOG_INFO(Renderer, "Selected physical device: %s", properties.deviceName);
            
            return true;
        }
    }

    ENJIN_LOG_ERROR(Renderer, "No suitable Vulkan device found");
    return false;
}

bool VulkanContext::CreateLogicalDevice() {
    // Find queue families
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

    // Find graphics queue family
    for (u32 i = 0; i < queueFamilyCount; ++i) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            m_GraphicsQueueFamily = i;
            break;
        }
    }

    if (m_GraphicsQueueFamily == UINT32_MAX) {
        ENJIN_LOG_ERROR(Renderer, "No graphics queue family found");
        return false;
    }

    // For now, assume present queue is same as graphics queue
    // This will need to be updated when we integrate with windowing system
    m_PresentQueueFamily = m_GraphicsQueueFamily;

    // Create device
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_GraphicsQueueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0; // Will add swapchain extension later

#ifdef ENJIN_BUILD_DEBUG
    if (CheckValidationLayerSupport()) {
        createInfo.enabledLayerCount = static_cast<u32>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }
#endif

    VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create logical device: %d", result);
        return false;
    }

    return true;
}

bool VulkanContext::CreateQueues() {
    vkGetDeviceQueue(m_Device, m_GraphicsQueueFamily, 0, &m_GraphicsQueue);
    vkGetDeviceQueue(m_Device, m_PresentQueueFamily, 0, &m_PresentQueue);
    return true;
}

std::vector<const char*> VulkanContext::GetRequiredExtensions() const {
    std::vector<const char*> extensions;

#ifdef ENJIN_BUILD_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    // GLFW will provide surface extensions
    // This will be updated when windowing is integrated

    return extensions;
}

bool VulkanContext::CheckValidationLayerSupport() const {
    u32 layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : VALIDATION_LAYERS) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }

    return true;
}

bool VulkanContext::IsDeviceSuitable(VkPhysicalDevice device) const {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);

    // Prefer discrete GPU
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        return true;
    }

    // Fallback to integrated GPU
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
        return true;
    }

    return false;
}

#ifdef ENJIN_BUILD_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    
    (void)pUserData;
    
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        ENJIN_LOG_WARN(Renderer, "Vulkan: %s", pCallbackData->pMessage);
    }
    
    return VK_FALSE;
}

bool VulkanContext::CreateDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        m_Instance, "vkCreateDebugUtilsMessengerEXT");
    
    if (func != nullptr) {
        return func(m_Instance, &createInfo, nullptr, &m_DebugMessenger) == VK_SUCCESS;
    }
    
    return false;
}

void VulkanContext::DestroyDebugMessenger() {
    if (m_DebugMessenger != VK_NULL_HANDLE) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            m_Instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(m_Instance, m_DebugMessenger, nullptr);
        }
        m_DebugMessenger = VK_NULL_HANDLE;
    }
}
#endif

} // namespace Renderer
} // namespace Enjin

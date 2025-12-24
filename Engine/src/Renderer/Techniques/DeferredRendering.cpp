#include "Enjin/Renderer/Techniques/DeferredRendering.h"
#include "Enjin/Renderer/Vulkan/VulkanRenderer.h"
#include "Enjin/Renderer/Vulkan/VulkanPipeline.h"
#include "Enjin/Renderer/Vulkan/VulkanShader.h"
#include "Enjin/Renderer/Vulkan/VulkanContext.h"
#include "Enjin/Renderer/RenderPipeline/RenderPipeline.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"
#include <array>

namespace Enjin {
namespace Renderer {

DeferredRendering::DeferredRendering()
    : RenderingTechnique("DeferredRendering") {
}

DeferredRendering::~DeferredRendering() {
    Shutdown();
}

bool DeferredRendering::Initialize(VulkanRenderer* renderer, RenderPipeline* pipeline) {
    m_Renderer = renderer;
    m_Pipeline = pipeline;
    
    ENJIN_LOG_INFO(Renderer, "Initializing Deferred Rendering technique...");
    
    if (!renderer || !renderer->GetContext()) {
        ENJIN_LOG_ERROR(Renderer, "Invalid renderer or context");
        return false;
    }
    
    VulkanContext* context = renderer->GetContext();
    VkExtent2D extent = renderer->GetSwapchainExtent();
    m_Width = extent.width;
    m_Height = extent.height;
    
    // Create G-Buffer
    if (!CreateGBuffer()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create G-Buffer");
        return false;
    }
    
    // Create geometry pass (uses existing pipeline)
    if (!CreateGeometryPass()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create geometry pass");
        return false;
    }
    
    // Create lighting pass
    if (!CreateLightingPass()) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create lighting pass");
        return false;
    }
    
    m_Initialized = true;
    ENJIN_LOG_INFO(Renderer, "Deferred Rendering initialized");
    return true;
}

void DeferredRendering::Shutdown() {
    if (!m_Initialized) {
        return;
    }
    
    ENJIN_LOG_INFO(Renderer, "Shutting down Deferred Rendering");
    
    DestroyGBuffer();
    
    if (m_LightingPipeline) {
        m_LightingPipeline->Destroy();
        m_LightingPipeline.reset();
    }
    
    m_LightingVertexShader.reset();
    m_LightingFragmentShader.reset();
    
    if (m_LightingDescriptorPool != VK_NULL_HANDLE && m_Renderer && m_Renderer->GetContext()) {
        vkDestroyDescriptorPool(m_Renderer->GetContext()->GetDevice(), m_LightingDescriptorPool, nullptr);
        m_LightingDescriptorPool = VK_NULL_HANDLE;
    }
    
    if (m_LightingDescriptorSetLayout != VK_NULL_HANDLE && m_Renderer && m_Renderer->GetContext()) {
        vkDestroyDescriptorSetLayout(m_Renderer->GetContext()->GetDevice(), m_LightingDescriptorSetLayout, nullptr);
        m_LightingDescriptorSetLayout = VK_NULL_HANDLE;
    }
    
    m_Initialized = false;
}

void DeferredRendering::Render(f32 deltaTime) {
    if (!m_Renderer || !m_Pipeline) {
        return;
    }
    
    VkCommandBuffer cmd = m_Renderer->GetCurrentCommandBuffer();
    if (cmd == VK_NULL_HANDLE) {
        return;
    }
    
    // Step 1: Geometry Pass - Render to G-Buffer
    // This would be handled by RenderSystem rendering entities to G-Buffer
    
    // Step 2: Lighting Pass - Calculate lighting from G-Buffer
    // Render fullscreen quad with lighting shader
    
    // Step 3: Forward Pass - Render transparent objects
    // This would be handled separately
    
    (void)deltaTime;
    (void)cmd;
}

bool DeferredRendering::CreateGBuffer() {
    VulkanContext* context = m_Renderer->GetContext();
    
    // Create position buffer (RGB16F)
    m_GBuffer.position = std::make_unique<VulkanImage>(context);
    if (!m_GBuffer.position->Create(
        m_Width, m_Height,
        VK_FORMAT_R16G16B16A16_SFLOAT,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    )) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create position G-Buffer");
        return false;
    }
    
    // Create normal buffer (RGB16F)
    m_GBuffer.normal = std::make_unique<VulkanImage>(context);
    if (!m_GBuffer.normal->Create(
        m_Width, m_Height,
        VK_FORMAT_R16G16B16A16_SFLOAT,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    )) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create normal G-Buffer");
        return false;
    }
    
    // Create albedo buffer (RGBA8)
    m_GBuffer.albedo = std::make_unique<VulkanImage>(context);
    if (!m_GBuffer.albedo->Create(
        m_Width, m_Height,
        VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    )) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create albedo G-Buffer");
        return false;
    }
    
    // Create depth buffer
    m_GBuffer.depth = std::make_unique<VulkanImage>(context);
    if (!m_GBuffer.depth->Create(
        m_Width, m_Height,
        VK_FORMAT_D32_SFLOAT,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
    )) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create depth G-Buffer");
        return false;
    }
    
    ENJIN_LOG_INFO(Renderer, "G-Buffer created: %dx%d", m_Width, m_Height);
    return true;
}

void DeferredRendering::DestroyGBuffer() {
    m_GBuffer.position.reset();
    m_GBuffer.normal.reset();
    m_GBuffer.albedo.reset();
    m_GBuffer.depth.reset();
    
    if (m_GBuffer.framebuffer != VK_NULL_HANDLE && m_Renderer && m_Renderer->GetContext()) {
        vkDestroyFramebuffer(m_Renderer->GetContext()->GetDevice(), m_GBuffer.framebuffer, nullptr);
        m_GBuffer.framebuffer = VK_NULL_HANDLE;
    }
    
    if (m_GBuffer.renderPass != VK_NULL_HANDLE && m_Renderer && m_Renderer->GetContext()) {
        vkDestroyRenderPass(m_Renderer->GetContext()->GetDevice(), m_GBuffer.renderPass, nullptr);
        m_GBuffer.renderPass = VK_NULL_HANDLE;
    }
}

bool DeferredRendering::CreateGeometryPass() {
    // Geometry pass uses the standard render pass
    // G-Buffer framebuffer and render pass would be created here
    // For now, this is a placeholder
    
    VulkanContext* context = m_Renderer->GetContext();
    
    // Create render pass for G-Buffer
    std::array<VkAttachmentDescription, 4> attachments{};
    
    // Position attachment
    attachments[0].format = VK_FORMAT_R16G16B16A16_SFLOAT;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    // Normal attachment
    attachments[1] = attachments[0];
    
    // Albedo attachment
    attachments[2].format = VK_FORMAT_R8G8B8A8_SRGB;
    attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[2].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    // Depth attachment
    attachments[3].format = VK_FORMAT_D32_SFLOAT;
    attachments[3].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[3].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[3].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[3].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[3].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[3].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[3].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    std::array<VkAttachmentReference, 3> colorRefs{};
    colorRefs[0] = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    colorRefs[1] = {1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    colorRefs[2] = {2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    
    VkAttachmentReference depthRef{3, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<u32>(colorRefs.size());
    subpass.pColorAttachments = colorRefs.data();
    subpass.pDepthStencilAttachment = &depthRef;
    
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    
    VkResult result = vkCreateRenderPass(
        context->GetDevice(), &renderPassInfo, nullptr, &m_GBuffer.renderPass);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create G-Buffer render pass: %d", result);
        return false;
    }
    
    ENJIN_LOG_INFO(Renderer, "Geometry pass created");
    return true;
}

bool DeferredRendering::CreateLightingPass() {
    VulkanContext* context = m_Renderer->GetContext();
    
    // Create descriptor set layout for G-Buffer textures
    std::array<VkDescriptorSetLayoutBinding, 3> bindings{};
    
    bindings[0].binding = 0;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[0].descriptorCount = 1;
    bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    bindings[1].binding = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].descriptorCount = 1;
    bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    bindings[2].binding = 2;
    bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[2].descriptorCount = 1;
    bindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<u32>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    
    VkResult result = vkCreateDescriptorSetLayout(
        context->GetDevice(), &layoutInfo, nullptr, &m_LightingDescriptorSetLayout);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create lighting descriptor set layout: %d", result);
        return false;
    }
    
    // Create descriptor pool
    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[0].descriptorCount = 3;
    
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 1;
    
    result = vkCreateDescriptorPool(
        context->GetDevice(), &poolInfo, nullptr, &m_LightingDescriptorPool);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to create lighting descriptor pool: %d", result);
        return false;
    }
    
    // Allocate descriptor set
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_LightingDescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_LightingDescriptorSetLayout;
    
    result = vkAllocateDescriptorSets(
        context->GetDevice(), &allocInfo, &m_LightingDescriptorSet);
    if (result != VK_SUCCESS) {
        ENJIN_LOG_ERROR(Renderer, "Failed to allocate lighting descriptor set: %d", result);
        return false;
    }
    
    ENJIN_LOG_INFO(Renderer, "Lighting pass created (shader loading would be implemented here)");
    return true;
}

} // namespace Renderer
} // namespace Enjin

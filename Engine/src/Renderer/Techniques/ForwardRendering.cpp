#include "Enjin/Renderer/Techniques/ForwardRendering.h"
#include "Enjin/Renderer/Vulkan/VulkanRenderer.h"
#include "Enjin/Renderer/RenderPipeline/RenderPipeline.h"
#include "Enjin/Logging/Log.h"

namespace Enjin {
namespace Renderer {

ForwardRendering::ForwardRendering()
    : RenderingTechnique("ForwardRendering") {
}

ForwardRendering::~ForwardRendering() {
    Shutdown();
}

bool ForwardRendering::Initialize(VulkanRenderer* renderer, RenderPipeline* pipeline) {
    m_Renderer = renderer;
    m_Pipeline = pipeline;
    
    ENJIN_LOG_INFO(Renderer, "Initializing Forward Rendering technique...");
    
    // Forward rendering is simple - just use the default pipeline
    // No special setup needed
    
    m_Initialized = true;
    ENJIN_LOG_INFO(Renderer, "Forward Rendering initialized");
    return true;
}

void ForwardRendering::Shutdown() {
    if (!m_Initialized) {
        return;
    }
    
    ENJIN_LOG_INFO(Renderer, "Shutting down Forward Rendering");
    m_Initialized = false;
}

void ForwardRendering::Render(f32 deltaTime) {
    if (!m_Renderer || !m_Pipeline) {
        return;
    }
    
    // Forward rendering: render objects directly to framebuffer
    // The actual rendering is handled by RenderSystem via the pipeline
    // This technique just ensures forward rendering is active
    
    (void)deltaTime;
}

} // namespace Renderer
} // namespace Enjin

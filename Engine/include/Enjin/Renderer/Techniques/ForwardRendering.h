#pragma once

#include "Enjin/Renderer/Techniques/RenderingTechnique.h"

namespace Enjin {
namespace Renderer {

/**
 * @brief Forward Rendering Technique
 * 
 * Traditional forward rendering:
 * - Render objects directly to framebuffer
 * - Simple and straightforward
 * - Good for transparent objects
 * - Limited by overdraw
 */
class ENJIN_API ForwardRendering : public RenderingTechnique {
public:
    ForwardRendering();
    ~ForwardRendering() override;

    bool Initialize(VulkanRenderer* renderer, RenderPipeline* pipeline) override;
    void Shutdown() override;
    void Render(f32 deltaTime) override;
    std::string GetDescription() const override { return "Traditional forward rendering - simple and direct"; }

private:
    // Forward rendering specific resources
};

} // namespace Renderer
} // namespace Enjin

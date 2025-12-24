#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Renderer/Vulkan/VulkanRenderer.h"
#include "Enjin/Renderer/RenderPipeline/RenderPipeline.h"
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace Enjin {
namespace Renderer {

// Forward declarations
class VulkanRenderer;
class RenderPipeline;

/**
 * @brief Base class for rendering techniques
 * 
 * Allows easy switching between different rendering approaches:
 * - Forward Rendering
 * - Deferred Rendering
 * - Forward+ / Clustered Forward
 * - Ray Traced Rendering
 * - Custom techniques
 * 
 * @note All techniques must inherit from this base class
 */
class ENJIN_API RenderingTechnique {
public:
    RenderingTechnique(const std::string& name);
    virtual ~RenderingTechnique();

    /**
     * @brief Initialize the rendering technique
     * @param renderer The Vulkan renderer instance
     * @param pipeline The render pipeline
     * @return true if initialization succeeded
     */
    virtual bool Initialize(VulkanRenderer* renderer, RenderPipeline* pipeline) = 0;

    /**
     * @brief Shutdown and cleanup the technique
     */
    virtual void Shutdown() = 0;

    /**
     * @brief Render a frame using this technique
     * @param deltaTime Time since last frame
     */
    virtual void Render(f32 deltaTime) = 0;

    /**
     * @brief Get the name of this technique
     */
    const std::string& GetName() const { return m_Name; }

    /**
     * @brief Check if technique is initialized
     */
    bool IsInitialized() const { return m_Initialized; }

    /**
     * @brief Get technique description
     */
    virtual std::string GetDescription() const { return ""; }

protected:
    std::string m_Name;
    VulkanRenderer* m_Renderer = nullptr;
    RenderPipeline* m_Pipeline = nullptr;
    bool m_Initialized = false;
};

/**
 * @brief Rendering technique manager
 * 
 * Manages multiple rendering techniques and allows switching between them
 * at runtime. Supports hot-reloading and dynamic technique registration.
 */
class ENJIN_API RenderingTechniqueManager {
public:
    RenderingTechniqueManager();
    ~RenderingTechniqueManager();

    /**
     * @brief Register a rendering technique
     * @param technique The technique to register (ownership transferred)
     * @return true if registration succeeded
     */
    bool RegisterTechnique(std::unique_ptr<RenderingTechnique> technique);

    /**
     * @brief Unregister a technique by name
     */
    void UnregisterTechnique(const std::string& name);

    /**
     * @brief Switch to a different rendering technique
     * @param name Name of the technique to switch to
     * @return true if switch succeeded
     */
    bool SwitchTechnique(const std::string& name);

    /**
     * @brief Get current active technique
     */
    RenderingTechnique* GetCurrentTechnique() const;

    /**
     * @brief Get technique by name
     */
    RenderingTechnique* GetTechnique(const std::string& name) const;

    /**
     * @brief Get all registered technique names
     */
    std::vector<std::string> GetTechniqueNames() const;

    /**
     * @brief Initialize current technique
     */
    bool InitializeCurrent(VulkanRenderer* renderer, RenderPipeline* pipeline);

    /**
     * @brief Render using current technique
     */
    void Render(f32 deltaTime);

private:
    std::unordered_map<std::string, std::unique_ptr<RenderingTechnique>> m_Techniques;
    RenderingTechnique* m_CurrentTechnique = nullptr;
    std::string m_CurrentTechniqueName;
};

} // namespace Renderer
} // namespace Enjin

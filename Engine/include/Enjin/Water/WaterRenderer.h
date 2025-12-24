#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Renderer/Vulkan/VulkanRenderer.h"
#include "Enjin/Math/Vector.h"
#include <memory>

namespace Enjin {
namespace Water {

/**
 * @brief Water Renderer
 * 
 * Renders realistic water surfaces with:
 * - Wave simulation
 * - Reflection/refraction
 * - Underwater effects
 * - Foam and splashes
 * - Dynamic water levels
 */
class ENJIN_API WaterRenderer {
public:
    WaterRenderer();
    ~WaterRenderer();

    /**
     * @brief Initialize water renderer
     */
    bool Initialize(Renderer::VulkanRenderer* renderer);

    /**
     * @brief Shutdown water renderer
     */
    void Shutdown();

    /**
     * @brief Render water surface
     */
    void Render(f32 deltaTime, const Math::Vector3& cameraPosition);

    /**
     * @brief Set water level
     */
    void SetWaterLevel(f32 level) { m_WaterLevel = level; }

    /**
     * @brief Get water level
     */
    f32 GetWaterLevel() const { return m_WaterLevel; }

    /**
     * @brief Set wave parameters
     */
    void SetWaveAmplitude(f32 amplitude) { m_WaveAmplitude = amplitude; }
    void SetWaveFrequency(f32 frequency) { m_WaveFrequency = frequency; }
    void SetWaveSpeed(f32 speed) { m_WaveSpeed = speed; }

private:
    Renderer::VulkanRenderer* m_Renderer = nullptr;
    f32 m_WaterLevel = 0.0f;
    f32 m_WaveAmplitude = 0.5f;
    f32 m_WaveFrequency = 1.0f;
    f32 m_WaveSpeed = 1.0f;
    f32 m_Time = 0.0f;
    
    // Water-specific resources would be here
    // Reflection/refraction textures, etc.
};

} // namespace Water
} // namespace Enjin

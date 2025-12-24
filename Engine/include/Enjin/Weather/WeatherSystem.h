#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Math/Vector.h"
#include <string>

namespace Enjin {
namespace Weather {

/**
 * @brief Weather types
 */
enum class WeatherType {
    Clear,
    Rain,
    Snow,
    Fog,
    Storm,
    Custom
};

/**
 * @brief Weather System
 * 
 * Manages weather effects including:
 * - Rain particles
 * - Snow particles
 * - Fog effects
 * - Wind simulation
 * - Weather transitions
 */
class ENJIN_API WeatherSystem {
public:
    WeatherSystem();
    ~WeatherSystem();

    /**
     * @brief Initialize weather system
     */
    bool Initialize();

    /**
     * @brief Shutdown weather system
     */
    void Shutdown();

    /**
     * @brief Set current weather type
     */
    void SetWeather(WeatherType type, f32 intensity = 1.0f);

    /**
     * @brief Get current weather type
     */
    WeatherType GetCurrentWeather() const { return m_CurrentWeather; }

    /**
     * @brief Get weather intensity (0-1)
     */
    f32 GetIntensity() const { return m_Intensity; }

    /**
     * @brief Set weather intensity (0-1)
     */
    void SetIntensity(f32 intensity);

    /**
     * @brief Update weather system
     */
    void Update(f32 deltaTime);

    /**
     * @brief Get wind direction
     */
    Math::Vector3 GetWindDirection() const { return m_WindDirection; }

    /**
     * @brief Set wind direction
     */
    void SetWindDirection(const Math::Vector3& direction);

    /**
     * @brief Get wind speed
     */
    f32 GetWindSpeed() const { return m_WindSpeed; }

    /**
     * @brief Set wind speed
     */
    void SetWindSpeed(f32 speed) { m_WindSpeed = speed; }

    /**
     * @brief Get fog density
     */
    f32 GetFogDensity() const;

    /**
     * @brief Get fog color
     */
    Math::Vector3 GetFogColor() const;

private:
    WeatherType m_CurrentWeather = WeatherType::Clear;
    f32 m_Intensity = 0.0f;
    f32 m_TargetIntensity = 0.0f;
    f32 m_TransitionSpeed = 2.0f; // Intensity change per second
    
    Math::Vector3 m_WindDirection = Math::Vector3(1.0f, 0.0f, 0.0f);
    f32 m_WindSpeed = 1.0f;
    
    // Particle systems would be managed here
    // For now, just the interface
};

} // namespace Weather
} // namespace Enjin

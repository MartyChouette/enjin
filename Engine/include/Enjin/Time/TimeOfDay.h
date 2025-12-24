#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Math/Vector.h"
#include "Enjin/Math/Matrix.h"
#include <chrono>

namespace Enjin {
namespace Time {

/**
 * @brief Time of Day System
 * 
 * Manages day/night cycle with realistic sun movement and sky colors.
 * Supports:
 * - Customizable day length
 * - Realistic sun position calculation
 * - Dynamic sky colors based on time
 * - Sun/moon phases
 */
class ENJIN_API TimeOfDay {
public:
    /**
     * @brief Time of day in hours (0-24)
     */
    f32 GetTime() const { return m_Time; }
    
    /**
     * @brief Set time of day in hours (0-24)
     */
    void SetTime(f32 time);
    
    /**
     * @brief Get day length in real-world seconds
     */
    f32 GetDayLength() const { return m_DayLength; }
    
    /**
     * @brief Set day length in real-world seconds
     */
    void SetDayLength(f32 seconds) { m_DayLength = seconds; }
    
    /**
     * @brief Update time based on delta time
     */
    void Update(f32 deltaTime);
    
    /**
     * @brief Get sun direction (normalized)
     */
    Math::Vector3 GetSunDirection() const;
    
    /**
     * @brief Get sun color based on time of day
     */
    Math::Vector3 GetSunColor() const;
    
    /**
     * @brief Get sky color based on time of day
     */
    Math::Vector4 GetSkyColor() const;
    
    /**
     * @brief Get ambient light color
     */
    Math::Vector3 GetAmbientColor() const;
    
    /**
     * @brief Get sun intensity (0-1)
     */
    f32 GetSunIntensity() const;
    
    /**
     * @brief Check if it's day time
     */
    bool IsDay() const { return m_Time >= 6.0f && m_Time < 20.0f; }
    
    /**
     * @brief Check if it's night time
     */
    bool IsNight() const { return !IsDay(); }
    
    /**
     * @brief Get time of day as string (e.g., "12:30 PM")
     */
    std::string GetTimeString() const;

private:
    f32 m_Time = 12.0f; // Noon by default
    f32 m_DayLength = 300.0f; // 5 minutes real time = 24 hours game time
    
    Math::Vector3 CalculateSunDirection(f32 time) const;
    Math::Vector3 CalculateSunColor(f32 time) const;
    Math::Vector4 CalculateSkyColor(f32 time) const;
};

} // namespace Time
} // namespace Enjin

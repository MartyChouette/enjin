#include "Enjin/Time/TimeOfDay.h"
#include "Enjin/Math/Math.h"
#include "Enjin/Logging/Log.h"
#include <sstream>
#include <iomanip>

namespace Enjin {
namespace Time {

void TimeOfDay::SetTime(f32 time) {
    m_Time = Math::Fmod(time, 24.0f);
    if (m_Time < 0.0f) {
        m_Time += 24.0f;
    }
}

void TimeOfDay::Update(f32 deltaTime) {
    // Convert deltaTime to game time
    f32 gameTimeDelta = (deltaTime / m_DayLength) * 24.0f;
    SetTime(m_Time + gameTimeDelta);
}

Math::Vector3 TimeOfDay::GetSunDirection() const {
    return CalculateSunDirection(m_Time);
}

Math::Vector3 TimeOfDay::GetSunColor() const {
    return CalculateSunColor(m_Time);
}

Math::Vector4 TimeOfDay::GetSkyColor() const {
    return CalculateSkyColor(m_Time);
}

Math::Vector3 TimeOfDay::GetAmbientColor() const {
    Math::Vector3 sunColor = GetSunColor();
    f32 intensity = GetSunIntensity();
    
    // Ambient is darker version of sun color
    return sunColor * (0.3f + intensity * 0.2f);
}

f32 TimeOfDay::GetSunIntensity() const {
    // Sun is brightest at noon (12:00), dimmest at night
    f32 normalizedTime = m_Time / 24.0f;
    f32 angle = normalizedTime * Math::PI_2;
    
    // Sun intensity based on elevation
    f32 elevation = Math::Sin(angle - Math::PI_HALF);
    return Math::Clamp(elevation, 0.0f, 1.0f);
}

std::string TimeOfDay::GetTimeString() const {
    u32 hours = static_cast<u32>(m_Time);
    u32 minutes = static_cast<u32>((m_Time - hours) * 60.0f);
    
    bool isPM = hours >= 12;
    if (hours > 12) {
        hours -= 12;
    }
    if (hours == 0) {
        hours = 12;
    }
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes
       << (isPM ? " PM" : " AM");
    return ss.str();
}

Math::Vector3 TimeOfDay::CalculateSunDirection(f32 time) const {
    // Convert time (0-24) to angle (0-2π)
    f32 normalizedTime = time / 24.0f;
    f32 angle = normalizedTime * Math::PI_2;
    
    // Sun moves in a circle
    // At 6:00 (sunrise), sun is at horizon (0°)
    // At 12:00 (noon), sun is at zenith (90°)
    // At 18:00 (sunset), sun is at horizon (0°)
    // At 0:00 (midnight), sun is below horizon (-90°)
    
    f32 elevation = Math::Sin(angle - Math::PI_HALF);
    f32 azimuth = Math::Cos(angle - Math::PI_HALF);
    
    // Convert to direction vector
    Math::Vector3 direction;
    direction.x = azimuth;
    direction.y = elevation;
    direction.z = 0.0f; // Sun moves in X-Y plane
    
    return direction.Normalized();
}

Math::Vector3 TimeOfDay::CalculateSunColor(f32 time) const {
    f32 intensity = GetSunIntensity();
    
    if (intensity <= 0.0f) {
        // Night - moon light (blueish)
        return Math::Vector3(0.2f, 0.3f, 0.5f);
    }
    
    // Day - sun color changes based on time
    f32 normalizedTime = time / 24.0f;
    
    // Sunrise/sunset colors (reddish)
    if (time >= 5.0f && time < 7.0f) {
        // Sunrise
        f32 t = (time - 5.0f) / 2.0f;
        return Math::Lerp(Math::Vector3(1.0f, 0.5f, 0.3f), Math::Vector3(1.0f, 1.0f, 0.9f), t);
    } else if (time >= 17.0f && time < 19.0f) {
        // Sunset
        f32 t = (time - 17.0f) / 2.0f;
        return Math::Lerp(Math::Vector3(1.0f, 1.0f, 0.9f), Math::Vector3(1.0f, 0.5f, 0.3f), t);
    }
    
    // Midday - bright white/yellow
    return Math::Vector3(1.0f, 1.0f, 0.95f);
}

Math::Vector4 TimeOfDay::CalculateSkyColor(f32 time) const {
    f32 intensity = GetSunIntensity();
    
    if (intensity <= 0.0f) {
        // Night sky - dark blue
        return Math::Vector4(0.05f, 0.05f, 0.15f, 1.0f);
    }
    
    // Day sky - blue, brighter at noon
    f32 normalizedTime = time / 24.0f;
    
    // Sunrise/sunset - orange/red sky
    if (time >= 5.0f && time < 7.0f) {
        f32 t = (time - 5.0f) / 2.0f;
        return Math::Vector4(
            Math::Lerp(0.8f, 0.5f, t),
            Math::Lerp(0.4f, 0.7f, t),
            Math::Lerp(0.2f, 0.9f, t),
            1.0f
        );
    } else if (time >= 17.0f && time < 19.0f) {
        f32 t = (time - 17.0f) / 2.0f;
        return Math::Vector4(
            Math::Lerp(0.5f, 0.8f, t),
            Math::Lerp(0.7f, 0.4f, t),
            Math::Lerp(0.9f, 0.2f, t),
            1.0f
        );
    }
    
    // Midday - bright blue sky
    return Math::Vector4(0.5f, 0.7f, 1.0f, 1.0f);
}

} // namespace Time
} // namespace Enjin

#pragma once

#include "Enjin/Platform/Types.h"
#include <cmath>
#include <limits>

namespace Enjin {
namespace Math {

// Constants
constexpr f32 PI = 3.14159265358979323846f;
constexpr f32 PI_2 = PI * 2.0f;
constexpr f32 PI_HALF = PI * 0.5f;
constexpr f32 EPSILON = std::numeric_limits<f32>::epsilon();
constexpr f32 FLOAT_MAX = std::numeric_limits<f32>::max();
constexpr f32 FLOAT_MIN = std::numeric_limits<f32>::lowest();

// Utility functions
ENJIN_FORCE_INLINE f32 Radians(f32 degrees) { return degrees * PI / 180.0f; }
ENJIN_FORCE_INLINE f32 Degrees(f32 radians) { return radians * 180.0f / PI; }
ENJIN_FORCE_INLINE f32 Abs(f32 value) { return std::abs(value); }
ENJIN_FORCE_INLINE f32 Sqrt(f32 value) { return std::sqrt(value); }
ENJIN_FORCE_INLINE f32 Sin(f32 value) { return std::sin(value); }
ENJIN_FORCE_INLINE f32 Cos(f32 value) { return std::cos(value); }
ENJIN_FORCE_INLINE f32 Tan(f32 value) { return std::tan(value); }
ENJIN_FORCE_INLINE f32 Asin(f32 value) { return std::asin(value); }
ENJIN_FORCE_INLINE f32 Acos(f32 value) { return std::acos(value); }
ENJIN_FORCE_INLINE f32 Atan(f32 value) { return std::atan(value); }
ENJIN_FORCE_INLINE f32 Atan2(f32 y, f32 x) { return std::atan2(y, x); }
ENJIN_FORCE_INLINE f32 Pow(f32 base, f32 exponent) { return std::pow(base, exponent); }
ENJIN_FORCE_INLINE f32 Exp(f32 value) { return std::exp(value); }
ENJIN_FORCE_INLINE f32 Log(f32 value) { return std::log(value); }
ENJIN_FORCE_INLINE f32 Log2(f32 value) { return std::log2(value); }
ENJIN_FORCE_INLINE f32 Floor(f32 value) { return std::floor(value); }
ENJIN_FORCE_INLINE f32 Ceil(f32 value) { return std::ceil(value); }
ENJIN_FORCE_INLINE f32 Round(f32 value) { return std::round(value); }
ENJIN_FORCE_INLINE f32 Fmod(f32 x, f32 y) { return std::fmod(x, y); }

ENJIN_FORCE_INLINE f32 Min(f32 a, f32 b) { return a < b ? a : b; }
ENJIN_FORCE_INLINE f32 Max(f32 a, f32 b) { return a > b ? a : b; }
ENJIN_FORCE_INLINE f32 Clamp(f32 value, f32 min, f32 max) {
    return value < min ? min : (value > max ? max : value);
}
ENJIN_FORCE_INLINE f32 Lerp(f32 a, f32 b, f32 t) {
    return a + (b - a) * Clamp(t, 0.0f, 1.0f);
}
ENJIN_FORCE_INLINE f32 SmoothStep(f32 edge0, f32 edge1, f32 x) {
    f32 t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

// Comparison functions
ENJIN_FORCE_INLINE bool IsNearZero(f32 value, f32 epsilon = EPSILON) {
    return Abs(value) < epsilon;
}
ENJIN_FORCE_INLINE bool IsEqual(f32 a, f32 b, f32 epsilon = EPSILON) {
    return Abs(a - b) < epsilon;
}

} // namespace Math
} // namespace Enjin

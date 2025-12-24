# Coding Standards

## Documentation Requirements

### Header Comments

Every file should start with a header comment:

```cpp
/**
 * @file RenderPipeline.h
 * @brief Extensible rendering pipeline with hooks and hot-reload
 * @author Enjin Engine Team
 * @date 2024
 */
```

### Class Documentation

```cpp
/**
 * @brief Rendering technique manager
 * 
 * Manages multiple rendering techniques and allows switching between them
 * at runtime. Supports hot-reloading and dynamic technique registration.
 * 
 * @note Thread-safe for read operations
 * @warning Not thread-safe for write operations
 */
class RenderingTechniqueManager {
```

### Method Documentation

```cpp
/**
 * @brief Switch to a different rendering technique
 * 
 * Shuts down current technique and initializes the new one.
 * 
 * @param name Name of the technique to switch to
 * @return true if switch succeeded, false otherwise
 * 
 * @note This operation may be expensive (reinitializes technique)
 * @see RegisterTechnique()
 */
bool SwitchTechnique(const std::string& name);
```

### Parameter Documentation

```cpp
/**
 * @brief Set time of day
 * @param time Time in hours (0-24), automatically wraps
 * 
 * @example
 * time.SetTime(12.5f); // 12:30 PM
 * time.SetTime(25.0f); // Wraps to 1:00 AM
 */
void SetTime(f32 time);
```

## Code Style

### Naming Conventions

- **Classes**: PascalCase (`RenderingTechnique`)
- **Functions**: PascalCase (`Initialize`, `Render`)
- **Variables**: camelCase (`deltaTime`, `renderer`)
- **Members**: m_ prefix (`m_Renderer`, `m_Initialized`)
- **Constants**: UPPER_CASE (`MAX_TEXTURES`)
- **Namespaces**: PascalCase (`Renderer`, `Physics`)

### File Organization

```
Engine/include/Enjin/System/SystemName.h
Engine/src/System/SystemName.cpp
```

### Includes

Order:
1. Corresponding header
2. Engine headers (alphabetical)
3. Standard library (alphabetical)
4. Third-party (alphabetical)

```cpp
#include "Enjin/Renderer/RenderPipeline.h"

#include "Enjin/Logging/Log.h"
#include "Enjin/Math/Vector.h"

#include <memory>
#include <string>
#include <vector>
```

## Error Handling

### Logging

Always log errors:

```cpp
if (!Initialize()) {
    ENJIN_LOG_ERROR(Renderer, "Failed to initialize renderer");
    return false;
}
```

### Assertions

Use assertions for programming errors:

```cpp
ENJIN_ASSERT(renderer != nullptr, "Renderer cannot be null");
```

### Graceful Degradation

Provide fallbacks:

```cpp
if (!CreateAdvancedFeature()) {
    ENJIN_LOG_WARN(Renderer, "Advanced feature not available, using fallback");
    CreateFallbackFeature();
}
```

## Performance Notes

Document performance characteristics:

```cpp
/**
 * @brief Execute GPU culling
 * 
 * Processes all objects in parallel on GPU.
 * 
 * @performance O(n) where n is object count
 * @performance Runs on GPU, minimal CPU overhead
 * @performance Typical: 0.1ms for 10,000 objects
 */
bool ExecuteCulling(...);
```

## Thread Safety

Document thread safety:

```cpp
/**
 * @brief Get current technique
 * @return Current active technique or nullptr
 * 
 * @threadsafe Yes (read-only operation)
 */
RenderingTechnique* GetCurrentTechnique() const;
```

## Examples

Always include usage examples in documentation:

```cpp
/**
 * @example
 * RenderingTechniqueManager techniques;
 * techniques.RegisterTechnique(std::make_unique<ForwardRendering>());
 * techniques.SwitchTechnique("ForwardRendering");
 */
```

## Documentation Checklist

- [ ] File header comment
- [ ] Class documentation
- [ ] Method documentation
- [ ] Parameter documentation
- [ ] Return value documentation
- [ ] Usage examples
- [ ] Performance notes
- [ ] Thread safety notes
- [ ] Error handling notes

All code should be self-documenting and easy to understand!

#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Math/Vector.h"
#include <functional>

namespace Enjin {

struct WindowDesc {
    u32 width = 1280;
    u32 height = 720;
    const char* title = "Enjin Engine";
    bool resizable = true;
    bool fullscreen = false;
};

class ENJIN_API Window {
public:
    using EventCallback = std::function<void()>;
    using ResizeCallback = std::function<void(u32, u32)>;

    virtual ~Window() = default;

    virtual void PollEvents() = 0;
    virtual bool ShouldClose() const = 0;
    virtual void Close() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;
    virtual Math::Vector2 GetSize() const = 0;

    virtual void* GetNativeHandle() const = 0; // Returns platform-specific window handle

    virtual void SetEventCallback(const EventCallback& callback) = 0;
    virtual void SetResizeCallback(const ResizeCallback& callback) = 0;
};

ENJIN_API Window* CreateWindow(const WindowDesc& desc);
ENJIN_API void DestroyWindow(Window* window);

} // namespace Enjin

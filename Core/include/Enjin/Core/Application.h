#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Platform/Types.h"

namespace Enjin {

// Forward declarations
class Window;

// Application base class - Engine owns the entry point
class ENJIN_API Application {
public:
    Application();
    virtual ~Application();

    // Main entry point (called by engine)
    int Run();

    // Override these in your game
    virtual void Initialize() {}
    virtual void Shutdown() {}
    virtual void Update(f32 deltaTime) {}
    virtual void Render() {}

protected:
    Window* GetWindow() const { return m_Window; }

private:
    void InitializeEngine();
    void ShutdownEngine();
    void MainLoop();

    Window* m_Window = nullptr;
    bool m_Running = true;
    f32 m_LastFrameTime = 0.0f;
};

// User must implement this function
extern Application* CreateApplication();

} // namespace Enjin

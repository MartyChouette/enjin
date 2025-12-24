#include "Enjin/Core/Application.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Platform/Window.h"
#include <chrono>

namespace Enjin {

extern Window* CreateWindow(const WindowDesc& desc);
extern void DestroyWindow(Window* window);

namespace Enjin {

Application::Application() {
}

Application::~Application() {
}

int Application::Run() {
    InitializeEngine();
    Initialize();

    MainLoop();

    Shutdown();
    ShutdownEngine();

    return 0;
}

void Application::InitializeEngine() {
    ENJIN_LOG_INFO(Core, "Initializing Enjin Engine...");
    
    Logger::Get().Initialize();
    
    // Create window
    WindowDesc windowDesc;
    windowDesc.width = 1280;
    windowDesc.height = 720;
    windowDesc.title = "Enjin Engine";
    m_Window = CreateWindow(windowDesc);
    
    if (!m_Window) {
        ENJIN_LOG_FATAL(Core, "Failed to create window");
        m_Running = false;
        return;
    }
    
    ENJIN_LOG_INFO(Core, "Engine initialized successfully");
}

void Application::ShutdownEngine() {
    ENJIN_LOG_INFO(Core, "Shutting down Enjin Engine...");
    
    if (m_Window) {
        DestroyWindow(m_Window);
        m_Window = nullptr;
    }
    
    Logger::Get().Shutdown();
}

void Application::MainLoop() {
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (m_Running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTimeNs = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime);
        f32 deltaTime = static_cast<f32>(deltaTimeNs.count()) / 1'000'000'000.0f;
        lastTime = currentTime;

        // Update window events
        if (m_Window) {
            m_Window->PollEvents();
            if (m_Window->ShouldClose()) {
                m_Running = false;
                break;
            }
        }

        Update(deltaTime);
        Render();
    }
}

} // namespace Enjin

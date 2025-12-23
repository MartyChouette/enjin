#include "Enjin/Core/Application.h"
#include "Enjin/Logging/Log.h"

// Example: Render a triangle
class TriangleExample : public Enjin::Application {
public:
    void Initialize() override {
        ENJIN_LOG_INFO(Game, "Triangle Example starting...");
    }

    void Shutdown() override {
        ENJIN_LOG_INFO(Game, "Triangle Example shutting down...");
    }

    void Update(f32 deltaTime) override {
        (void)deltaTime;
        // Update logic
    }

    void Render() override {
        // Render triangle (Vulkan implementation coming soon)
    }
};

Enjin::Application* CreateApplication() {
    return new TriangleExample();
}

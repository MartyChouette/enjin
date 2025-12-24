#include "Enjin/Core/Application.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/ECS/World.h"
#include "Enjin/ECS/Systems/RenderSystem.h"
#include "Enjin/Renderer/Vulkan/VulkanRenderer.h"
#include <memory>

// Example: Render a triangle using ECS and Vulkan
class TriangleExample : public Enjin::Application {
public:
    void Initialize() override {
        ENJIN_LOG_INFO(Game, "Triangle Example starting...");

        // Initialize renderer
        m_Renderer = std::make_unique<Enjin::Renderer::VulkanRenderer>();
        if (!m_Renderer->Initialize(GetWindow())) {
            ENJIN_LOG_FATAL(Game, "Failed to initialize renderer");
            return;
        }

        // Create ECS world
        m_World = std::make_unique<Enjin::ECS::World>();

        // Register render system
        m_RenderSystem = m_World->RegisterSystem<Enjin::ECS::RenderSystem>(
            m_World.get(), m_Renderer.get());
        
        // Initialize render system (creates pipeline, buffers, etc.)
        m_RenderSystem->Initialize();

        ENJIN_LOG_INFO(Game, "Triangle Example initialized");
    }

    void Shutdown() override {
        ENJIN_LOG_INFO(Game, "Triangle Example shutting down...");
        
        if (m_RenderSystem) {
            m_RenderSystem->Shutdown();
        }
        m_RenderSystem = nullptr;
        m_World.reset();
        m_Renderer.reset();
    }

    void Update(f32 deltaTime) override {
        // Update ECS world
        if (m_World) {
            m_World->Update(deltaTime);
        }
    }

    void Render() override {
        if (!m_Renderer) {
            return;
        }

        // Begin frame (starts command buffer recording)
        m_Renderer->BeginFrame();

        // Render systems will be called during Update, which records draw commands
        // The actual rendering happens when EndFrame() submits the command buffer

        // End frame (submits command buffer and presents)
        m_Renderer->EndFrame();
    }

private:
    std::unique_ptr<Enjin::Renderer::VulkanRenderer> m_Renderer;
    std::unique_ptr<Enjin::ECS::World> m_World;
    Enjin::ECS::RenderSystem* m_RenderSystem = nullptr;
};

Enjin::Application* CreateApplication() {
    return new TriangleExample();
}

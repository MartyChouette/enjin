#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/ECS/System.h"
#include "Enjin/ECS/World.h"
#include "Enjin/ECS/Components/Transform.h"
#include "Enjin/ECS/Components/Mesh.h"
#include "Enjin/Renderer/Vulkan/VulkanRenderer.h"
#include <vulkan/vulkan.h>

namespace Enjin {
namespace ECS {

// Render system - renders entities with Transform and Mesh components
class ENJIN_API RenderSystem : public ISystem {
public:
    RenderSystem(World* world, Renderer::VulkanRenderer* renderer);
    ~RenderSystem();

    void Update(f32 deltaTime) override;
    void OnEntityAdded(Entity entity) override;
    void OnEntityRemoved(Entity entity) override;

private:
    void RenderEntity(Entity entity);
    void CreateTriangleMesh(); // Helper to create a triangle for testing

    World* m_World = nullptr;
    Renderer::VulkanRenderer* m_Renderer = nullptr;
    Entity m_TriangleEntity = INVALID_ENTITY;
};

} // namespace ECS
} // namespace Enjin

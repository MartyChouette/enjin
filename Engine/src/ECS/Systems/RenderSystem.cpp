#include "Enjin/ECS/Systems/RenderSystem.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Math/Math.h"

namespace Enjin {
namespace ECS {

RenderSystem::RenderSystem(World* world, Renderer::VulkanRenderer* renderer)
    : m_World(world), m_Renderer(renderer) {
    CreateTriangleMesh();
}

RenderSystem::~RenderSystem() {
}

void RenderSystem::Update(f32 deltaTime) {
    (void)deltaTime;

    if (!m_Renderer) {
        return;
    }

    // Render all entities with Transform and Mesh components
    // For now, we'll render the triangle entity
    if (m_TriangleEntity != INVALID_ENTITY) {
        RenderEntity(m_TriangleEntity);
    }
}

void RenderSystem::OnEntityAdded(Entity entity) {
    (void)entity;
    // Could set up rendering resources here
}

void RenderSystem::OnEntityRemoved(Entity entity) {
    (void)entity;
    // Clean up rendering resources
}

void RenderSystem::CreateTriangleMesh() {
    m_TriangleEntity = m_World->CreateEntity();

    // Add transform
    TransformComponent& transform = m_World->AddComponent<TransformComponent>(m_TriangleEntity);
    transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
    transform.scale = Math::Vector3(1.0f);

    // Add mesh (triangle)
    MeshComponent& mesh = m_World->AddComponent<MeshComponent>(m_TriangleEntity);
    mesh.vertices = {
        { Math::Vector3(0.0f, -0.5f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f), Math::Vector2(0.5f, 0.0f) },
        { Math::Vector3(0.5f, 0.5f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f), Math::Vector2(1.0f, 1.0f) },
        { Math::Vector3(-0.5f, 0.5f, 0.0f), Math::Vector3(0.0f, 0.0f, 1.0f), Math::Vector2(0.0f, 1.0f) }
    };
    mesh.indices = { 0, 1, 2 };

    ENJIN_LOG_INFO(Renderer, "Created triangle entity: %llu", m_TriangleEntity);
}

void RenderSystem::RenderEntity(Entity entity) {
    // For now, this is a placeholder
    // In a full implementation, you would:
    // 1. Get the command buffer from the renderer
    // 2. Bind vertex/index buffers
    // 3. Set up pipeline
    // 4. Draw the mesh
    
    TransformComponent* transform = m_World->GetComponent<TransformComponent>(entity);
    MeshComponent* mesh = m_World->GetComponent<MeshComponent>(entity);

    if (!transform || !mesh || !mesh->IsValid()) {
        return;
    }

    // TODO: Actual rendering will be implemented when we add pipelines
    (void)transform;
    (void)mesh;
}

} // namespace ECS
} // namespace Enjin

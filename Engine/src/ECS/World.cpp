#include "Enjin/ECS/World.h"
#include "Enjin/Logging/Log.h"

/**
 * @file World.cpp
 * @brief Implementation of World class
 * @author Enjin Engine Team
 * @date 2025
 */

namespace Enjin {
namespace ECS {

World::World() {
    m_SystemManager = std::make_unique<SystemManager>();
}

World::~World() {
    Clear();
}

Entity World::CreateEntity() {
    return m_EntityManager.CreateEntity();
}

void World::DestroyEntity(Entity entity) {
    if (!m_EntityManager.IsValid(entity)) {
        return;
    }

    m_SystemManager->OnEntityRemoved(entity);

    // Remove components from all storages
    for (auto& [typeId, storage] : m_ComponentStorages) {
        storage->Remove(entity);
    }

    m_EntityManager.DestroyEntity(entity);
}

bool World::IsValid(Entity entity) const {
    return m_EntityManager.IsValid(entity);
}

void World::Update(f32 deltaTime) {
    m_SystemManager->Update(deltaTime);
}

void World::Clear() {
    m_ComponentStorages.clear();
    m_EntityManager.Reset();
}

} // namespace ECS
} // namespace Enjin

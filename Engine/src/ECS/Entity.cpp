#include "Enjin/ECS/Entity.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Core/Assert.h"

namespace Enjin {
namespace ECS {

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

Entity EntityManager::CreateEntity() {
    Entity entity = m_NextEntity++;
    if (m_NextEntity == INVALID_ENTITY) {
        m_NextEntity = 1; // Wrap around (skip 0)
    }
    return entity;
}

void EntityManager::DestroyEntity(Entity entity) {
    // In a full implementation, you'd mark the entity as destroyed
    // and potentially reuse its ID. For now, we just validate.
    (void)entity;
}

bool EntityManager::IsValid(Entity entity) const {
    return entity != INVALID_ENTITY;
}

void EntityManager::Reset() {
    m_NextEntity = 1;
}

} // namespace ECS
} // namespace Enjin

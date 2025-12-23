#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Platform/Types.h"

namespace Enjin {
namespace ECS {

// Entity is just an ID
using Entity = u64;

// Invalid entity ID
constexpr Entity INVALID_ENTITY = 0;

// Entity manager - creates and manages entity IDs
class ENJIN_API EntityManager {
public:
    EntityManager();
    ~EntityManager();

    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    bool IsValid(Entity entity) const;

    void Reset(); // Destroy all entities

private:
    Entity m_NextEntity = 1;
    // In a full implementation, you'd track active entities
    // For now, we use a simple counter
};

} // namespace ECS
} // namespace Enjin

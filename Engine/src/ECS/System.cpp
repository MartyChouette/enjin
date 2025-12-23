#include "Enjin/ECS/System.h"
#include "Enjin/Logging/Log.h"

namespace Enjin {
namespace ECS {

SystemManager::SystemManager() {
}

SystemManager::~SystemManager() {
}

void SystemManager::Update(f32 deltaTime) {
    for (auto& system : m_Systems) {
        system->Update(deltaTime);
    }
}

void SystemManager::OnEntityAdded(Entity entity) {
    for (auto& system : m_Systems) {
        system->OnEntityAdded(entity);
    }
}

void SystemManager::OnEntityRemoved(Entity entity) {
    for (auto& system : m_Systems) {
        system->OnEntityRemoved(entity);
    }
}

} // namespace ECS
} // namespace Enjin

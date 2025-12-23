#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/ECS/Entity.h"
#include "Enjin/ECS/Component.h"
#include <vector>
#include <memory>

namespace Enjin {
namespace ECS {

// Forward declaration
class World;

// Base system interface
class ENJIN_API ISystem {
public:
    virtual ~ISystem() = default;
    virtual void Update(f32 deltaTime) = 0;
    virtual void OnEntityAdded(Entity entity) {}
    virtual void OnEntityRemoved(Entity entity) {}
};

// System manager - manages all systems
class ENJIN_API SystemManager {
public:
    SystemManager();
    ~SystemManager();

    template<typename T, typename... Args>
    T* RegisterSystem(Args&&... args) {
        static_assert(std::is_base_of_v<ISystem, T>, "T must inherit from ISystem");
        
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = system.get();
        m_Systems.push_back(std::move(system));
        return ptr;
    }

    void Update(f32 deltaTime);
    void OnEntityAdded(Entity entity);
    void OnEntityRemoved(Entity entity);

private:
    std::vector<std::unique_ptr<ISystem>> m_Systems;
};

} // namespace ECS
} // namespace Enjin

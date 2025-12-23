#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/ECS/Entity.h"
#include "Enjin/ECS/Component.h"
#include "Enjin/ECS/System.h"
#include <unordered_map>
#include <memory>

namespace Enjin {
namespace ECS {

// World - main ECS container
class ENJIN_API World {
public:
    World();
    ~World();

    // Entity management
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    bool IsValid(Entity entity) const;

    // Component management
    template<typename T>
    T& AddComponent(Entity entity, const T& component = T{}) {
        auto storage = GetOrCreateStorage<T>();
        if (storage->Has(entity)) {
            *storage->Get(entity) = component;
            return *storage->Get(entity);
        }
        T& comp = storage->Add(entity);
        comp = component;
        m_SystemManager->OnEntityAdded(entity);
        return comp;
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        auto storage = GetOrCreateStorage<T>();
        if (storage->Has(entity)) {
            storage->Remove(entity);
            m_SystemManager->OnEntityRemoved(entity);
        }
    }

    template<typename T>
    T* GetComponent(Entity entity) {
        auto storage = GetOrCreateStorage<T>();
        return storage->Get(entity);
    }

    template<typename T>
    const T* GetComponent(Entity entity) const {
        auto storage = GetStorage<T>();
        if (!storage) {
            return nullptr;
        }
        return storage->Get(entity);
    }

    template<typename T>
    bool HasComponent(Entity entity) const {
        auto storage = GetStorage<T>();
        if (!storage) {
            return false;
        }
        return storage->Has(entity);
    }

    // System management
    template<typename T, typename... Args>
    T* RegisterSystem(Args&&... args) {
        return m_SystemManager->RegisterSystem<T>(std::forward<Args>(args)...);
    }

    void Update(f32 deltaTime);

    void Clear(); // Clear all entities and components

private:
    // Type-erased component storage wrapper
    struct StorageBase {
        virtual ~StorageBase() = default;
    };

    template<typename T>
    struct StorageWrapper : public StorageBase {
        ComponentStorage<T> storage;
    };

    template<typename T>
    ComponentStorage<T>* GetOrCreateStorage() {
        ComponentTypeId typeId = ComponentRegistry::GetTypeId<T>();
        auto it = m_ComponentStorages.find(typeId);
        if (it == m_ComponentStorages.end()) {
            auto wrapper = std::make_unique<StorageWrapper<T>>();
            ComponentStorage<T>* ptr = &wrapper->storage;
            m_ComponentStorages[typeId] = std::move(wrapper);
            return ptr;
        }
        return &static_cast<StorageWrapper<T>*>(it->second.get())->storage;
    }

    template<typename T>
    const ComponentStorage<T>* GetStorage() const {
        ComponentTypeId typeId = ComponentRegistry::GetTypeId<T>();
        auto it = m_ComponentStorages.find(typeId);
        if (it == m_ComponentStorages.end()) {
            return nullptr;
        }
        return &static_cast<const StorageWrapper<T>*>(it->second.get())->storage;
    }

    EntityManager m_EntityManager;
    std::unique_ptr<SystemManager> m_SystemManager;
    std::unordered_map<ComponentTypeId, std::unique_ptr<StorageBase>> m_ComponentStorages;
};

} // namespace ECS
} // namespace Enjin

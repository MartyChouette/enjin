#include "Enjin/Physics/PhysicsWorld.h"
#include "Enjin/Logging/Log.h"
#include "Enjin/Math/Math.h"
#include <algorithm>

namespace Enjin {
namespace Physics {

PhysicsWorld::PhysicsWorld() {
}

PhysicsWorld::~PhysicsWorld() {
    Shutdown();
}

bool PhysicsWorld::Initialize() {
    ENJIN_LOG_INFO(Renderer, "Initializing Physics World...");
    return true;
}

void PhysicsWorld::Shutdown() {
    m_RigidBodies.clear();
}

void PhysicsWorld::SetGravity(const Math::Vector3& gravity) {
    m_Gravity = gravity;
}

void PhysicsWorld::AddRigidBody(std::shared_ptr<RigidBody> body) {
    if (body) {
        m_RigidBodies.push_back(body);
    }
}

void PhysicsWorld::RemoveRigidBody(std::shared_ptr<RigidBody> body) {
    m_RigidBodies.erase(
        std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), body),
        m_RigidBodies.end()
    );
}

void PhysicsWorld::Step(f32 deltaTime) {
    // Simple physics step
    Integrate(deltaTime);
    DetectCollisions();
    ResolveCollisions();
}

void PhysicsWorld::Integrate(f32 deltaTime) {
    for (auto& body : m_RigidBodies) {
        if (body->IsStatic()) {
            continue;
        }

        // Apply gravity
        Math::Vector3 acceleration = m_Gravity;
        
        // Update velocity
        body->SetVelocity(body->GetVelocity() + acceleration * deltaTime);
        
        // Update position
        body->SetPosition(body->GetPosition() + body->GetVelocity() * deltaTime);
    }
}

void PhysicsWorld::DetectCollisions() {
    // Simple AABB collision detection
    // In full implementation, would use spatial partitioning
    for (usize i = 0; i < m_RigidBodies.size(); ++i) {
        for (usize j = i + 1; j < m_RigidBodies.size(); ++j) {
            // Check collision between bodies[i] and bodies[j]
            // Simplified - would need collider shapes
        }
    }
}

void PhysicsWorld::ResolveCollisions() {
    // Resolve detected collisions
    // Simplified - would apply impulses, etc.
}

RigidBody::RigidBody() {
}

RigidBody::~RigidBody() {
}

} // namespace Physics
} // namespace Enjin

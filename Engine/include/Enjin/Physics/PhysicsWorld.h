#pragma once

#include "Enjin/Platform/Platform.h"
#include "Enjin/Math/Vector.h"
#include "Enjin/Math/Matrix.h"
#include <memory>
#include <vector>

namespace Enjin {
namespace Physics {

// Forward declarations
class RigidBody;
class Collider;

/**
 * @brief Physics World
 * 
 * Simple physics engine for:
 * - Rigid body dynamics
 * - Collision detection
 * - Gravity simulation
 * - Basic constraints
 * 
 * @note Designed to be simple and performant
 */
class ENJIN_API PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();

    /**
     * @brief Initialize physics world
     */
    bool Initialize();

    /**
     * @brief Shutdown physics world
     */
    void Shutdown();

    /**
     * @brief Set gravity
     */
    void SetGravity(const Math::Vector3& gravity);

    /**
     * @brief Get gravity
     */
    Math::Vector3 GetGravity() const { return m_Gravity; }

    /**
     * @brief Add rigid body to world
     */
    void AddRigidBody(std::shared_ptr<RigidBody> body);

    /**
     * @brief Remove rigid body from world
     */
    void RemoveRigidBody(std::shared_ptr<RigidBody> body);

    /**
     * @brief Step physics simulation
     * @param deltaTime Time step
     */
    void Step(f32 deltaTime);

    /**
     * @brief Get all rigid bodies
     */
    const std::vector<std::shared_ptr<RigidBody>>& GetRigidBodies() const {
        return m_RigidBodies;
    }

private:
    Math::Vector3 m_Gravity = Math::Vector3(0.0f, -9.81f, 0.0f);
    std::vector<std::shared_ptr<RigidBody>> m_RigidBodies;
    
    void Integrate(f32 deltaTime);
    void DetectCollisions();
    void ResolveCollisions();
};

/**
 * @brief Rigid Body
 */
class ENJIN_API RigidBody {
public:
    RigidBody();
    ~RigidBody();

    Math::Vector3 GetPosition() const { return m_Position; }
    void SetPosition(const Math::Vector3& pos) { m_Position = pos; }

    Math::Vector3 GetVelocity() const { return m_Velocity; }
    void SetVelocity(const Math::Vector3& vel) { m_Velocity = vel; }

    f32 GetMass() const { return m_Mass; }
    void SetMass(f32 mass) { m_Mass = mass; }

    bool IsStatic() const { return m_IsStatic; }
    void SetStatic(bool isStatic) { m_IsStatic = isStatic; }

private:
    Math::Vector3 m_Position = Math::Vector3(0.0f);
    Math::Vector3 m_Velocity = Math::Vector3(0.0f);
    f32 m_Mass = 1.0f;
    bool m_IsStatic = false;
};

} // namespace Physics
} // namespace Enjin

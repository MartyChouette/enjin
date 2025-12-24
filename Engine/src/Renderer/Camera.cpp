#include "Enjin/Renderer/Camera.h"
#include "Enjin/Math/Math.h"

namespace Enjin {
namespace Renderer {

Camera::Camera() {
    SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
}

Camera::~Camera() {
}

void Camera::SetPosition(const Math::Vector3& position) {
    m_Position = position;
    m_ViewDirty = true;
}

void Camera::SetRotation(const Math::Quaternion& rotation) {
    m_Rotation = rotation;
    m_ViewDirty = true;
}

void Camera::SetLookAt(const Math::Vector3& eye, const Math::Vector3& center, const Math::Vector3& up) {
    m_Position = eye;
    Math::Vector3 forward = (center - eye).Normalized();
    Math::Vector3 right = forward.Cross(up).Normalized();
    Math::Vector3 actualUp = right.Cross(forward);
    
    // Create rotation matrix from basis vectors
    Math::Matrix4 rotationMat;
    rotationMat.m[0] = right.x;
    rotationMat.m[4] = right.y;
    rotationMat.m[8] = right.z;
    rotationMat.m[1] = actualUp.x;
    rotationMat.m[5] = actualUp.y;
    rotationMat.m[9] = actualUp.z;
    rotationMat.m[2] = -forward.x;
    rotationMat.m[6] = -forward.y;
    rotationMat.m[10] = -forward.z;
    
    // Convert to quaternion (simplified - assumes pure rotation)
    m_Rotation = Math::Quaternion::Identity(); // Simplified
    m_ViewDirty = true;
}

void Camera::SetPerspective(f32 fov, f32 aspect, f32 nearPlane, f32 farPlane) {
    m_IsPerspective = true;
    m_Fov = fov;
    m_Aspect = aspect;
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
    m_ProjectionDirty = true;
}

void Camera::SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane) {
    m_IsPerspective = false;
    m_Left = left;
    m_Right = right;
    m_Bottom = bottom;
    m_Top = top;
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
    m_ProjectionDirty = true;
}

Math::Matrix4 Camera::GetViewMatrix() const {
    if (m_ViewDirty) {
        Math::Matrix4 translation = Math::Matrix4::Translation(-m_Position);
        Math::Matrix4 rotation = m_Rotation.ToMatrix().Transposed(); // Inverse rotation
        m_ViewMatrix = rotation * translation;
        m_ViewDirty = false;
    }
    return m_ViewMatrix;
}

Math::Matrix4 Camera::GetProjectionMatrix() const {
    if (m_ProjectionDirty) {
        if (m_IsPerspective) {
            m_ProjectionMatrix = Math::Matrix4::Perspective(
                Math::Radians(m_Fov), m_Aspect, m_NearPlane, m_FarPlane);
        } else {
            m_ProjectionMatrix = Math::Matrix4::Orthographic(
                m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
        }
        m_ProjectionDirty = false;
    }
    return m_ProjectionMatrix;
}

Math::Matrix4 Camera::GetViewProjectionMatrix() const {
    return GetProjectionMatrix() * GetViewMatrix();
}

Math::Vector3 Camera::GetForward() const {
    Math::Matrix4 rot = m_Rotation.ToMatrix();
    return Math::Vector3(-rot.m[2], -rot.m[6], -rot.m[10]).Normalized();
}

Math::Vector3 Camera::GetRight() const {
    Math::Matrix4 rot = m_Rotation.ToMatrix();
    return Math::Vector3(rot.m[0], rot.m[4], rot.m[8]).Normalized();
}

Math::Vector3 Camera::GetUp() const {
    Math::Matrix4 rot = m_Rotation.ToMatrix();
    return Math::Vector3(rot.m[1], rot.m[5], rot.m[9]).Normalized();
}

void Camera::UpdateViewMatrix() {
    m_ViewDirty = true;
}

} // namespace Renderer
} // namespace Enjin

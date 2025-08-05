#include <qk_gui/PCH.h>
#include <qk_gui/Camera.h>

namespace qk_gui
{
    Vector3 Camera::Forward() const noexcept
    {
        Vector3 forward{};
        forward.x = std::cos(dx::XMConvertToRadians(m_yaw)) * std::cos(dx::XMConvertToRadians(m_pitch));
        forward.y = std::sin(dx::XMConvertToRadians(m_pitch));
        forward.z = std::sin(dx::XMConvertToRadians(m_yaw)) * std::cos(dx::XMConvertToRadians(m_pitch));
        forward.Normalize();
        return forward;
    }
    void Camera::AddPitch(float angle) noexcept
    {
        m_pitch = std::clamp(m_pitch + angle, MIN_PITCH, MAX_PITCH);
    }
    Vector3 Camera::Right() const noexcept
    {
        Vector3 right{ Forward() };
        right = right.Cross(UP);
        right.Normalize();
        return right;
    }
}

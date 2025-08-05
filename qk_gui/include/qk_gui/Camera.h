#pragma once

#include <qk_gui/Math.h>

namespace qk_gui
{
    class Camera
    {
    private:
        constexpr static float START_YAW{ -90.0f };
        constexpr static float MIN_PITCH{ -89.0f };
        constexpr static float MAX_PITCH{ -89.0f };
        constexpr static Vector3 UP{ 0.0f, 1.0f, 0.0f };
        constexpr static Vector3 START_POSITION{ 0.0f, 1.0f, 5.0f };
    public:
        Camera() : m_position{ START_POSITION }, m_pitch{}, m_yaw{ START_YAW } {}
    public:
        Vector3 Position() const noexcept { return m_position; }
        Vector3 Target() const noexcept { return m_position + Forward(); }
        Vector3 Up() const noexcept { return UP; }
        Vector3 Forward() const noexcept;
        Vector3 Right() const noexcept;
    public:
        void AddPitch(float angle) noexcept;
        void AddYaw(float angle) noexcept { m_yaw += angle; }
    public:
        void Move(Vector3 move) noexcept { m_position += move; }
    private:
        Vector3 m_position;
        float m_pitch;
        float m_yaw;
    };
}
#pragma once

#include <cstddef>

namespace qk::r3d
{
    class MeshID
    {
    public:
        constexpr MeshID() : m_id{} {}
        constexpr explicit MeshID(std::size_t id) : m_id{ id } {}
    public:
        explicit operator std::size_t() const noexcept { return m_id; }
    public:
        bool operator==(const MeshID& other) const noexcept { return m_id == other.m_id; }
    private:
        std::size_t m_id;
    };

    constexpr MeshID CUBE{ 0 };
    constexpr MeshID QUAD{ 1 };
    constexpr MeshID ICOSPHERE{ 2 };
    constexpr MeshID CONE{ 3 };
    constexpr MeshID CONE_NO_BASE{ 4 };

    struct MeshVertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uvs;
    };

    using MeshIndex = std::uint32_t;
}

#pragma once

#include <qk/Vectors.h>

#include <cstdint>
#include <cstddef>

namespace qk
{
    class MeshID
    {
    public:
        constexpr MeshID() : m_id{} {}
        constexpr explicit MeshID(std::size_t id) : m_id{ id } {}
    public:
        bool operator==(const MeshID& other) const { return m_id == other.m_id; }
    private:
        std::size_t m_id;
    };

    constexpr MeshID CUBE_MESH_ID{ 0 };
    constexpr MeshID QUAD_MESH_ID{ 1 };

    struct MeshVertex
    {
        qk::v3 position;
        qk::v3 normal;
        qk::v2 uvs;
    };

    using MeshIndex = std::uint32_t;
}

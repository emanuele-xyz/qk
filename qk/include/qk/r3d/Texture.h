#pragma once

#include <cstddef>

namespace qk::r3d
{
    class TextureID
    {
    public:
        constexpr TextureID() : m_id{} {}
        constexpr explicit TextureID(std::size_t id) : m_id{ id } {}
    public:
        explicit operator std::size_t() const noexcept { return m_id; }
    public:
        bool operator==(const TextureID& other) const noexcept { return m_id == other.m_id; }
    private:
        std::size_t m_id;
    };

    constexpr TextureID TEXTURE_ID_INVALID{ 0 };
    constexpr TextureID ALBEDO_BLACK{ 1 };
    constexpr TextureID ALBEDO_WHITE{ 2 };
    constexpr TextureID ALBEDO_PINK{ 3 };
    constexpr TextureID ALBEDO_CHECKER{ 4 };
}

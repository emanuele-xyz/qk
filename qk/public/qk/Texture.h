#pragma once

#include <cstddef>

namespace qk
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

    constexpr TextureID ALBEDO_BLACK{ 0 };
    constexpr TextureID ALBEDO_WHITE{ 1 };
    constexpr TextureID ALBEDO_PINK{ 2 };
    constexpr TextureID ALBEDO_CHECKER{ 3 };
}

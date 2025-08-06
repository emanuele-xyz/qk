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

    constexpr TextureID BLACK_TEXTURE_ID{ 0 };
}

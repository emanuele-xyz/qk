#pragma once

#include <array>

namespace qk_gui
{
    enum class Key
    {
        W,
        A,
        S,
        D,
        Shift,
        Space,
        Ctrl,
        Count
    };

    class Keyboard
    {
    public:
        Keyboard();
        ~Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        Keyboard(Keyboard&&) noexcept = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(Keyboard&&) noexcept = delete;
    public:
        bool KeyState(Key key) const noexcept { return m_current[static_cast<int>(key)]; }
    public:
        void Update() noexcept;
    private:
        std::array<bool, static_cast<int>(Key::Count)> m_previous;
        std::array<bool, static_cast<int>(Key::Count)> m_current;
    };
}

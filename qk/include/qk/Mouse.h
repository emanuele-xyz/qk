#pragma once

#include <array>

namespace qk
{
    enum class Button
    {
        Left,
        Right,
        Count
    };

    class Mouse
    {
    private:
        struct State
        {
            std::array<bool, static_cast<int>(Button::Count)> buttons;
            int x, y;
        };
    public:
        Mouse();
        ~Mouse() = default;
        Mouse(const Mouse&) = delete;
        Mouse(Mouse&&) noexcept = delete;
        Mouse& operator=(const Mouse&) = delete;
        Mouse& operator=(Mouse&&) noexcept = delete;
    public:
        bool ButtonState(Button button) const noexcept { return m_current.buttons[static_cast<int>(button)]; }
        int CursorX() const noexcept { return m_current.x; }
        int CursorY() const noexcept { return m_current.y; }
        int CursorDX() const noexcept { return m_current.x - m_previous.x; }
        int CursorDY() const noexcept { return m_current.y - m_previous.y; }
    public:
        void Update(void* hwnd);
    private:
        State m_previous;
        State m_current;
    };
}

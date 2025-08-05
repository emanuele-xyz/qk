#include <qk_gui/PCH.h>
#include <qk_gui/Mouse.h>
#include <qk_gui/W32.h>

namespace qk_gui
{
    static int GetKeyCode(Button button)
    {
        int code{};
        switch (button)
        {
        case Button::Left: { code = VK_LBUTTON; } break;
        case Button::Right: { code = VK_RBUTTON; } break;
        default: { qk_gui_Unreachable(); } break;
        }
        return code;
    }

    Mouse::Mouse()
        : m_previous{}
        , m_current{}
    {
    }
    void Mouse::Update(void* hwnd)
    {
        // copy current state into previous
        m_previous = m_current;

        // clear current state
        m_current = {};

        // fetch current state
        {
            // fetch buttons
            for (int i{}; i < static_cast<int>(Button::Count); i++)
            {
                SHORT state{ GetKeyState(GetKeyCode(static_cast<Button>(i))) };
                m_current.buttons[i] = (state & 0x8000); // high order bit is 1 iff the key is down
            }

            // fetch cursor client position
            {
                POINT position{};
                qk_gui_Check(GetCursorPos(&position));
                qk_gui_Check(ScreenToClient(static_cast<HWND>(hwnd), &position));
                m_current.x = position.x;
                m_current.y = position.y;
            }
        }
    }
}

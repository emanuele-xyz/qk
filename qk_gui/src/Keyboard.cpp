#include <qk_gui/PCH.h>
#include <qk_gui/Keyboard.h>
#include <qk_gui/Commons.h>
#include <qk_gui/W32.h>

namespace qk_gui
{
    static int GetKeyCode(Key key)
    {
        int code{};
        switch (key)
        {
        case Key::W: { code = 'W'; } break;
        case Key::A: { code = 'A'; } break;
        case Key::S: { code = 'S'; } break;
        case Key::D: { code = 'D'; } break;
        case Key::Shift: { code = VK_SHIFT; } break;
        case Key::Space: { code = VK_SPACE; } break;
        case Key::Ctrl: { code = VK_CONTROL; } break;
        default: { qk_gui_Unreachable(); } break;
        }
        return code;
    }

    Keyboard::Keyboard()
        : m_previous{}
        , m_current{}
    {
    }
    void Keyboard::Update() noexcept
    {
        // copy current state into previous
        m_previous = m_current;

        // clear current state
        m_current = {};

        // fetch current state
        for (int i{}; i < static_cast<int>(Key::Count); i++)
        {
            SHORT state{ GetKeyState(GetKeyCode(static_cast<Key>(i))) };
            m_current[i] = (state & 0x8000); // high order bit is 1 iff the key is down
        }
    }
}
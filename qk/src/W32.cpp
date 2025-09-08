#include <qk/PCH.h>
#include <qk/W32.h>

namespace qk::w32
{
    WindowClass::WindowClass(const char* name, WNDPROC window_proc)
        : m_class_name{ name }
    {
        WNDCLASSEXA wc{};
        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = window_proc;
        //wc.cbClsExtra = ;
        //wc.cbWndExtra = ;
        wc.hInstance = GetModuleHandleA(nullptr);
        wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
        wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
        //wc.hbrBackground = ;
        //wc.lpszMenuName = ;
        wc.lpszClassName = name;
        wc.hIconSm = LoadIconA(0, IDI_APPLICATION);

        qk_Check(RegisterClassExA(&wc));
    }
    WindowClass::~WindowClass()
    {
        UnregisterClassA(m_class_name.c_str(), GetModuleHandleA(nullptr));
    }

    WindowHandle::WindowHandle(const char* title, const char* class_name, DWORD style, int client_w, int client_h, void* user_data)
        : m_handle{}
    {
        int w{}, h{};
        if (client_w == 0 || client_h == 0)
        {
            w = h = CW_USEDEFAULT;
        }
        else
        {
            RECT rect{ 0, 0, client_w, client_h };
            qk_Check(AdjustWindowRect(&rect, style, false));
            w = rect.right - rect.left;
            h = rect.bottom - rect.top;
        }

        m_handle = CreateWindowA(class_name, title, style, CW_USEDEFAULT, CW_USEDEFAULT, w, h, nullptr, nullptr, GetModuleHandleA(nullptr), user_data);
        qk_Check(m_handle);

        ShowWindow(m_handle, SW_SHOW);
    }
    WindowHandle::~WindowHandle()
    {
        DestroyWindow(m_handle);
    }
    std::pair<int, int> WindowHandle::Dimensions() const
    {
        RECT rect{};
        qk_Check(GetClientRect(m_handle, &rect));
        return std::make_pair(rect.right, rect.bottom);
    }

    void PumpMessages()
    {
        MSG msg{};
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
}

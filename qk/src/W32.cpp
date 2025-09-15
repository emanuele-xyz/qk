#include <qk/PCH.h>
#include <qk/W32.h>

#include <io.h>
#include <fcntl.h>
#include <tlhelp32.h>

#include <scope_guard.hpp>

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

    DWORD GetParentProcessId(DWORD pid)
    {
        HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        qk_Check(snapshot != INVALID_HANDLE_VALUE);
        auto close_snapshot_handle_on_oos{ sg::make_scope_guard([=]() { CloseHandle(snapshot); }) };

        PROCESSENTRY32 process_entry{};
        process_entry.dwSize = sizeof(PROCESSENTRY32);
        qk_Check(Process32First(snapshot, &process_entry));

        DWORD pid_parent{};
        do
        {
            if (process_entry.th32ProcessID == pid)
            {
                pid_parent = process_entry.th32ParentProcessID;
            }
        } while (Process32Next(snapshot, &process_entry) && !pid_parent);

        return pid_parent;
    }
    std::string GetProcessName(DWORD pid)
    {
        HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        qk_Check(snapshot != INVALID_HANDLE_VALUE);
        auto close_snapshot_handle_on_oos{ sg::make_scope_guard([=]() { CloseHandle(snapshot); }) };

        PROCESSENTRY32 process_entry{};
        process_entry.dwSize = sizeof(PROCESSENTRY32);
        qk_Check(Process32First(snapshot, &process_entry));

        std::string name{};
        do
        {
            if (process_entry.th32ProcessID == pid)
            {
                name = std::string{ process_entry.szExeFile };
            }
        } while (Process32Next(snapshot, &process_entry) && name.empty());

        return name;
    }
}

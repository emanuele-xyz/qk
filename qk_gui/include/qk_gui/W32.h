#pragma once

#include <qk_gui/Commons.h>

#include <string>
#include <utility>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <wrl/client.h> // for ComPtr
namespace wrl = Microsoft::WRL;

#define qk_gui_CheckHR(hr) qk_gui_Check(SUCCEEDED(hr))

namespace qk_gui::w32
{
    class WindowClass
    {
    public:
        WindowClass(const char* name, WNDPROC window_proc);
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) noexcept = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        WindowClass& operator=(WindowClass&&) noexcept = delete;
    private:
        std::string m_class_name;
    };

    class WindowHandle
    {
    public:
        WindowHandle(const char* title, const char* class_name, DWORD style, int client_w = 0, int client_h = 0, void* user_data = nullptr);
        ~WindowHandle();
        WindowHandle(const WindowHandle&) = delete;
        WindowHandle(WindowHandle&&) noexcept = delete;
        WindowHandle& operator=(const WindowHandle&) = delete;
        WindowHandle& operator=(WindowHandle&&) noexcept = delete;
    public:
        HWND Handle() const noexcept { return m_handle; }
    public:
        std::pair<int, int> Dimensions() const;
    private:
        HWND m_handle;
    };

    void PumpMessages();
}

#include <qk_gui/PCH.h>
#include <qk_gui/QkGUI.h>
#include <qk_gui/W32.h>
#include <qk_gui/D11.h>

namespace qk_gui
{
    constexpr const char* WINDOW_CLASS_NAME{ "qk_gui" };
    constexpr const char* WINDOW_TITLE{ "Quick GUI" };
    constexpr int MIN_WINDOW_DIMENSION{ 8 };

    struct AppContext
    {
        bool is_running{ true };
        bool did_resize{};
    };

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        LRESULT result{};

        // get application context
        auto ctx{ reinterpret_cast<AppContext*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA)) };

        // per message custom logic
        switch (msg)
        {
        case WM_CREATE:
        {
            auto create_struct{ reinterpret_cast<CREATESTRUCTA*>(lparam) };
            SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create_struct->lpCreateParams));
        } break;
        case WM_CLOSE:
        {
            ctx->is_running = false;
        } break;
        case WM_SIZE:
        {
            ctx->did_resize = true;
        } break;
        }

        // capture/forward message to default window procedure
        switch (msg)
        {
        case WM_CLOSE:
        {
            // don't forward to default window procedure
        } break;
        default:
        {
            result = DefWindowProcA(hwnd, msg, wparam, lparam);
        } break;
        }

        return result;
    }

    static void Entry()
    {
        AppContext app_context{};
        qk_gui_Check(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE));
        w32::WindowClass window_class{ WINDOW_CLASS_NAME, WindowProcedure };
        w32::WindowHandle window{ WINDOW_TITLE, WINDOW_CLASS_NAME, WS_OVERLAPPEDWINDOW, 0, 0, &app_context };
        d11::SetupDXGIInforQueue();
        wrl::ComPtr<ID3D11Device> d3d_dev{ d11::CreateDevice(D3D_FEATURE_LEVEL_11_0) };
        d11::SetupD11InfoQueue(d3d_dev.Get());
        wrl::ComPtr<ID3D11DeviceContext> d3d_ctx{};
        d3d_dev->GetImmediateContext(d3d_ctx.ReleaseAndGetAddressOf());
        wrl::ComPtr<IDXGISwapChain1> swap_chain{ d11::CreateSwapChain(window.Handle(), d3d_dev.Get()) };
        d11::FrameBuffer frame_buffer{ d3d_dev.Get(), swap_chain.Get() };

        while (app_context.is_running)
        {
            w32::PumpMessages();

            auto [window_w, window_h] { window.Dimensions() };

            // sanitize window dimensions so that they cannot be zero
            window_w = std::max(MIN_WINDOW_DIMENSION, window_w);
            window_h = std::max(MIN_WINDOW_DIMENSION, window_h);

            if (app_context.did_resize)
            {
                // clear d11 context state (some resources may be implicitly referenced by the context)
                d3d_ctx->ClearState();

                // destroy frame buffer
                frame_buffer = {};

                // resize swap chain
                qk_gui_CheckHR(swap_chain->ResizeBuffers(0, window_w, window_h, DXGI_FORMAT_UNKNOWN, 0));

                // create new frame buffer
                frame_buffer = { d3d_dev.Get(), swap_chain.Get() };

                // resize event has been handled
                app_context.did_resize = false;
            }

            // clear back buffer rtv (TODO: to be removed)
            {
                float clear_color[4]{ 0.2f, 0.3f, 0.3f, 1.0f };
                d3d_ctx->ClearRenderTargetView(frame_buffer.BackBufferRTV(), clear_color);
            }

            // present
            qk_gui_CheckHR(swap_chain->Present(1, 0)); // use vsync
        }
    }
}

int main()
{
    try
    {
        qk_gui::Entry();
    }
    catch (const qk_gui::Crash& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}

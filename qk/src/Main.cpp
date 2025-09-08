#include <qk/PCH.h>
#include <qk/Commons.h>
#include <qk/W32.h>
#include <qk/D11.h>
#include <qk/imguiex/ImGuiHandle.h>
#include <qk/Keyboard.h>
#include <qk/Mouse.h>
#include <qk/Editor.h>

#include <imgui.h>
#include <imgui_impl_win32.h>

// forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace qk
{
    constexpr const char* WINDOW_CLASS_NAME{ "qk" };
    constexpr const char* WINDOW_TITLE{ "Quick" };
    constexpr int MIN_WINDOW_DIMENSION{ 8 };

    struct AppContext
    {
        bool is_running{ true };
        bool did_resize{};
    };

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
        {
            return 1;
        }
        else
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
    }

    static void Entry()
    {
        AppContext app_context{};
        qk_Check(SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2));

        w32::WindowClass window_class{ WINDOW_CLASS_NAME, WindowProcedure };
        w32::WindowHandle window{ WINDOW_TITLE, WINDOW_CLASS_NAME, WS_OVERLAPPEDWINDOW, 0, 0, &app_context };
        d11::SetupDXGIInforQueue();
        wrl::ComPtr<ID3D11Device> d3d_dev{ d11::CreateDevice(D3D_FEATURE_LEVEL_11_0) };
        d11::SetupD11InfoQueue(d3d_dev.Get());
        wrl::ComPtr<ID3D11DeviceContext> d3d_ctx{};
        d3d_dev->GetImmediateContext(d3d_ctx.ReleaseAndGetAddressOf());
        wrl::ComPtr<IDXGISwapChain1> swap_chain{ d11::CreateSwapChain(window.Handle(), d3d_dev.Get()) };
        d11::FrameBuffer frame_buffer{ d3d_dev.Get(), swap_chain.Get() };
        ImGuiEx::ImGuiHandle imgui_handle{ window.Handle(), d3d_dev.Get(), d3d_ctx.Get() };
        r3d::Renderer renderer{ d3d_dev.Get(), d3d_ctx.Get() };
        Keyboard keyboard{};
        Mouse mouse{};
        Editor editor{ keyboard, mouse };
        Stopwatch frame_stopwatch{};

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
                qk_CheckHR(swap_chain->ResizeBuffers(0, window_w, window_h, DXGI_FORMAT_UNKNOWN, 0));

                // create new frame buffer
                frame_buffer = { d3d_dev.Get(), swap_chain.Get() };

                // resize event has been handled
                app_context.did_resize = false;
            }

            // update input state
            keyboard.Update();
            mouse.Update(window.Handle());

            // update editor state
            editor.Update(frame_stopwatch.ElapsedSec());

            // render scene to the back buffer
            renderer.Render(window_w, window_h, frame_buffer.BackBufferRTV(), editor.Scene());

            // render editor ui
            imgui_handle.BeginFrame();
            {
                editor.Render();

                ImGui::Begin("Frame Data");
                {
                    ImGui::Text("Delta Time: %.2f sec", frame_stopwatch.ElapsedSec());
                    ImGui::Text("Delta Time: %.2f msec", frame_stopwatch.ElapsedSec() * 1000.0f);
                }
                ImGui::End();
            }
            imgui_handle.EndFrame(frame_buffer.BackBufferRTV());

            // present
            qk_CheckHR(swap_chain->Present(1, 0)); // use vsync

            // update stopwatch to keep track of how much time the frame took
            frame_stopwatch.Mark();
        }
    }
}

// TODO: https://devblogs.microsoft.com/oldnewthing/20160125-00/?p=92922
int main()
{
    try
    {
        qk::Entry();
    }
    catch (const qk::Crash& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}

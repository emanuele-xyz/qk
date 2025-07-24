#include <qk_gui/PCH.h>
#include <qk_gui/D11.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")

namespace qk_gui::d11
{
    void SetupDXGIInforQueue()
    {
        #if defined(_DEBUG)
        wrl::ComPtr<IDXGIInfoQueue> queue{};
        qk_gui_CheckHR(DXGIGetDebugInterface1(0, IID_PPV_ARGS(queue.ReleaseAndGetAddressOf())));
        qk_gui_CheckHR(queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true));
        qk_gui_CheckHR(queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true));
        #endif
    }
    wrl::ComPtr<ID3D11Device> CreateDevice(D3D_FEATURE_LEVEL requested_level)
    {
        UINT flags{};
        #if defined(_DEBUG)
        flags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif
        D3D_FEATURE_LEVEL supported_level{};

        wrl::ComPtr<ID3D11Device> dev{};
        qk_gui_CheckHR(D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            &requested_level, 1,
            D3D11_SDK_VERSION,
            dev.ReleaseAndGetAddressOf(),
            &supported_level,
            nullptr
        ));

        qk_gui_Check(requested_level == supported_level);

        return dev;
    }
    void SetupD11InfoQueue([[maybe_unused]] ID3D11Device* dev)
    {
        #if defined(_DEBUG)
        wrl::ComPtr<ID3D11InfoQueue> queue{};
        qk_gui_CheckHR(dev->QueryInterface(queue.ReleaseAndGetAddressOf()));
        qk_gui_CheckHR(queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true));
        qk_gui_CheckHR(queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true));
        #endif
    }
    wrl::ComPtr<IDXGISwapChain1> CreateSwapChain(HWND window, ID3D11Device* dev)
    {
        wrl::ComPtr<IDXGIDevice> dxgi_dev{};
        qk_gui_CheckHR(dev->QueryInterface(dxgi_dev.ReleaseAndGetAddressOf()));
        wrl::ComPtr<IDXGIAdapter> dxgi_adapter{};
        qk_gui_CheckHR(dxgi_dev->GetAdapter(dxgi_adapter.ReleaseAndGetAddressOf()));

        DXGI_ADAPTER_DESC adapter_desc{};
        qk_gui_CheckHR(dxgi_adapter->GetDesc(&adapter_desc));

        std::println("adapter: {}", StrFromWStr(adapter_desc.Description));
        std::println("VRAM: {}", GetBytesStr(adapter_desc.DedicatedVideoMemory));
        std::println("dedicated RAM: {}", GetBytesStr(adapter_desc.DedicatedSystemMemory));
        std::println("shared RAM: {}", GetBytesStr(adapter_desc.SharedSystemMemory));

        wrl::ComPtr<IDXGIFactory2> dxgi_factory{};
        qk_gui_CheckHR(dxgi_adapter->GetParent(IID_PPV_ARGS(dxgi_factory.ReleaseAndGetAddressOf())));

        DXGI_SWAP_CHAIN_DESC1 desc{};
        desc.Width = 0; // use window width
        desc.Height = 0; // use window height
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo = false;
        desc.SampleDesc = { .Count = 1, .Quality = 0 };
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 2; // double buffering
        desc.Scaling = DXGI_SCALING_NONE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags = 0;

        wrl::ComPtr<IDXGISwapChain1> swap_chain{};
        qk_gui_CheckHR(dxgi_factory->CreateSwapChainForHwnd(dev, window, &desc, nullptr, nullptr, swap_chain.ReleaseAndGetAddressOf()));

        // disable Alt+Enter changing monitor resolution to match window size
        qk_gui_CheckHR(dxgi_factory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER));

        return swap_chain;
    }

    FrameBuffer::FrameBuffer(ID3D11Device* dev, IDXGISwapChain1* swap_chain)
        : FrameBuffer{}
    {
        // get swap chain back buffer handle
        qk_gui_CheckHR(swap_chain->GetBuffer(0, IID_PPV_ARGS(m_back_buffer.ReleaseAndGetAddressOf())));

        // create swap chain back buffer rtv
        qk_gui_CheckHR(dev->CreateRenderTargetView(m_back_buffer.Get(), nullptr, m_back_buffer_rtv.ReleaseAndGetAddressOf()));
    }
    FrameBuffer::FrameBuffer()
        : m_back_buffer{}
        , m_back_buffer_rtv{}
    {
    }
}

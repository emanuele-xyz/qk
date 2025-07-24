#pragma once

#include <qk_gui/W32.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

namespace qk_gui::d11
{
    void SetupDXGIInforQueue();
    wrl::ComPtr<ID3D11Device> CreateDevice(D3D_FEATURE_LEVEL requested_level);
    void SetupD11InfoQueue(ID3D11Device* dev);
    wrl::ComPtr<IDXGISwapChain1> CreateSwapChain(HWND window, ID3D11Device* dev);

    class FrameBuffer
    {
    public:
        FrameBuffer(ID3D11Device* dev, IDXGISwapChain1* swap_chain);
        FrameBuffer();
        ~FrameBuffer() = default;
        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&&) noexcept = default;
        FrameBuffer& operator=(const FrameBuffer&) = delete;
        FrameBuffer& operator=(FrameBuffer&&) noexcept = default;
    public:
        ID3D11RenderTargetView* BackBufferRTV() const noexcept { return m_back_buffer_rtv.Get(); }
    private:
        wrl::ComPtr<ID3D11Texture2D> m_back_buffer;
        wrl::ComPtr<ID3D11RenderTargetView> m_back_buffer_rtv;
    };
}

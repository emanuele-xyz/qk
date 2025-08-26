#pragma once

#include <qk/Commons.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <wrl/client.h> // for ComPtr
namespace wrl = Microsoft::WRL;

#include <d3d11.h>

#define qk_CheckHR(hr) qk_Check(SUCCEEDED(hr))

namespace qk::d11
{
    class SubresourceMap
    {
    public:
        SubresourceMap(ID3D11DeviceContext* ctx, ID3D11Resource* res, UINT subres_idx, D3D11_MAP map_type, UINT map_flags = 0);
        ~SubresourceMap();
        SubresourceMap(const SubresourceMap&) = delete;
        SubresourceMap(SubresourceMap&&) noexcept = delete;
        SubresourceMap& operator=(const SubresourceMap&) = delete;
        SubresourceMap& operator=(SubresourceMap&&) noexcept = delete;
    public:
        template <typename T>
        T* Data() const noexcept { return static_cast<T*>(Data()); }
    public:
        void* Data() const noexcept { return m_mapped_subres.pData; }
    private:
        ID3D11DeviceContext* m_ctx;
        ID3D11Resource* m_res;
        UINT m_subres_idx;
        D3D11_MAPPED_SUBRESOURCE m_mapped_subres;
    };

    class Buffer
    {
    public:
        Buffer();
        Buffer(ID3D11Device* dev, const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initial_data = nullptr);
        ~Buffer() = default;
        Buffer(const Buffer&) = delete;
        Buffer(Buffer&&) noexcept = default;
        Buffer& operator=(const Buffer&) = delete;
        Buffer& operator=(Buffer&&) noexcept = default;
    public:
        explicit operator bool() const noexcept { return m_buffer; }
    public:
        ID3D11Buffer* operator->() const noexcept { return m_buffer.Get(); }
    public:
        ID3D11Buffer* Get() const noexcept { return m_buffer.Get(); }
    public:
        SubresourceMap Map(D3D11_MAP map_type, UINT map_flags = 0) const { return SubresourceMap{ m_ctx.Get(), m_buffer.Get(), 0, map_type, map_flags}; }
    private:
        ID3D11Device* m_dev;
        wrl::ComPtr<ID3D11DeviceContext> m_ctx;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
    };

    class ConstantBuffer
    {
    public:
        template <typename T>
        ConstantBuffer(ID3D11Device* dev, UINT misc_flags = 0) : ConstantBuffer{ dev, sizeof(T), misc_flags } {}
    public:
        ConstantBuffer();
        ConstantBuffer(ID3D11Device* dev, UINT size_in_bytes, UINT misc_flags = 0);
        ~ConstantBuffer() = default;
        ConstantBuffer(const ConstantBuffer&) = delete;
        ConstantBuffer(ConstantBuffer&&) noexcept = default;
        ConstantBuffer& operator=(const ConstantBuffer&) = delete;
        ConstantBuffer& operator=(ConstantBuffer&&) noexcept = default;
    public:
        SubresourceMap Map(D3D11_MAP map_type, UINT map_flags = 0) const { return m_buffer.Map(map_type, map_flags); }
    private:
        ID3D11Device* m_dev;
        wrl::ComPtr<ID3D11DeviceContext> m_ctx;
        Buffer m_buffer;
    };

    class StructuredBuffer
    {
    public:
        template <typename T, unsigned int N>
        StructuredBuffer(ID3D11Device* dev, UINT misc_flags = 0) : StructuredBuffer{ dev, sizeof(T), sizeof(T) * N, misc_flags } {}
    public:
        StructuredBuffer(ID3D11Device* dev, UINT stride_in_bytes, UINT size_in_bytes = 0, UINT misc_flags = 0);
        ~StructuredBuffer() = default;
        StructuredBuffer(const StructuredBuffer&) = delete;
        StructuredBuffer(StructuredBuffer&&) noexcept = default;
        StructuredBuffer& operator=(const StructuredBuffer&) = delete;
        StructuredBuffer& operator=(StructuredBuffer&&) noexcept = default;
    public:
        explicit operator bool() const noexcept { return m_buffer && m_srv; }
    public:
        ID3D11ShaderResourceView* SRV() const noexcept { return m_srv.Get(); }
    public:
        SubresourceMap Map(D3D11_MAP map_type, UINT map_flags = 0) const { return m_buffer.Map(map_type, map_flags); }
        void Resize(UINT size_in_bytes);
    private:
        ID3D11Device* m_dev;
        wrl::ComPtr<ID3D11DeviceContext> m_ctx;
        D3D11_BUFFER_DESC m_buffer_desc;
        Buffer m_buffer;
        wrl::ComPtr<ID3D11ShaderResourceView> m_srv;
    };

    class DepthStencilBuffer
    {
    public:
        constexpr static UINT DEFAULT_W{ 8 };
        constexpr static UINT DEFAULT_H{ 8 };
    public:
        DepthStencilBuffer(ID3D11Device* dev, DXGI_FORMAT format);
        DepthStencilBuffer(ID3D11Device* dev, DXGI_FORMAT format, UINT w, UINT h);
        ~DepthStencilBuffer() = default;
        DepthStencilBuffer(const DepthStencilBuffer&) = delete;
        DepthStencilBuffer(DepthStencilBuffer&) noexcept = default;
        DepthStencilBuffer& operator=(const DepthStencilBuffer&) = delete;
        DepthStencilBuffer& operator=(DepthStencilBuffer&) noexcept = default;
    public:
        explicit operator bool() const noexcept { return m_texture && m_dsv; }
    public:
        ID3D11DepthStencilView* DSV() const noexcept { return m_dsv.Get(); }
    public:
        void Resize(UINT w, UINT h);
    private:
        ID3D11Device* m_dev;
        wrl::ComPtr<ID3D11Texture2D> m_texture;
        wrl::ComPtr<ID3D11DepthStencilView> m_dsv;
    };
}

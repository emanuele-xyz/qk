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
        SubresourceMap(ID3D11DeviceContext* ctx, ID3D11Resource* res, UINT subres_idx, D3D11_MAP map_type, UINT map_flags);
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
        ID3D11Buffer* Get() const { return m_buffer.Get(); }
    public:
        SubresourceMap Map(D3D11_MAP map_type, UINT map_flags) const;
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
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
        SubresourceMap Map(D3D11_MAP map_type, UINT map_flags) const { return m_buffer.Map(map_type, map_flags); }
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        Buffer m_buffer;
    };

    class StructuredBuffer
    {
    public:
        template <typename T, unsigned int N>
        StructuredBuffer(ID3D11Device* dev, UINT misc_flags = 0) : StructuredBuffer{ dev, sizeof(T) * N, sizeof(T), misc_flags } {}
    public:
        StructuredBuffer();
        StructuredBuffer(ID3D11Device* dev, UINT size_in_bytes, UINT stride_in_bytes, UINT misc_flags = 0);
        ~StructuredBuffer() = default;
        StructuredBuffer(const StructuredBuffer&) = delete;
        StructuredBuffer(StructuredBuffer&&) noexcept = default;
        StructuredBuffer& operator=(const StructuredBuffer&) = delete;
        StructuredBuffer& operator=(StructuredBuffer&&) noexcept = default;
    public:
        SubresourceMap Map(D3D11_MAP map_type, UINT map_flags) const { return m_buffer.Map(map_type, map_flags); }
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        Buffer m_buffer;
        wrl::ComPtr<ID3D11ShaderResourceView> m_srv;
    };
}

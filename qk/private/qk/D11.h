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
        void* Data() { return m_mapped_subres.pData; }
    private:
        ID3D11DeviceContext* m_ctx;
        ID3D11Resource* m_res;
        UINT m_subres_idx;
        D3D11_MAPPED_SUBRESOURCE m_mapped_subres;
    };

    // TODO: create helper classes for constant buffers and structured buffers
}

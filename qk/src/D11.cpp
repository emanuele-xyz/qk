#include <qk/PCH.h>
#include <qk/D11.h>

namespace qk::d11
{
    SubresourceMap::SubresourceMap(ID3D11DeviceContext* ctx, ID3D11Resource* res, UINT subres_idx, D3D11_MAP map_type, UINT map_flags)
        : m_ctx{ ctx }
        , m_res{ res }
        , m_subres_idx{ subres_idx }
        , m_mapped_subres{}
    {
        qk_CheckHR(m_ctx->Map(m_res, m_subres_idx, map_type, map_flags, &m_mapped_subres));
    }
    SubresourceMap::~SubresourceMap()
    {
        m_ctx->Unmap(m_res, m_subres_idx);
    }
}

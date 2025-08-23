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

    Buffer::Buffer()
        : m_dev{}
        , m_ctx{}
        , m_buffer{}
    {
    }
    Buffer::Buffer(ID3D11Device* dev, const D3D11_BUFFER_DESC* desc, const D3D11_SUBRESOURCE_DATA* initial_data)
        : m_dev{ dev }
        , m_ctx{}
        , m_buffer{}
    {
        // fetch context
        m_dev->GetImmediateContext(&m_ctx);
        // create buffer
        qk_CheckHR(m_dev->CreateBuffer(desc, initial_data, m_buffer.ReleaseAndGetAddressOf()));
    }
    SubresourceMap Buffer::Map(D3D11_MAP map_type, UINT map_flags) const
    {
        return SubresourceMap{ m_ctx, m_buffer.Get(), 0, map_type, map_flags };
    }

    ConstantBuffer::ConstantBuffer()
        : m_dev{}
        , m_ctx{}
        , m_buffer{}
    {
    }
    ConstantBuffer::ConstantBuffer(ID3D11Device* dev, UINT size_in_bytes, UINT misc_flags)
        : m_dev{ dev }
        , m_ctx{}
        , m_buffer{}
    {
        m_dev->GetImmediateContext(&m_ctx);

        // create buffer
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = size_in_bytes;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = misc_flags;
            desc.StructureByteStride = 0;
            m_buffer = Buffer{ m_dev, &desc };
        }
    }

    StructuredBuffer::StructuredBuffer()
        : m_dev{}
        , m_ctx{}
        , m_buffer{}
        , m_srv{}
    {
    }
    StructuredBuffer::StructuredBuffer(ID3D11Device* dev, UINT size_in_bytes, UINT stride_in_bytes, UINT misc_flags)
        : m_dev{ dev }
        , m_ctx{}
        , m_buffer{}
        , m_srv{}
    {
        // fetch context
        m_dev->GetImmediateContext(&m_ctx);
        // create buffer
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = size_in_bytes;
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED | misc_flags;
            desc.StructureByteStride = stride_in_bytes;
            m_buffer = Buffer{ m_dev, &desc };
        }
        // create shader resource view
        qk_CheckHR(m_dev->CreateShaderResourceView(m_buffer.Get(), nullptr, m_srv.ReleaseAndGetAddressOf()));
    }
}

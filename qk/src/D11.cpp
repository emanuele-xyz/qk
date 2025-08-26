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
        m_dev->GetImmediateContext(m_ctx.ReleaseAndGetAddressOf());
        // create buffer
        qk_CheckHR(m_dev->CreateBuffer(desc, initial_data, m_buffer.ReleaseAndGetAddressOf()));
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
        // fetch context
        m_dev->GetImmediateContext(m_ctx.ReleaseAndGetAddressOf());
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

    StructuredBuffer::StructuredBuffer(ID3D11Device* dev, UINT stride_in_bytes, UINT size_in_bytes, UINT misc_flags)
        : m_dev{ dev }
        , m_ctx{}
        , m_buffer_desc{}
        , m_buffer{}
        , m_srv{}
    {
        m_buffer_desc.ByteWidth = size_in_bytes;
        m_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        m_buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        m_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        m_buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED | misc_flags;
        m_buffer_desc.StructureByteStride = stride_in_bytes;

        // fetch context
        m_dev->GetImmediateContext(m_ctx.ReleaseAndGetAddressOf());
        if (m_buffer_desc.ByteWidth > 0)
        {
            // create buffer
            m_buffer = Buffer{ m_dev, &m_buffer_desc };
            // create shader resource view
            qk_CheckHR(m_dev->CreateShaderResourceView(m_buffer.Get(), nullptr, m_srv.ReleaseAndGetAddressOf()));
        }
    }
    void StructuredBuffer::Resize(UINT size_in_bytes)
    {
        // if the buffer's size is different from the required size, resize it
        if (size_in_bytes > 0 && m_buffer_desc.ByteWidth != size_in_bytes)
        {
            m_buffer_desc.ByteWidth = size_in_bytes;

            // create buffer
            m_buffer = Buffer{ m_dev, &m_buffer_desc };
            // create srv
            qk_CheckHR(m_dev->CreateShaderResourceView(m_buffer.Get(), nullptr, m_srv.ReleaseAndGetAddressOf()));
        }
    }

    DepthStencilBuffer::DepthStencilBuffer(ID3D11Device* dev, DXGI_FORMAT format)
        : DepthStencilBuffer{ dev, format, 0, 0 }
    {
    }
    DepthStencilBuffer::DepthStencilBuffer(ID3D11Device* dev, DXGI_FORMAT format, UINT w, UINT h)
        : m_dev{ dev }
        , m_texture_desc{}
        , m_texture{}
        , m_dsv{}
    {
        m_texture_desc.Width = w;
        m_texture_desc.Height = h;
        m_texture_desc.MipLevels = 1;
        m_texture_desc.ArraySize = 1;
        m_texture_desc.Format = format;
        m_texture_desc.SampleDesc = { .Count = 1, .Quality = 0 };
        m_texture_desc.Usage = D3D11_USAGE_DEFAULT;
        m_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        m_texture_desc.CPUAccessFlags = 0;
        m_texture_desc.MiscFlags = 0;

        if (m_texture_desc.Width > 0 && m_texture_desc.Height > 0)
        {
            // create texture
            qk_CheckHR(m_dev->CreateTexture2D(&m_texture_desc, nullptr, m_texture.ReleaseAndGetAddressOf()));
            // create depth stencil view
            qk_CheckHR(m_dev->CreateDepthStencilView(m_texture.Get(), nullptr, m_dsv.ReleaseAndGetAddressOf()));
        }
    }
    void DepthStencilBuffer::Resize(UINT w, UINT h)
    {
        // if the buffer resoultion is different from the required one, resize it
        if (w > 0 && h > 0 && (m_texture_desc.Width != w || m_texture_desc.Height != h))
        {
            m_texture_desc.Width = static_cast<UINT>(w);
            m_texture_desc.Height = static_cast<UINT>(h);

            // create depth stencil buffer
            qk_CheckHR(m_dev->CreateTexture2D(&m_texture_desc, nullptr, m_texture.ReleaseAndGetAddressOf()));
            // create depth stencil view
            qk_CheckHR(m_dev->CreateDepthStencilView(m_texture.Get(), nullptr, m_dsv.ReleaseAndGetAddressOf()));
        }
    }
}

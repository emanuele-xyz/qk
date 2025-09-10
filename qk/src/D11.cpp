#include <qk/PCH.h>
#include <qk/D11.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")

#include <DirectXTex.h>

namespace qk::d11
{
    void SetupDXGIInforQueue()
    {
        #if defined(_DEBUG)
        wrl::ComPtr<IDXGIInfoQueue> queue{};
        qk_CheckHR(DXGIGetDebugInterface1(0, IID_PPV_ARGS(queue.ReleaseAndGetAddressOf())));
        qk_CheckHR(queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true));
        qk_CheckHR(queue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true));
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
        qk_CheckHR(D3D11CreateDevice(
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

        qk_Check(requested_level == supported_level);

        return dev;
    }
    void SetupD11InfoQueue([[maybe_unused]] ID3D11Device* dev)
    {
        #if defined(_DEBUG)
        wrl::ComPtr<ID3D11InfoQueue> queue{};
        qk_CheckHR(dev->QueryInterface(queue.ReleaseAndGetAddressOf()));
        qk_CheckHR(queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true));
        qk_CheckHR(queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true));
        #endif
    }
    wrl::ComPtr<IDXGISwapChain1> CreateSwapChain(HWND window, ID3D11Device* dev)
    {
        wrl::ComPtr<IDXGIDevice> dxgi_dev{};
        qk_CheckHR(dev->QueryInterface(dxgi_dev.ReleaseAndGetAddressOf()));
        wrl::ComPtr<IDXGIAdapter> dxgi_adapter{};
        qk_CheckHR(dxgi_dev->GetAdapter(dxgi_adapter.ReleaseAndGetAddressOf()));

        DXGI_ADAPTER_DESC adapter_desc{};
        qk_CheckHR(dxgi_adapter->GetDesc(&adapter_desc));

        std::println("adapter: {}", StrFromWStr(adapter_desc.Description));
        std::println("VRAM: {}", GetBytesStr(adapter_desc.DedicatedVideoMemory));
        std::println("dedicated RAM: {}", GetBytesStr(adapter_desc.DedicatedSystemMemory));
        std::println("shared RAM: {}", GetBytesStr(adapter_desc.SharedSystemMemory));

        wrl::ComPtr<IDXGIFactory2> dxgi_factory{};
        qk_CheckHR(dxgi_adapter->GetParent(IID_PPV_ARGS(dxgi_factory.ReleaseAndGetAddressOf())));

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
        qk_CheckHR(dxgi_factory->CreateSwapChainForHwnd(dev, window, &desc, nullptr, nullptr, swap_chain.ReleaseAndGetAddressOf()));

        // disable Alt+Enter changing monitor resolution to match window size
        qk_CheckHR(dxgi_factory->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER));

        return swap_chain;
    }

    FrameBuffer::FrameBuffer(ID3D11Device* dev, IDXGISwapChain1* swap_chain)
        : FrameBuffer{}
    {
        // get swap chain back buffer handle
        qk_CheckHR(swap_chain->GetBuffer(0, IID_PPV_ARGS(m_back_buffer.ReleaseAndGetAddressOf())));

        D3D11_TEXTURE2D_DESC buffer_desc{};
        m_back_buffer->GetDesc(&buffer_desc);

        D3D11_RENDER_TARGET_VIEW_DESC rtv_desc{};
        rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        // create linear swap chain back buffer rtv
        rtv_desc.Format = DirectX::MakeLinear(buffer_desc.Format);
        qk_CheckHR(dev->CreateRenderTargetView(m_back_buffer.Get(), &rtv_desc, m_back_buffer_rtv_linear.ReleaseAndGetAddressOf()));

        // create srgb swap chain back buffer rtv
        rtv_desc.Format = DirectX::MakeSRGB(buffer_desc.Format);
        qk_CheckHR(dev->CreateRenderTargetView(m_back_buffer.Get(), &rtv_desc, m_back_buffer_rtv_srgb.ReleaseAndGetAddressOf()));
    }
    FrameBuffer::FrameBuffer()
        : m_back_buffer{}
        , m_back_buffer_rtv_linear{}
        , m_back_buffer_rtv_srgb{}
    {
    }

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

    Buffer2D::Buffer2D(ID3D11Device* dev, DXGI_FORMAT format, UINT bind_flags, UINT w, UINT h, UINT cpu_access_flags, UINT misc_flags)
        : m_dev{ dev }
        , m_texture_desc{}
        , m_texture{}
        , m_rtv{}
        , m_srv{}
        , m_dsv{}
    {
        m_texture_desc.Width = w > 0 ? w : 0;
        m_texture_desc.Height = h > 0 ? h : 0;
        m_texture_desc.MipLevels = 1;
        m_texture_desc.ArraySize = 1;
        m_texture_desc.Format = format;
        m_texture_desc.SampleDesc = { .Count = 1, .Quality = 0 };
        m_texture_desc.Usage = D3D11_USAGE_DEFAULT;
        m_texture_desc.BindFlags = bind_flags;
        m_texture_desc.CPUAccessFlags = cpu_access_flags;
        m_texture_desc.MiscFlags = misc_flags;
        Init();
    }
    void Buffer2D::Resize(UINT w, UINT h)
    {
        // if the texture's resolution is different from the required one, resize it
        if (m_texture_desc.Width != w || m_texture_desc.Height != h)
        {
            m_texture_desc.Width = w;
            m_texture_desc.Height = h;
            Init();
        }
    }
    void Buffer2D::Free()
    {
        m_texture.Reset();
        m_rtv.Reset();
        m_srv.Reset();
        m_dsv.Reset();

        // since buffer is empty, its resolution should be 0x0
        m_texture_desc.Width = 0;
        m_texture_desc.Height = 0;
    }
    void Buffer2D::Init()
    {
        // create texture, if resolution is valid
        if (m_texture_desc.Width > 0 && m_texture_desc.Height > 0)
        {
            qk_CheckHR(m_dev->CreateTexture2D(&m_texture_desc, nullptr, m_texture.ReleaseAndGetAddressOf()));

            // create rtv, if necessary
            if (m_texture_desc.BindFlags & D3D11_BIND_RENDER_TARGET)
            {
                qk_CheckHR(m_dev->CreateRenderTargetView(m_texture.Get(), nullptr, m_rtv.ReleaseAndGetAddressOf()));
            }
            // create srv, if necessary
            if (m_texture_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
            {
                qk_CheckHR(m_dev->CreateShaderResourceView(m_texture.Get(), nullptr, m_srv.ReleaseAndGetAddressOf()));
            }
            // create dsv, if necessary
            if (m_texture_desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
            {
                qk_CheckHR(m_dev->CreateDepthStencilView(m_texture.Get(), nullptr, m_dsv.ReleaseAndGetAddressOf()));
            }
        }
    }

    SamplerCache::SamplerCache(ID3D11Device* dev)
        : m_dev{ dev }
        , m_descs{}
        , m_states{}
    {
    }
    ID3D11SamplerState* SamplerCache::Get(const D3D11_SAMPLER_DESC& request)
    {
        // function used to check for equality between the given sampler description and the requested one
        auto cmp_fn
        {
            [request](const D3D11_SAMPLER_DESC& desc)
            {
                return desc.Filter == request.Filter &&
                desc.AddressU == request.AddressU &&
                desc.AddressV == request.AddressV &&
                desc.AddressW == request.AddressW &&
                desc.MipLODBias == request.MipLODBias &&
                desc.MaxAnisotropy == request.MaxAnisotropy &&
                desc.ComparisonFunc == request.ComparisonFunc &&
                desc.BorderColor[0] == request.BorderColor[0] &&
                desc.BorderColor[1] == request.BorderColor[1] &&
                desc.BorderColor[2] == request.BorderColor[2] &&
                desc.BorderColor[3] == request.BorderColor[3] &&
                desc.MinLOD == request.MinLOD &&
                desc.MaxLOD == request.MaxLOD;
            }
        };

        // do we already have a sampler with the same description?
        auto it{ std::find_if(m_descs.begin(), m_descs.end(), cmp_fn) };
        if (it != m_descs.end()) // yes
        {
            // return already created sampler
            return m_states[std::distance(m_descs.begin(), it)].Get();
        }
        else // no
        {
            // add requested desc to the list, create new sampler using the requested desc, and return it
            m_descs.emplace_back(request);
            wrl::ComPtr<ID3D11SamplerState>& ss{ m_states.emplace_back() };
            m_dev->CreateSamplerState(&request, ss.ReleaseAndGetAddressOf());
            return ss.Get();
        }
    }
}

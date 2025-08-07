#include <qk/PCH.h>
#include <qk/Renderer.h>
#include <qk/Mesh.h>
#include <qk/Texture.h>
#include <qk/D11.h>

#include <SimpleMath.h>
using Matrix = DirectX::SimpleMath::Matrix;
using Vector3 = DirectX::SimpleMath::Vector3;
namespace dx = DirectX; // for DirectX namespace included by DirectXMath (included by SimpleMath)

#define matrix Matrix
#define float3 Vector3
#include <qk/hlsl/OpaquePassConstants.h>
#undef float3
#undef matrix

#include <qk/hlsl/OpaquePassVS.h>
#include <qk/hlsl/OpaquePassPS.h>

namespace qk
{
    constexpr DXGI_FORMAT MESH_INDEX_FORMAT{ DXGI_FORMAT_R32_UINT };
    static_assert(sizeof(MeshIndex) == 4);

    class Mesh
    {
    public:
        static Mesh Cube(ID3D11Device* dev);
        static Mesh Quad(ID3D11Device* dev);
    public:
        Mesh(ID3D11Device* dev, std::span<const MeshVertex> vertices, std::span<const MeshIndex> indices);
        ~Mesh() = default;
        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) noexcept = default;
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) noexcept = default;
    public:
        ID3D11Buffer* Vertices() const noexcept { return m_vertices.Get(); }
        ID3D11Buffer* Indices() const noexcept { return m_indices.Get(); }
        UINT VertexCount() const noexcept { return m_vertex_count; }
        UINT IndexCount() const noexcept { return m_index_count; }
    private:
        wrl::ComPtr<ID3D11Buffer> m_vertices;
        wrl::ComPtr<ID3D11Buffer> m_indices;
        UINT m_vertex_count;
        UINT m_index_count;
    };
    Mesh Mesh::Cube(ID3D11Device* dev)
    {
        MeshVertex vertices[]
        {
            // front face (Z+)
            { { -0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            { { +0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { +0.5f, +0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -0.5f, +0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },

            // back face (Z-)
            { { +0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
            { { -0.5f, +0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
            { { +0.5f, +0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },

            // left face (X-)
            { { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
            { { -0.5f, -0.5f, +0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            { { -0.5f, +0.5f, +0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -0.5f, +0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },

            // right face (X+)
            { { +0.5f, -0.5f, +0.5f }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
            { { +0.5f, -0.5f, -0.5f }, { +1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
            { { +0.5f, +0.5f, -0.5f }, { +1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
            { { +0.5f, +0.5f, +0.5f }, { +1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },

            // top face (Y+)
            { { -0.5f, +0.5f, +0.5f }, { 0.0f, +1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { +0.5f, +0.5f, +0.5f }, { 0.0f, +1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { +0.5f, +0.5f, -0.5f }, { 0.0f, +1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -0.5f, +0.5f, -0.5f }, { 0.0f, +1.0f, 0.0f }, { 0.0f, 0.0f } },

            // bottom face (Y-)
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { +0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
            { { +0.5f, -0.5f, +0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
            { { -0.5f, -0.5f, +0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
        };

        MeshIndex indices[]
        {
            // front
            0, 1, 2,
            0, 2, 3,

            // back
            4, 5, 6,
            4, 6, 7,

            // left
            8, 9,10,
            8,10,11,

            // right
            12,13,14,
            12,14,15,

            // top 
            16,17,18,
            16,18,19,

            // bottom
            20,21,22,
            20,22,23
        };

        return Mesh{ dev, vertices, indices };
    }
    Mesh Mesh::Quad(ID3D11Device* dev)
    {
        MeshVertex vertices[]
        {
            { { -0.5f, -0.5f, +0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            { { +0.5f, -0.5f, +0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { +0.5f, +0.5f, +0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -0.5f, +0.5f, +0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        };

        MeshIndex indices[]
        {
            0, 1, 2,
            0, 2, 3,
        };

        return Mesh{ dev, vertices, indices };
    }
    Mesh::Mesh(ID3D11Device* dev, std::span<const MeshVertex> vertices, std::span<const MeshIndex> indices)
        : m_vertices{}
        , m_indices{}
        , m_vertex_count{ static_cast<UINT>(vertices.size()) }
        , m_index_count{ static_cast<UINT>(indices.size()) }

    {
        // upload vertices to the GPU
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = static_cast<UINT>(vertices.size_bytes());
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            D3D11_SUBRESOURCE_DATA data{};
            data.pSysMem = vertices.data();
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            qk_CheckHR(dev->CreateBuffer(&desc, &data, m_vertices.ReleaseAndGetAddressOf()));
        }

        // upload indices to the GPU
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = static_cast<UINT>(indices.size_bytes());
            desc.Usage = D3D11_USAGE_IMMUTABLE;
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            D3D11_SUBRESOURCE_DATA data{};
            data.pSysMem = indices.data();
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            qk_CheckHR(dev->CreateBuffer(&desc, &data, m_indices.ReleaseAndGetAddressOf()));
        }
    }

    class Texture
    {
    public:
        static Texture AlbedoBlack(ID3D11Device* dev);
        static Texture AlbedoWhite(ID3D11Device* dev);
        static Texture AlbedoPink(ID3D11Device* dev);
        static Texture AlbedoChecker(ID3D11Device* dev);
    private:
        constexpr static int ALBEDO_DIM{ 64 };
    public:
        Texture(ID3D11Device* dev, int w, int h, int channels, const void* data);
        ~Texture() = default;
        Texture(const Texture&) = delete;
        Texture(Texture&&) noexcept = default;
        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) noexcept = default;
    public:
        ID3D11ShaderResourceView* SRV() const noexcept { return m_srv.Get(); }
    private:
        wrl::ComPtr<ID3D11Texture2D> m_texture;
        wrl::ComPtr<ID3D11ShaderResourceView> m_srv;
    };
    Texture Texture::AlbedoBlack(ID3D11Device* dev)
    {
        constexpr int CHANNELS{ 4 };
        constexpr int DIM{ ALBEDO_DIM };

        struct Pixel
        {
            std::uint8_t r, g, b, a;
        };

        auto pixels{ std::make_unique<Pixel[]>(DIM * DIM) };

        for (int i{}; i < DIM * DIM; i++)
        {
            pixels[i] =
            {
                .r = 0,
                .g = 0,
                .b = 0,
                .a = std::numeric_limits<std::uint8_t>::max(),
            };
        }

        return Texture{ dev, DIM, DIM, CHANNELS, pixels.get() };
    }
    Texture Texture::AlbedoWhite(ID3D11Device* dev)
    {
        constexpr int CHANNELS{ 4 };
        constexpr int DIM{ ALBEDO_DIM };

        struct Pixel
        {
            std::uint8_t r, g, b, a;
        };

        auto pixels{ std::make_unique<Pixel[]>(DIM * DIM) };

        for (int i{}; i < DIM * DIM; i++)
        {
            pixels[i] =
            {
                .r = std::numeric_limits<std::uint8_t>::max(),
                .g = std::numeric_limits<std::uint8_t>::max(),
                .b = std::numeric_limits<std::uint8_t>::max(),
                .a = std::numeric_limits<std::uint8_t>::max(),
            };
        }

        return Texture{ dev, DIM, DIM, CHANNELS, pixels.get() };
    }
    Texture Texture::AlbedoPink(ID3D11Device* dev)
    {
        constexpr int CHANNELS{ 4 };
        constexpr int DIM{ ALBEDO_DIM };

        struct Pixel
        {
            std::uint8_t r, g, b, a;
        };

        auto pixels{ std::make_unique<Pixel[]>(DIM * DIM) };

        for (int i{}; i < DIM * DIM; i++)
        {
            pixels[i] =
            {
                .r = std::numeric_limits<std::uint8_t>::max(),
                .g = 0,
                .b = std::numeric_limits<std::uint8_t>::max(),
                .a = std::numeric_limits<std::uint8_t>::max(),
            };
        }

        return Texture{ dev, DIM, DIM, CHANNELS, pixels.get() };
    }
    Texture Texture::AlbedoChecker(ID3D11Device* dev)
    {
        constexpr int CHANNELS{ 4 };
        constexpr int DIM{ ALBEDO_DIM };
        constexpr int HALF_DIM{ DIM / 2 };

        struct Pixel
        {
            std::uint8_t r, g, b, a;
        };

        auto pixels{ std::make_unique<Pixel[]>(DIM * DIM) };

        for (int row{}; row < DIM; row++)
        {
            for (int col{}; col < DIM; col++)
            {
                bool is_less_half_on_row{ row < HALF_DIM };
                bool is_less_half_on_col{ col < HALF_DIM };
                int idx{ row * DIM + col };
                if (is_less_half_on_row != is_less_half_on_col) // logical xor
                {
                    pixels[idx] =
                    {
                        .r = 0,
                        .g = 0,
                        .b = 0,
                        .a = std::numeric_limits<std::uint8_t>::max(),
                    };
                }
                else
                {
                    pixels[idx] =
                    {
                        .r = std::numeric_limits<std::uint8_t>::max(),
                        .g = std::numeric_limits<std::uint8_t>::max(),
                        .b = std::numeric_limits<std::uint8_t>::max(),
                        .a = std::numeric_limits<std::uint8_t>::max(),
                    };
                }
            }
        }

        return Texture{ dev, DIM, DIM, CHANNELS, pixels.get() };
    }
    Texture::Texture(ID3D11Device* dev, int w, int h, int channels, const void* data)
        : m_texture{}
        , m_srv{}
    {
        DXGI_FORMAT format{};
        switch (channels)
        {
        case 1: { format = DXGI_FORMAT_R8_UNORM; } break;
        case 2: { format = DXGI_FORMAT_R8G8_UNORM; } break;
        case 4: { format = DXGI_FORMAT_R8G8B8A8_UNORM; } break;
        default: { qk_Unreachable(); } break;
        }

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = static_cast<UINT>(w);
        desc.Height = static_cast<UINT>(h);
        desc.MipLevels = 1; // TODO: generate mip chain
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc = { .Count = 1, .Quality = 0 };
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA initial_data{};
        initial_data.pSysMem = data;
        initial_data.SysMemPitch = w * channels;
        qk_CheckHR(dev->CreateTexture2D(&desc, &initial_data, m_texture.ReleaseAndGetAddressOf()));
        qk_CheckHR(dev->CreateShaderResourceView(m_texture.Get(), nullptr, m_srv.ReleaseAndGetAddressOf()));
    }

    class OpaquePass
    {
    public:
        OpaquePass(ID3D11Device* dev, ID3D11DeviceContext* ctx, const std::vector<Mesh>& meshes, const std::vector<Texture>& textures);
        ~OpaquePass() = default;
        OpaquePass(const OpaquePass&) = delete;
        OpaquePass(OpaquePass&&) noexcept = delete;
        OpaquePass& operator=(const OpaquePass&) = delete;
        OpaquePass& operator=(OpaquePass&&) noexcept = delete;
    public:
        void Render(int w, int h, ID3D11RenderTargetView* rtv, const Scene& scene);
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        const std::vector<Mesh>& m_meshes;
        const std::vector<Texture>& m_textures;
        D3D11_VIEWPORT m_viewport;
        wrl::ComPtr<ID3D11VertexShader> m_vs;
        wrl::ComPtr<ID3D11PixelShader> m_ps;
        wrl::ComPtr<ID3D11InputLayout> m_il;
        wrl::ComPtr<ID3D11RasterizerState> m_rs;
        wrl::ComPtr<ID3D11SamplerState> m_texture_ss;
        wrl::ComPtr<ID3D11Buffer> m_cb_scene;
        wrl::ComPtr<ID3D11Buffer> m_cb_object;
        wrl::ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;
        wrl::ComPtr<ID3D11DepthStencilView> m_dsv;
    };
    OpaquePass::OpaquePass(ID3D11Device* dev, ID3D11DeviceContext* ctx, const std::vector<Mesh>& meshes, const std::vector<Texture>& textures)
        : m_dev{ dev }
        , m_ctx{ ctx }
        , m_meshes{ meshes }
        , m_textures{ textures }
        , m_viewport{ .TopLeftX = 0.0f, .TopLeftY = 0.0f, .MinDepth = 0.0f, .MaxDepth = 1.0f }
        , m_vs{}
        , m_ps{}
        , m_il{}
        , m_rs{}
        , m_cb_scene{}
        , m_cb_object{}
        , m_depth_stencil_buffer{}
        , m_dsv{}
    {
        // vertex shader
        qk_CheckHR(m_dev->CreateVertexShader(OpaquePassVS_bytes, sizeof(OpaquePassVS_bytes), nullptr, m_vs.ReleaseAndGetAddressOf()));

        // pixel shader
        qk_CheckHR(m_dev->CreatePixelShader(OpaquePassPS_bytes, sizeof(OpaquePassPS_bytes), nullptr, m_ps.ReleaseAndGetAddressOf()));

        // input layout
        {
            D3D11_INPUT_ELEMENT_DESC desc[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            qk_CheckHR(m_dev->CreateInputLayout(desc, std::size(desc), OpaquePassVS_bytes, sizeof(OpaquePassVS_bytes), m_il.ReleaseAndGetAddressOf()));
        }

        // rasterizer state
        {
            D3D11_RASTERIZER_DESC desc{};
            desc.FillMode = D3D11_FILL_SOLID;
            desc.CullMode = D3D11_CULL_BACK;
            desc.FrontCounterClockwise = true;
            desc.DepthBias = 0;
            desc.DepthBiasClamp = 0.0f;
            desc.SlopeScaledDepthBias = 0.0f;
            desc.DepthClipEnable = true;
            desc.ScissorEnable = false;
            desc.MultisampleEnable = false;
            desc.AntialiasedLineEnable = false;
            qk_CheckHR(m_dev->CreateRasterizerState(&desc, m_rs.ReleaseAndGetAddressOf()));
        }

        // sampler state
        {
            D3D11_SAMPLER_DESC desc{};
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            //desc.MaxAnisotropy = ;
            desc.BorderColor[0] = 0.0f;
            desc.BorderColor[1] = 0.0f;
            desc.BorderColor[2] = 0.0f;
            desc.BorderColor[3] = 0.0f;
            desc.MinLOD = 0;
            desc.MaxLOD = D3D11_FLOAT32_MAX;
            qk_CheckHR(m_dev->CreateSamplerState(&desc, m_texture_ss.ReleaseAndGetAddressOf()));
        }

        // scene constant buffer
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(OpaquePassSceneConstants);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            qk_CheckHR(m_dev->CreateBuffer(&desc, nullptr, m_cb_scene.ReleaseAndGetAddressOf()));
        }

        // object constant buffer
        {
            D3D11_BUFFER_DESC desc{};
            desc.ByteWidth = sizeof(OpaquePassObjectConstants);
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            desc.MiscFlags = 0;
            desc.StructureByteStride = 0;
            qk_CheckHR(m_dev->CreateBuffer(&desc, nullptr, m_cb_object.ReleaseAndGetAddressOf()));
        }
    }
    void OpaquePass::Render(int w, int h, ID3D11RenderTargetView* rtv, const Scene& scene)
    {
        // resize depth buffer if w or h changed
        {
            D3D11_TEXTURE2D_DESC desc{};
            desc.Width = 0;
            desc.Height = 0;
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_D32_FLOAT;
            desc.SampleDesc = { .Count = 1, .Quality = 0 };
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            desc.CPUAccessFlags = 0;
            desc.MiscFlags = 0;

            if (m_depth_stencil_buffer)
            {
                m_depth_stencil_buffer->GetDesc(&desc);
            }

            if (desc.Width != static_cast<UINT>(w) || desc.Height != static_cast<UINT>(h))
            {
                desc.Width = static_cast<UINT>(w);
                desc.Height = static_cast<UINT>(h);

                // depth stencil buffer
                qk_CheckHR(m_dev->CreateTexture2D(&desc, nullptr, m_depth_stencil_buffer.ReleaseAndGetAddressOf()));
                // depth stencil view
                qk_CheckHR(m_dev->CreateDepthStencilView(m_depth_stencil_buffer.Get(), nullptr, m_dsv.ReleaseAndGetAddressOf()));
            }
        }

        // clear dsv
        m_ctx->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

        // set new viewport data
        m_viewport.Width = static_cast<float>(w);
        m_viewport.Height = static_cast<float>(h);

        // prepare pipeline for drawing
        {
            ID3D11Buffer* cbufs[]{ m_cb_scene.Get(), m_cb_object.Get() };

            m_ctx->ClearState();
            m_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            m_ctx->IASetInputLayout(m_il.Get());
            m_ctx->VSSetShader(m_vs.Get(), nullptr, 0);
            m_ctx->VSSetConstantBuffers(0, std::size(cbufs), cbufs);
            m_ctx->PSSetShader(m_ps.Get(), nullptr, 0);
            m_ctx->PSSetConstantBuffers(0, std::size(cbufs), cbufs);
            m_ctx->RSSetState(m_rs.Get());
            m_ctx->RSSetViewports(1, &m_viewport);
            m_ctx->OMSetRenderTargets(1, &rtv, m_dsv.Get());
        }

        // build camera view and projection matriaces, then upload them to the GPU
        {
            float aspect{ m_viewport.Width / m_viewport.Height };
            float fov_rad{ DirectX::XMConvertToRadians(scene.camera.fov_deg) };
            Matrix view{ Matrix::CreateLookAt(Vector3{ scene.camera.eye.elems }, Vector3{ scene.camera.target.elems }, { 0.0f, 1.0f, 0.0f }) };
            Matrix projection{ Matrix::CreatePerspectiveFieldOfView(fov_rad, aspect, scene.camera.near_plane, scene.camera.far_plane) };

            d11::SubresourceMap map{ m_ctx, m_cb_scene.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
            auto constants{ static_cast<OpaquePassSceneConstants*>(map.Data()) };
            constants->view = view;
            constants->projection = projection;
        }

        // loop over each object node and render it
        for (const Object& object : scene.objects)
        {
            // compute object's model and normal matrices, then upload them to the GPU
            {
                Vector3 rotation_rad{};
                rotation_rad.x = DirectX::XMConvertToRadians(object.rotation.x());
                rotation_rad.y = DirectX::XMConvertToRadians(object.rotation.y());
                rotation_rad.z = DirectX::XMConvertToRadians(object.rotation.z());

                Matrix translate{ Matrix::CreateTranslation(Vector3{ object.position.elems }) };
                Matrix rotate{ Matrix::CreateFromYawPitchRoll(rotation_rad) };
                Matrix scale{ Matrix::CreateScale(Vector3{ object.scaling.elems }) };
                Matrix model{ scale * rotate * translate };
                Matrix normal{ scale * rotate };
                normal.Invert();
                normal.Transpose();

                d11::SubresourceMap map{ m_ctx, m_cb_object.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                auto constants{ static_cast<OpaquePassObjectConstants*>(map.Data()) };
                constants->model = model;
                constants->normal = normal;
                constants->albedo_color = Vector3{ object.albedo_color.elems };
                constants->albedo_mix = object.albedo_mix;
            }

            // set mesh related pipeline state and submit draw call
            {
                // fetch mesh
                const Mesh& mesh{ m_meshes.at(static_cast<std::size_t>(object.mesh_id)) };

                // fetch albedo
                const Texture& albedo{ m_textures.at(static_cast<std::size_t>(object.albedo_id)) };

                // prepare mesh related data for pipeline state
                ID3D11Buffer* vertices{ mesh.Vertices() };
                UINT vertex_stride{ sizeof(MeshVertex) };
                UINT vertex_offset{};

                // prepare texture related data for pipeline state
                ID3D11SamplerState* sss[]{ m_texture_ss.Get() };
                ID3D11ShaderResourceView* srvs[]{ albedo.SRV() };

                // set pipeline state
                m_ctx->IASetIndexBuffer(mesh.Indices(), MESH_INDEX_FORMAT, 0);
                m_ctx->IASetVertexBuffers(0, 1, &vertices, &vertex_stride, &vertex_offset);
                m_ctx->PSSetSamplers(0, std::size(sss), sss);
                m_ctx->PSSetShaderResources(0, std::size(srvs), srvs);

                // draw
                m_ctx->DrawIndexed(mesh.IndexCount(), 0, 0);
            }
        }
    }

    class RendererImpl
    {
    public:
        RendererImpl(ID3D11Device* dev, ID3D11DeviceContext* ctx);
        ~RendererImpl() = default;
        RendererImpl(const RendererImpl&) = delete;
        RendererImpl(RendererImpl&&) noexcept = delete;
        RendererImpl& operator=(const RendererImpl&) = delete;
        RendererImpl& operator=(RendererImpl&&) noexcept = delete;
    public:
        void Render(int w, int h, ID3D11RenderTargetView* rtv, const Scene& scene);
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        std::vector<Mesh> m_meshes;
        std::vector<Texture> m_textures;
        OpaquePass m_opaque_pass;
    };

    RendererImpl::RendererImpl(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_dev{ dev }
        , m_ctx{ ctx }
        , m_meshes{}
        , m_textures{}
        , m_opaque_pass{ m_dev, m_ctx, m_meshes, m_textures }
    {
        // upload default meshes
        {
            {
                size_t idx{ m_meshes.size() };
                m_meshes.emplace_back(Mesh::Cube(m_dev));
                qk_Check(MeshID{ idx } == qk::CUBE); // check that the mesh index matches its predefined id
            }
            {
                size_t idx{ m_meshes.size() };
                m_meshes.emplace_back(Mesh::Quad(m_dev));
                qk_Check(MeshID{ idx } == qk::QUAD); // check that the mesh index matches its predefined id
            }
        }
        // upload ddefault textures
        {
            {
                size_t idx{ m_textures.size() };
                m_textures.emplace_back(Texture::AlbedoBlack(m_dev));
                qk_Check(TextureID{ idx } == qk::ALBEDO_BLACK); // check that the texture index matches its predefined id
            }
            {
                size_t idx{ m_textures.size() };
                m_textures.emplace_back(Texture::AlbedoWhite(m_dev));
                qk_Check(TextureID{ idx } == qk::ALBEDO_WHITE); // check that the texture index matches its predefined id
            }
            {
                size_t idx{ m_textures.size() };
                m_textures.emplace_back(Texture::AlbedoPink(m_dev));
                qk_Check(TextureID{ idx } == qk::ALBEDO_PINK); // check that the texture index matches its predefined id
            }
            {
                size_t idx{ m_textures.size() };
                m_textures.emplace_back(Texture::AlbedoChecker(m_dev));
                qk_Check(TextureID{ idx } == qk::ALBEDO_CHECKER); // check that the texture index matches its predefined id
            }
        }
    }
    void RendererImpl::Render(int w, int h, ID3D11RenderTargetView* rtv, const Scene& scene)
    {
        // clear rtv using the specified background
        m_ctx->ClearRenderTargetView(rtv, scene.background.color.elems);

        // run render passes
        m_opaque_pass.Render(w, h, rtv, scene);
    }

    Renderer::Renderer(void* d3d_dev, void* d3d_ctx)
        : m_impl{ std::make_shared<RendererImpl>(static_cast<ID3D11Device*>(d3d_dev), static_cast<ID3D11DeviceContext*>(d3d_ctx)) }
    {
    }
    void Renderer::Render(int w, int h, void* rtv, const Scene& scene)
    {
        std::static_pointer_cast<RendererImpl>(m_impl)->Render(w, h, static_cast<ID3D11RenderTargetView*>(rtv), scene);
    }
}

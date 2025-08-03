#include <qk/PCH.h>
#include <qk/Renderer.h>
#include <qk/Mesh.h>
#include <qk/D11.h>

#include <SimpleMath.h>
using Matrix = DirectX::SimpleMath::Matrix;
using Vector3 = DirectX::SimpleMath::Vector3;
namespace dx = DirectX; // for DirectX namespace included by DirectXMath (included by SimpleMath)

#define matrix Matrix
#include <qk/hlsl/OpaquePassConstants.h>
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
            { { -0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            { { +0.5f, -0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { +0.5f, +0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -0.5f, +0.5f, +0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
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

    class OpaquePass
    {
    public:
        OpaquePass(ID3D11Device* dev, ID3D11DeviceContext* ctx, const std::vector<Mesh>& meshes);
        ~OpaquePass() = default;
        OpaquePass(const OpaquePass&) = delete;
        OpaquePass(OpaquePass&&) noexcept = delete;
        OpaquePass& operator=(const OpaquePass&) = delete;
        OpaquePass& operator=(OpaquePass&&) noexcept = delete;
    public:
        void Render(int w, int h, ID3D11RenderTargetView* rtv, std::span<const Node> nodes);
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        const std::vector<Mesh>& m_meshes;
        D3D11_VIEWPORT m_viewport;
        wrl::ComPtr<ID3D11VertexShader> m_vs;
        wrl::ComPtr<ID3D11PixelShader> m_ps;
        wrl::ComPtr<ID3D11InputLayout> m_il;
        wrl::ComPtr<ID3D11RasterizerState> m_rs;
        wrl::ComPtr<ID3D11Buffer> m_cb_scene;
        wrl::ComPtr<ID3D11Buffer> m_cb_object;
    };
    OpaquePass::OpaquePass(ID3D11Device* dev, ID3D11DeviceContext* ctx, const std::vector<Mesh>& meshes)
        : m_dev{ dev }
        , m_ctx{ ctx }
        , m_meshes{ meshes }
        , m_viewport{ .TopLeftX = 0.0f, .TopLeftY = 0.0f, .MinDepth = 0.0f, .MaxDepth = 1.0f }
        , m_vs{}
        , m_ps{}
        , m_il{}
        , m_rs{}
        , m_cb_scene{}
        , m_cb_object{}
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
    void OpaquePass::Render(int w, int h, ID3D11RenderTargetView* rtv, std::span<const Node> nodes)
    {
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
            m_ctx->OMSetRenderTargets(1, &rtv, nullptr);
        }

        // find the main camera, build its view and projection matrices, and upload them to the GPU
        {
            Matrix view{}, projection{};
            
            // we take as main camera the last main camera in the list
            auto it{ std::find_if(nodes.rbegin(), nodes.rend(), [](const Node& n) { return n.type == NodeType::Camera && n.camera.is_main; }) };
            if (it != nodes.rend()) // main camera found
            {
                float aspect{ m_viewport.Width / m_viewport.Height };
                float fov_rad{ DirectX::XMConvertToRadians(it->camera.fov_deg) };
                view = Matrix::CreateLookAt(Vector3{ it->camera.eye.elems }, Vector3{ it->camera.target.elems }, { 0.0f, 1.0f, 0.0f });
                projection = Matrix::CreatePerspectiveFieldOfView(fov_rad, aspect, it->camera.near_plane, it->camera.far_plane);
            }

            d11::SubresourceMap map{ m_ctx, m_cb_scene.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
            auto constants{ static_cast<OpaquePassSceneConstants*>(map.Data()) };
            constants->view = view;
            constants->projection = projection;
        }

        // loop over each object node and render it
        for (const Node& node : nodes)
        {
            if (node.type == NodeType::Object)
            {
                // compute object's model and normal matrices
                Matrix model{};
                Matrix normal{};
                {
                    Vector3 rotation_rad{};
                    rotation_rad.x = DirectX::XMConvertToRadians(node.object.rotation.x());
                    rotation_rad.y = DirectX::XMConvertToRadians(node.object.rotation.y());
                    rotation_rad.z = DirectX::XMConvertToRadians(node.object.rotation.z());

                    Matrix translate{ Matrix::CreateTranslation(Vector3{ node.object.position.elems }) };
                    Matrix rotate{ Matrix::CreateFromYawPitchRoll(rotation_rad) };
                    Matrix scale{ Matrix::CreateScale(Vector3{ node.object.scaling.elems }) };
                    model = scale * rotate * translate;
                    normal = scale * rotate;
                    normal.Invert();
                    normal.Transpose();
                }

                // upload object constants
                {
                    d11::SubresourceMap map{ m_ctx, m_cb_object.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                    auto constants{ static_cast<OpaquePassObjectConstants*>(map.Data()) };
                    constants->model = model;
                    constants->normal = normal;
                }

                // set mesh related pipeline state and submit draw call
                {
                    // fetch mesh
                    const Mesh& mesh{ m_meshes.at(static_cast<std::size_t>(node.object.mesh_id)) };

                    // prepare data for pipeline state
                    ID3D11Buffer* vertices{ mesh.Vertices() };
                    UINT vertex_stride{ sizeof(MeshVertex) };
                    UINT vertex_offset{};

                    // set pipeline state
                    m_ctx->IASetIndexBuffer(mesh.Indices(), MESH_INDEX_FORMAT, 0);
                    m_ctx->IASetVertexBuffers(0, 1, &vertices, &vertex_stride, &vertex_offset);

                    // draw
                    m_ctx->DrawIndexed(mesh.IndexCount(), 0, 0);
                }
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
        void Render(int w, int h, ID3D11RenderTargetView* rtv, std::span<const Node> nodes);
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        std::vector<Mesh> m_meshes;
        OpaquePass m_opaque_pass;
    };

    RendererImpl::RendererImpl(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_dev{ dev }
        , m_ctx{ ctx }
        , m_meshes{}
        , m_opaque_pass{ m_dev, m_ctx, m_meshes }
    {
        // upload cube and quad meshes
        {
            {
                size_t idx{ m_meshes.size() };
                m_meshes.emplace_back(Mesh::Cube(m_dev));
                qk_Check(MeshID{ idx } == qk::CUBE_MESH_ID); // check that the cube mesh index matches its predefined id
            }
            {
                size_t idx{ m_meshes.size() };
                m_meshes.emplace_back(Mesh::Quad(m_dev));
                qk_Check(MeshID{ idx } == qk::QUAD_MESH_ID); // check that the cube mesh index matches its predefined id
            }
        }
    }
    void RendererImpl::Render(int w, int h, ID3D11RenderTargetView* rtv, std::span<const Node> nodes)
    {
        // clear rtv
        {
            // if there is at least one backgorund node, find the last one and use its color as clear color, otherwise use black
            v4 clear_color{};
            {
                const auto it{ std::find_if(nodes.rbegin(), nodes.rend(), [](const Node& n) { return n.type == NodeType::Background; }) };
                if (it != nodes.rend())
                {
                    clear_color = it->background.color;
                }
            }
            m_ctx->ClearRenderTargetView(rtv, clear_color.elems);
        }

        // run render passes
        m_opaque_pass.Render(w, h, rtv, nodes);
    }

    Renderer::Renderer(void* d3d_dev, void* d3d_ctx)
        : m_impl{ std::make_shared<RendererImpl>(static_cast<ID3D11Device*>(d3d_dev), static_cast<ID3D11DeviceContext*>(d3d_ctx)) }
    {
    }
    void Renderer::Render(int w, int h, void* rtv, std::span<const Node> nodes)
    {
        std::static_pointer_cast<RendererImpl>(m_impl)->Render(w, h, static_cast<ID3D11RenderTargetView*>(rtv), nodes);
    }
}

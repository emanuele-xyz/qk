#include <qk/PCH.h>
#include <qk/Renderer.h>
#include <qk/Mesh.h>
#include <qk/D11.h>

namespace qk
{
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
        void Render(ID3D11RenderTargetView* rtv, std::span<const Node> nodes);
    private:
        ID3D11Device* m_dev;
        ID3D11DeviceContext* m_ctx;
        std::vector<Mesh> m_meshes;
    };

    RendererImpl::RendererImpl(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_dev{ dev }
        , m_ctx{ ctx }
        , m_meshes{}
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
    void RendererImpl::Render(ID3D11RenderTargetView* rtv, std::span<const Node> nodes)
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
    }

    Renderer::Renderer(void* d3d_dev, void* d3d_ctx)
        : m_impl{ std::make_shared<RendererImpl>(static_cast<ID3D11Device*>(d3d_dev), static_cast<ID3D11DeviceContext*>(d3d_ctx)) }
    {
    }
    void Renderer::Render(void* rtv, std::span<const Node> nodes)
    {
        std::static_pointer_cast<RendererImpl>(m_impl)->Render(static_cast<ID3D11RenderTargetView*>(rtv), nodes);
    }
}

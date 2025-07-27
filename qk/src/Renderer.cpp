#include <qk/PCH.h>
#include <qk/Renderer.h>
#include <qk/Mesh.h>
#include <qk/D11.h>

#include <SimpleMath.h>
using Matrix = DirectX::SimpleMath::Matrix;
namespace dx = DirectX; // for DirectX namespace included by DirectXMath (included by SimpleMath)

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
        void Render(int w, int h, ID3D11RenderTargetView* rtv, std::span<const Node> nodes);
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

        // initialize viewport
        D3D11_VIEWPORT viewport{};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(w);
        viewport.Height = static_cast<float>(h);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

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
            m_ctx->RSSetViewports(1, &viewport);
            m_ctx->OMSetRenderTargets(1, &rtv, nullptr);
        }

        // build the view and projection matrices from the last camera node and then upload them to the GPU
        {
            auto it{ std::find_if(nodes.rbegin(), nodes.rend(), [](const Node& n) { return n.type == NodeType::Camera; }) };
            qk_Check(it != nodes.rend());

            float aspect{ viewport.Width / viewport.Height };
            float fov_rad{ DirectX::XMConvertToRadians(camera.fov_deg) };

            d11::SubresourceMap map{ m_ctx, m_cb_scene.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
            auto constants{ static_cast<SceneConstants*>(map.Data()) };
            constants->view = Matrix::CreateLookAt(camera.eye, camera.target, { 0.0f, 1.0f, 0.0f });
            constants->projection = Matrix::CreatePerspectiveFieldOfView(fov_rad, aspect, camera.near_plane, camera.far_plane);
        }

        // loop over each model node and render it
        for (const Node& node : nodes)
        {
            if (node.type == NodeType::Model)
            {
                // upload object constants
                {
                    d11::SubresourceMap map{ m_ctx.Get(), m_cb_object.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0 };
                    auto constants{ static_cast<ObjectConstants*>(map.Data()) };
                    constants->model = obj.model;
                    constants->normal = obj.normal;
                }

                // set pipeline state
                {
                    // set pipeline state
                    m_ctx->IASetIndexBuffer(obj.mesh->Indices(), obj.mesh->IndexFormat(), 0);
                    m_ctx->IASetVertexBuffers(0, 1, obj.mesh->Vertices(), obj.mesh->Stride(), obj.mesh->Offset());
                }

                // draw
                m_ctx->DrawIndexed(obj.mesh->IndexCount(), 0, 0);
            }
        }
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

#include <qk/PCH.h>
#include <qk/Renderer.h>
#include <qk/D11.h>

namespace qk
{
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
    };

    RendererImpl::RendererImpl(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_dev{ dev }
        , m_ctx{ ctx }
    {
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

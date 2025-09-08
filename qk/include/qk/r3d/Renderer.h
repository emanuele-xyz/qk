#pragma once

#include <qk/r3d/Scene.h>

#include <memory>

namespace qk::r3d
{
    class Renderer
    {
    public:
        Renderer(void* d3d_dev, void* d3d_ctx);
        ~Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;
    public:
        void Render(int w, int h, void* rtv, const Scene& scene);
    private:
        std::shared_ptr<void> m_impl;
    };
}

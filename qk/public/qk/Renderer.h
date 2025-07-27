#pragma once

#include <qk/Node.h>

#include <memory>
#include <span>

namespace qk
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
		void Render(int w, int h, void* rtv, std::span<const Node> nodes);
	private:
		std::shared_ptr<void> m_impl;
	};
}

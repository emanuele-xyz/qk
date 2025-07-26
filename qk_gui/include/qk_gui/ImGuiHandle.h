#pragma once

#include <qk_gui/W32.h>
#include <qk_gui/D11.h>

namespace qk_gui
{
	class ImGuiHandle
	{
	public:
		ImGuiHandle(HWND window, ID3D11Device* dev, ID3D11DeviceContext* ctx);
		~ImGuiHandle();
		ImGuiHandle(const ImGuiHandle&) = delete;
		ImGuiHandle(ImGuiHandle&&) noexcept = delete;
		ImGuiHandle& operator=(const ImGuiHandle&) = delete;
		ImGuiHandle& operator=(ImGuiHandle&&) noexcept = delete;
	public:
		void BeginFrame() const noexcept;
		void EndFrame(ID3D11RenderTargetView* rtv) const noexcept;
	private:
		ID3D11DeviceContext* m_ctx;
	};
}

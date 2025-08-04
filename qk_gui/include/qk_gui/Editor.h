#pragma once

#include <qk/Qk.h>
#include <qk_gui/Keyboard.h>

#include <optional>
#include <span>
#include <vector>

namespace qk_gui
{
    class Editor
    {
    public:
        Editor(const Keyboard& keyboard);
        ~Editor() = default;
        Editor(const Editor&) = delete;
        Editor(Editor&&) noexcept = delete;
        Editor& operator=(const Editor&) = delete;
        Editor& operator=(Editor&&) noexcept = delete;
    public:
        std::span<const qk::Node> Nodes() const noexcept { return m_nodes; }
    public:
        void Update(float dt);
        void Render();
    private:
        void RenderBackgroundNode(qk::Node& node);
        void RenderCameraNode(qk::Node& node);
        void RenderObjectNode(qk::Node& node);
    private:
        const Keyboard& m_keyboard;
        std::vector<qk::Node> m_nodes;
        std::optional<std::size_t> m_to_be_removed_idx;
    };
}

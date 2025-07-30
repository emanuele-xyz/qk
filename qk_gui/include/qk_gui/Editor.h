#pragma once

#include <qk/Qk.h>

#include <vector>
#include <span>

namespace qk_gui
{
    class Editor
    {
    public:
        Editor();
        ~Editor() = default;
        Editor(const Editor&) = delete;
        Editor(Editor&&) noexcept = delete;
        Editor& operator=(const Editor&) = delete;
        Editor& operator=(Editor&&) noexcept = delete;
    public:
        std::span<const qk::Node> Nodes() const noexcept { return m_nodes; }
    public:
        void UpdateAndRender();
    private:
        void UpdateAndRenderBackground(qk::Node& node);
        void UpdateAndRenderCamera(qk::Node& node);
        void UpdateAndRenderObject(qk::Node& node);
    private:
        std::vector<qk::Node> m_nodes;
    };
}

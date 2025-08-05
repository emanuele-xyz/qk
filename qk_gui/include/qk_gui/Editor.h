#pragma once

#include <qk/Qk.h>
#include <qk_gui/Keyboard.h>
#include <qk_gui/Mouse.h>
#include <qk_gui/Camera.h>

#include <optional>
#include <span>
#include <vector>

namespace qk_gui
{
    class Editor
    {
    public:
        Editor(const Keyboard& keyboard, const Mouse& mouse);
        ~Editor() = default;
        Editor(const Editor&) = delete;
        Editor(Editor&&) noexcept = delete;
        Editor& operator=(const Editor&) = delete;
        Editor& operator=(Editor&&) noexcept = delete;
    public:
        const qk::Scene& Scene() const noexcept { return m_scene; }
    public:
        void Update(float dt);
        void Render();
    private:
        const Keyboard& m_keyboard;
        const Mouse& m_mouse;
        Camera m_camera;
        qk::Scene m_scene;
    };
}

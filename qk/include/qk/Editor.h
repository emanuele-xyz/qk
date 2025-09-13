#pragma once

#include <qk/Keyboard.h>
#include <qk/Mouse.h>
#include <qk/Camera.h>
#include <qk/r3d/Scene.h>
#include <qk/r3d/Renderer.h>

namespace qk
{
    class Editor
    {
    public:
        Editor(r3d::Renderer& renderer, const Keyboard& keyboard, const Mouse& mouse);
        ~Editor() = default;
        Editor(const Editor&) = delete;
        Editor(Editor&&) noexcept = delete;
        Editor& operator=(const Editor&) = delete;
        Editor& operator=(Editor&&) noexcept = delete;
    public:
        const r3d::Scene& Scene() const noexcept { return m_scene; }
    public:
        void Update(float dt);
        void Render();
    private:
        r3d::Renderer& m_renderer;
        const Keyboard& m_keyboard;
        const Mouse& m_mouse;
        Camera m_camera;
        r3d::Scene m_scene;
    };
}

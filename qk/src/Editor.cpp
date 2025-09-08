#include <qk/PCH.h>
#include <qk/Editor.h>
#include <qk/Commons.h>
#include <qk/Math.h>
#include <qk/imguiex/imguiex.h>

#include <imgui.h>

namespace qk
{
    constexpr float CAMERA_MOVE_SPEED{ 10.0f }; // TODO: make editable
    constexpr float CAMERA_MOVE_SPEED_MULTIPLIER{ 2.0f }; // TODO: make editable
    constexpr float CAMERA_LOOK_SPEED{ 2.0f }; // TODO: make editable

    static const char* GetSceneTransparencyTechniqueStr(r3d::SceneTransparencyTechnique technique)
    {
        const char* str{};
        switch (technique)
        {
        case r3d::SceneTransparencyTechnique::Sorted: { str = "Sorted"; } break;
        case r3d::SceneTransparencyTechnique::WeightedBlendedOIT: { str = "Weighted Blended OIT"; } break;
        default: { qk_Unreachable(); } break;
        }
        return str;
    }
    static const char* GetLightTypeStr(r3d::LightType light_type)
    {
        const char* str{};
        switch (light_type)
        {
        case r3d::LightType::Directional: { str = "Directional"; } break;
        case r3d::LightType::Point: { str = "Point"; } break;
        case r3d::LightType::Spot: { str = "Spot"; } break;
        default: { qk_Unreachable(); } break;
        }
        return str;
    }
    static const char* GetShadingModeStr(r3d::ShadingMode shading_mode)
    {
        const char* str{};
        switch (shading_mode)
        {
        case r3d::ShadingMode::Flat: { str = "Flat"; } break;
        case r3d::ShadingMode::Shaded: { str = "Shaded"; } break;
        default: { qk_Unreachable(); } break;
        }
        return str;
    }

    Editor::Editor(const Keyboard& keyboard, const Mouse& mouse)
        : m_keyboard{ keyboard }
        , m_mouse{ mouse }
        , m_camera{}
        , m_scene{}
    {
        // TODO: to be removed
        {
            // lights
            {
                r3d::Light light{};

                // directional light
                {
                    light = {};
                    light.type = r3d::LightType::Directional;
                    light.color = Vector3{ 0.5f, 0.5f, 0.5f };
                    light.direction = Vector3{ 0.0f, -1.0f, 0.0f };
                    m_scene.lights.emplace_back(light);
                }
                // point lights
                {
                    light = {};
                    light.type = r3d::LightType::Point;
                    light.position = Vector3{ 3.0f, 2.0f, 1.0f };
                    light.r_max = 2.0f;
                    m_scene.lights.emplace_back(light);

                    light = {};
                    light.type = r3d::LightType::Point;
                    light.position = Vector3{ -3.0f, 2.0f, 1.0f };
                    light.r_max = 2.0f;
                    m_scene.lights.emplace_back(light);
                }
                // spot lights
                {
                    light = {};
                    light.type = r3d::LightType::Spot;
                    light.position = Vector3{ 0.0f, 2.0f, 0.0f };
                    light.r_max = 10.0f;
                    m_scene.lights.emplace_back(light);
                }
            }
            // objects
            {
                r3d::Object object{};

                object = r3d::Object{};
                object.shading_mode = r3d::ShadingMode::Shaded;
                object.rotation = Vector3{ -90.0f, 0.0f, 0.0f };
                object.scaling = Vector3{ 10.0f, 10.0f, 1.0f };
                object.mesh_id = r3d::QUAD;
                object.albedo_mix = 0.75f;
                object.albedo_color = Vector3{ 1.0f, 0.0f, 0.0f };
                object.albedo_id = r3d::ALBEDO_CHECKER;
                m_scene.objects.emplace_back(object);

                object = r3d::Object{};
                object.shading_mode = r3d::ShadingMode::Shaded;
                object.position = Vector3{ 0.0f, 0.5f, 0.0f };
                object.mesh_id = r3d::CUBE;
                object.albedo_mix = 0.75f;
                object.albedo_color = Vector3{ 0.0f, 0.0f, 1.0f };
                object.albedo_id = r3d::ALBEDO_CHECKER;
                m_scene.objects.emplace_back(object);

                object = r3d::Object{};
                object.shading_mode = r3d::ShadingMode::Shaded;
                object.position = Vector3{ -2.0f, 2.0f, 0.0f };
                object.mesh_id = r3d::ICOSPHERE;
                object.albedo_mix = 0.0f;
                object.albedo_color = Vector3{ 0.0f, 1.0f, 1.0f };
                m_scene.objects.emplace_back(object);

                object = r3d::Object{};
                object.shading_mode = r3d::ShadingMode::Shaded;
                object.position = Vector3{ +2.0f, 2.0f, 0.0f };
                object.mesh_id = r3d::CONE;
                object.albedo_mix = 0.0f;
                object.albedo_color = Vector3{ 1.0f, 0.0f, 0.0f };
                m_scene.objects.emplace_back(object);
            }
        }
    }
    void Editor::Update(float dt)
    {
        // update camera
        {
            // camera mouse look logic
            if (m_mouse.ButtonState(Button::Right))
            {
                m_camera.AddPitch(-m_mouse.CursorDY() * CAMERA_LOOK_SPEED * dt);
                m_camera.AddYaw(m_mouse.CursorDX() * CAMERA_LOOK_SPEED * dt);
            }

            // compute canera move direction
            Vector3 move{};
            {
                Vector3 forward{ m_camera.Forward() };
                Vector3 right{ m_camera.Right() };
                Vector3 up{ m_camera.Up() };
                if (m_keyboard.KeyState(Key::W))
                {
                    move += forward;
                }
                if (m_keyboard.KeyState(Key::S))
                {
                    move -= forward;
                }
                if (m_keyboard.KeyState(Key::D))
                {
                    move += right;
                }
                if (m_keyboard.KeyState(Key::A))
                {
                    move -= right;
                }
                if (m_keyboard.KeyState(Key::Space))
                {
                    if (!m_keyboard.KeyState(Key::Ctrl))
                    {
                        move += up;
                    }
                    else
                    {
                        move -= up;
                    }
                }
                move.Normalize();
            }

            // compute move speed
            float speed{ CAMERA_MOVE_SPEED };
            if (m_keyboard.KeyState(Key::Shift))
            {
                speed *= CAMERA_MOVE_SPEED_MULTIPLIER;
            }

            // apply movement to camera
            m_camera.Move(move * speed * dt);

            // set scene camera
            {
                Vector3 eye{ m_camera.Position() };
                Vector3 target{ m_camera.Target() };
                Vector3 up{ m_camera.Up() };
                m_scene.camera.eye = Vector3{ eye.x, eye.y, eye.z };
                m_scene.camera.target = Vector3{ target.x, target.y, target.z };
                m_scene.camera.up = Vector3{ up.x, up.y, up.z };
            }
        }
    }
    void Editor::Render()
    {
        ImGui::Begin("Scene");
        {
            ImGuiTabBarFlags tab_bar_flags{ ImGuiTabBarFlags_None };
            if (ImGui::BeginTabBar("SceneTabBar", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Settings"))
                {
                    ImGui::DragFloat("Gamma", &m_scene.settings.gamma, 0.1f, 0.0f, 1000.0f); // TODO: hardcoded max gamma
                    if (ImGui::BeginCombo("Transparency", GetSceneTransparencyTechniqueStr(m_scene.settings.transparency)))
                    {
                        for (int n{}; n < static_cast<int>(r3d::SceneTransparencyTechnique::Count); n++)
                        {
                            r3d::SceneTransparencyTechnique n_technique{ static_cast<r3d::SceneTransparencyTechnique>(n) };
                            bool is_selected{ m_scene.settings.transparency == n_technique };
                            if (ImGui::Selectable(GetSceneTransparencyTechniqueStr(n_technique), is_selected))
                            {
                                m_scene.settings.transparency = n_technique;
                            }
                            if (is_selected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Background"))
                {
                    ImGuiEx::ColorEdit3("Color", m_scene.background.color);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Lights"))
                {
                    for (std::size_t i{}; i < m_scene.lights.size(); i++)
                    {
                        r3d::Light& light{ m_scene.lights[i] };

                        ImGui::PushID(static_cast<int>(i));
                        {
                            std::string label{ std::format("Light {}", i) };
                            ImGuiTreeNodeFlags tree_node_flags{ ImGuiTreeNodeFlags_DefaultOpen };
                            if (ImGui::CollapsingHeader(label.c_str(), tree_node_flags))
                            {
                                if (ImGui::BeginCombo("Type", GetLightTypeStr(light.type)))
                                {
                                    for (int n{}; n < static_cast<int>(r3d::LightType::Count); n++)
                                    {
                                        r3d::LightType light_type{ static_cast<r3d::LightType>(n) };
                                        bool is_selected{ light.type == light_type };
                                        if (ImGui::Selectable(GetLightTypeStr(light_type), is_selected))
                                        {
                                            light.type = light_type;
                                        }
                                        if (is_selected)
                                        {
                                            ImGui::SetItemDefaultFocus();
                                        }
                                    }
                                    ImGui::EndCombo();
                                }

                                switch (light.type)
                                {
                                case r3d::LightType::Directional:
                                {
                                    ImGuiEx::DragFloat3("Direction", light.direction, 0.01f, -1.0f, +1.0f);
                                    ImGuiEx::ColorEdit3("Color", light.color);
                                } break;
                                case r3d::LightType::Point:
                                {
                                    ImGuiEx::DragFloat3("Position", light.position, 0.01f);
                                    ImGuiEx::ColorEdit3("Color", light.color);
                                    ImGui::DragFloat("Min Radius", &light.r_min, 0.01f, 0.01f, 1000.0f);
                                    ImGui::DragFloat("Max Radius", &light.r_max, 0.01f, 0.01f, 1000.0f);
                                } break;
                                case r3d::LightType::Spot:
                                {
                                    ImGuiEx::DragFloat3("Position", light.position, 0.01f);
                                    ImGuiEx::DragFloat3("Direction", light.direction, 0.01f);
                                    ImGuiEx::ColorEdit3("Color", light.color);
                                    ImGui::DragFloat("Min Radius", &light.r_min, 0.01f, 0.01f, 1000.0f);
                                    ImGui::DragFloat("Max Radius", &light.r_max, 0.01f, 0.01f, 1000.0f);
                                    ImGui::DragFloat("Umbra (deg)", &light.umbra_angle_deg, 0.1f, 0.0f, 89.0f);
                                    ImGui::DragFloat("Penumbra (deg)", &light.penumbra_angle_deg, 0.1f, 0.0f, 89.0f);
                                } break;
                                default:
                                {
                                    qk_Unreachable();
                                } break;
                                }
                                ImGui::Checkbox("Render Gizmos", &light.render_gizmos);
                            }
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Objects"))
                {
                    for (std::size_t i{}; i < m_scene.objects.size(); i++)
                    {
                        r3d::Object& object{ m_scene.objects[i] };

                        ImGui::PushID(static_cast<int>(i));
                        {
                            std::string label{ std::format("Object {}", i) };
                            ImGuiTreeNodeFlags tree_node_flags{ ImGuiTreeNodeFlags_DefaultOpen };
                            if (ImGui::CollapsingHeader(label.c_str(), tree_node_flags))
                            {
                                if (ImGui::BeginCombo("Shading Mode", GetShadingModeStr(object.shading_mode)))
                                {
                                    for (int n{}; n < static_cast<int>(r3d::ShadingMode::Count); n++)
                                    {
                                        r3d::ShadingMode shading_mode{ static_cast<r3d::ShadingMode>(n) };
                                        bool is_selected{ object.shading_mode == shading_mode };
                                        if (ImGui::Selectable(GetShadingModeStr(shading_mode), is_selected))
                                        {
                                            object.shading_mode = shading_mode;
                                        }
                                        if (is_selected)
                                        {
                                            ImGui::SetItemDefaultFocus();
                                        }
                                    }
                                    ImGui::EndCombo();
                                }

                                ImGuiEx::DragFloat3("Position", object.position, 0.1f);
                                ImGuiEx::DragFloat3("Rotation", object.rotation, 0.1f);
                                ImGuiEx::DragFloat3("Scaling", object.scaling, 0.1f);
                                ImGui::Text("Mesh ID: %d", object.mesh_id);
                                ImGui::DragFloat("Albedo Mix", &object.albedo_mix, 0.001f, 0.0f, 1.0f);
                                ImGuiEx::ColorEdit3("Albedo Color", object.albedo_color);
                                ImGui::Text("Albedo ID: %d", object.albedo_id);
                                ImGui::DragFloat("Opacity", &object.opacity, 0.01f, 0.0f, 1.0f);

                                // TODO: use combo for mesh
                                //const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO", "PPPP", "QQQQQQQQQQ", "RRR", "SSSS" };
                                //static const char* current_item = NULL;
                                //
                                //if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
                                //{
                                //    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                                //    {
                                //        bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
                                //        if (ImGui::Selectable(items[n], is_selected)
                                //            current_item = items[n];
                                //            if (is_selected)
                                //                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                                //    }
                                //    ImGui::EndCombo();
                                //}
                            }
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();

        ImGui::Begin("Camera");
        {
            ImGui::DragFloat("FOV (degrees)", &m_scene.camera.fov_deg, 0.1f, 0.0f, 360.0f);
            ImGui::DragFloat("Near Plane", &m_scene.camera.near_plane, 0.1f);
            ImGui::DragFloat("Far Plane", &m_scene.camera.far_plane, 0.1f);
        }
        ImGui::End();
    }
}

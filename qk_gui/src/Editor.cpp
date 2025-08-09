#include <qk_gui/PCH.h>
#include <qk_gui/Editor.h>
#include <qk_gui/Commons.h>
#include <qk_gui/Math.h>

#include <imgui.h>

namespace qk_gui
{
    constexpr float CAMERA_MOVE_SPEED{ 10.0f }; // TODO: make editable
    constexpr float CAMERA_MOVE_SPEED_MULTIPLIER{ 2.0f }; // TODO: make editable
    constexpr float CAMERA_LOOK_SPEED{ 2.0f }; // TODO: make editable

    Editor::Editor(const Keyboard& keyboard, const Mouse& mouse)
        : m_keyboard{ keyboard }
        , m_mouse{ mouse }
        , m_camera{}
        , m_scene{}
    {
        // TODO: to be removed
        {
            m_scene.directional_light.color = qk::v3{};
            m_scene.directional_light.direction = qk::v3{};

            {
                qk::PointLight point_light{};

                point_light.position = qk::v3{ 1.0f, 1.0f, 1.0f };
                m_scene.point_lights.emplace_back(point_light);

                point_light.position = qk::v3{ -1.0f, +1.0f, -1.0f };
                m_scene.point_lights.emplace_back(point_light);
            }

            {
                qk::Object object{};

                object = qk::Object{};
                object.rotation = qk::v3{ -90.0f, 0.0f, 0.0f };
                object.scaling = qk::v3{ 10.0f, 10.0f, 1.0f };
                object.mesh_id = qk::QUAD;
                object.albedo_mix = 0.75f;
                object.albedo_color = qk::v3{ 1.0f, 0.0f, 0.0f };
                object.albedo_id = qk::ALBEDO_CHECKER;
                m_scene.objects.emplace_back(object);

                object = qk::Object{};
                object.position = qk::v3{ 0.0f, 0.5f, 0.0f };
                object.mesh_id = qk::CUBE;
                object.albedo_mix = 0.75f;
                object.albedo_color = qk::v3{ 0.0f, 0.0f, 1.0f };
                object.albedo_id = qk::ALBEDO_CHECKER;
                m_scene.objects.emplace_back(object);
            }
        }
    }
    void Editor::Update(float dt)
    {
        // update camera
        {
            // camera mouse look logic
            if (m_mouse.ButtonState(Button::Left))
            {
                m_camera.AddPitch(m_mouse.CursorDY() * CAMERA_LOOK_SPEED * dt);
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
                m_scene.camera.eye = qk::v3{ eye.x, eye.y, eye.z };
                m_scene.camera.target = qk::v3{ target.x, target.y, target.z };
                m_scene.camera.up = qk::v3{ up.x, up.y, up.z };
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
                if (ImGui::BeginTabItem("Background"))
                {
                    ImGui::ColorEdit3("Color", m_scene.background.color.elems);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Directional Light"))
                {
                    ImGui::DragFloat3("Direction", m_scene.directional_light.direction.elems, 0.01f, -1.0f, +1.0f);
                    ImGui::ColorEdit3("Color", m_scene.directional_light.color.elems);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Point Lights"))
                {
                    for (std::size_t i{}; i < m_scene.point_lights.size(); i++)
                    {
                        qk::PointLight& point_light{ m_scene.point_lights[i] };

                        ImGui::PushID(static_cast<int>(i));
                        {
                            std::string label{ std::format("Point Light {}", i) };
                            ImGuiTreeNodeFlags tree_node_flags{ ImGuiTreeNodeFlags_DefaultOpen };
                            if (ImGui::CollapsingHeader(label.c_str(), tree_node_flags))
                            {
                                ImGui::DragFloat3("Position", point_light.position.elems, 0.01f);
                                ImGui::ColorEdit3("Color", point_light.color.elems);
                                ImGui::DragFloat("Min Radius", &point_light.r_min, 0.01f, 0.01f, 1000.0f);
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
                        qk::Object& object{ m_scene.objects[i] };

                        ImGui::PushID(static_cast<int>(i));
                        {
                            std::string label{ std::format("Object {}", i) };
                            ImGuiTreeNodeFlags tree_node_flags{ ImGuiTreeNodeFlags_DefaultOpen };
                            if (ImGui::CollapsingHeader(label.c_str(), tree_node_flags))
                            {
                                ImGui::DragFloat3("Position", object.position.elems, 0.1f);
                                ImGui::DragFloat3("Rotation", object.rotation.elems, 0.1f);
                                ImGui::DragFloat3("Scaling", object.scaling.elems, 0.1f);
                                ImGui::Text("Mesh ID: %d", object.mesh_id);
                                ImGui::DragFloat("Albedo Mix", &object.albedo_mix, 0.001f, 0.0f, 1.0f);
                                ImGui::ColorEdit3("Albedo Color", object.albedo_color.elems);
                                ImGui::Text("Albedo ID: %d", object.albedo_id);

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

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
        m_scene.objects.emplace_back(qk::Object{ qk::v3{}, qk::v3{ -90.0f, 0.0f, 0.0f }, qk::v3{ 10.0f, 10.0f, 1.0f }, qk::QUAD_MESH_ID });
        m_scene.objects.emplace_back(qk::Object{ qk::v3{}, qk::v3{}, qk::v3{ 1.0f, 1.0f, 1.0f }, qk::CUBE_MESH_ID });
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
        #if 0
        ImGui::Begin("Nodes");
        {
            for (std::size_t i{}; i < m_nodes.size(); i++)
            {
                qk::Node& node{ m_nodes[i] };

                ImGui::PushID(static_cast<int>(i));
                {
                    if (ImGui::CollapsingHeader(qk::NodeTypeStr(node.type), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        // render node type
                        if (ImGui::BeginCombo("Type", qk::NodeTypeStr(node.type)))
                        {
                            for (int type_idx{}; type_idx < static_cast<int>(qk::NodeType::Count); type_idx++)
                            {
                                qk::NodeType type{ static_cast<qk::NodeType>(type_idx) };
                                bool is_selected{ node.type == type };
                                if (ImGui::Selectable(qk::NodeTypeStr(type), is_selected))
                                {
                                    node = qk::Node::MakeDefault(type);
                                }
                                if (is_selected)
                                {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                            ImGui::EndCombo();
                        }

                        // render node ui
                        switch (node.type)
                        {
                        case qk::NodeType::Background: { RenderBackgroundNode(node); } break;
                        case qk::NodeType::Camera: { RenderCameraNode(node); } break;
                        case qk::NodeType::Object: { RenderObjectNode(node); } break;
                        default: { qk_gui_Unreachable(); } break;
                        }

                        // render remove node button
                        if (ImGui::Button("Remove Node"))
                        {
                            m_to_be_removed_idx = i;
                        }
                    }
                }
                ImGui::PopID();
            }

            ImGui::Separator();

            // render add node button
            if (ImGui::Button("Add Node"))
            {
                m_nodes.emplace_back(qk::DEFAULT_BACKGROUND);
            }
        }
        ImGui::End();
        #endif
    }
    #if 0
    void Editor::RenderBackgroundNode(qk::Node& node)
    {
        ImGui::ColorEdit3("Color", node.background.color.elems);
    }
    void Editor::RenderCameraNode(qk::Node& node)
    {
        ImGui::Text("Main:");
        ImGui::SameLine();
        if (node.camera.is_main)
        {
            ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, "Yes");
        }
        else
        {
            ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, "No");
        }
        ImGui::DragFloat3("Eye", node.camera.eye.elems);
        ImGui::DragFloat3("Target", node.camera.target.elems);
        ImGui::DragFloat3("Up", node.camera.up.elems);
        ImGui::DragFloat("FOV (degrees)", &node.camera.fov_deg);
        ImGui::DragFloat("Near Plane", &node.camera.near_plane);
        ImGui::DragFloat("Far Plane", &node.camera.far_plane);
        if (!node.camera.is_main) // render make main camera button if camera is not a main camera
        {
            if (ImGui::Button("Make Main"))
            {
                // when marking a camera as main, we need to unmark all the other cameras
                for (qk::Node& n : m_nodes)
                {
                    if (n.type == qk::NodeType::Camera)
                    {
                        n.camera.is_main = false;
                    }
                }

                // mark this camera node as a main camera
                node.camera.is_main = true;
            }
        }
    }
    void Editor::RenderObjectNode(qk::Node& node)
    {
        ImGui::DragFloat3("Position", node.object.position.elems);
        ImGui::DragFloat3("Rotation", node.object.rotation.elems);
        ImGui::DragFloat3("Scaling", node.object.scaling.elems);

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
    #endif
}

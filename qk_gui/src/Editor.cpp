#include <qk_gui/PCH.h>
#include <qk_gui/Editor.h>
#include <qk_gui/Commons.h>
#include <qk_gui/Math.h>

#include <imgui.h>

namespace qk_gui
{
    constexpr float CAMERA_MOVE_SPEED{ 10.0f };
    constexpr float CAMERA_MOVE_SPEED_MULTIPLIER{ 2.0f };

    Editor::Editor(const Keyboard& keyboard)
        : m_keyboard{ keyboard }
        , m_nodes{}
        , m_to_be_removed_idx{}
    {
        // TODO: to be removed
        m_nodes.emplace_back(qk::DEFAULT_BACKGROUND);
        m_nodes.emplace_back(qk::Node::MakeObject({ qk::v3{}, qk::v3{ -90.0f, 0.0f, 0.0f }, qk::v3{ 10.0f, 10.0f, 1.0f }, qk::QUAD_MESH_ID }));
        m_nodes.emplace_back(qk::Node::MakeObject({ qk::v3{}, qk::v3{}, qk::v3{ 1.0f, 1.0f, 1.0f }, qk::CUBE_MESH_ID }));
        m_nodes.emplace_back(qk::Node::MakeCamera({ true, qk::v3{ 0.0f, 2.0f, 5.0f }, qk::v3{}, qk::v3{ 0.0f, 1.0f, 0.0f }, 45.0f, 0.01f, 100.0f }));
        m_nodes.emplace_back(qk::Node::MakeCamera({ false, qk::v3{ 0.0f, 2.0f, 5.0f }, qk::v3{}, qk::v3{ 0.0f, 1.0f, 0.0f }, 45.0f, 0.01f, 100.0f }));
        m_nodes.emplace_back(qk::Node::MakeCamera({ false, qk::v3{ 0.0f, 2.0f, 5.0f }, qk::v3{}, qk::v3{ 0.0f, 1.0f, 0.0f }, 45.0f, 0.01f, 100.0f }));
    }
    void Editor::Update(float dt)
    {
        // if there is a node to be removed, do it
        if (m_to_be_removed_idx)
        {
            m_nodes.erase(m_nodes.begin() + m_to_be_removed_idx.value());
            m_to_be_removed_idx = {}; // the node has been removed
        }

        // update camera
        {
            qk::Node* camera_node{};

            // get a pointer to the node representing the main camera
            {
                // search for the last main camera node
                auto it{ std::find_if(m_nodes.rbegin(), m_nodes.rend(), [](const qk::Node& n) { return n.type == qk::NodeType::Camera && n.camera.is_main; }) };
                if (it != m_nodes.rend()) // node found
                {
                    camera_node = std::to_address(it);
                }
                else // no main camera node found
                {
                    // search for the last camera node
                    it = std::find_if(m_nodes.rbegin(), m_nodes.rend(), [](const qk::Node& n) { return n.type == qk::NodeType::Camera; });
                    if (it != m_nodes.rend()) // node found
                    {
                        it->camera.is_main = true; // make camera node main
                        camera_node = std::to_address(it);
                    }
                }
            }

            // update main camera, if it exists
            if (camera_node)
            {
                Vector3 eye{ camera_node->camera.eye.elems };
                Vector3 target{ camera_node->camera.target.elems };
                Vector3 up{ camera_node->camera.up.elems };

                // compute camera forward and camera right
                Vector3 forward{ target - eye };
                Vector3 right{ forward };
                right = right.Cross(up);

                // compute move vector based on keyboard input
                Vector3 move{};
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
                move.Normalize();

                // compute speed
                float speed{ CAMERA_MOVE_SPEED };
                if (m_keyboard.KeyState(Key::Shift))
                {
                    speed *= CAMERA_MOVE_SPEED_MULTIPLIER;
                }

                // apply movement to camera's position and target vectors
                eye += move * speed * dt;
                target += move * speed * dt;

                // write new camera position and target back into the node
                camera_node->camera.eye = qk::v3{ eye.x, eye.y, eye.z };
                camera_node->camera.target = qk::v3{ target.x, target.y, target.z };
            }
        }
    }
    void Editor::Render()
    {
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
    }
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
}

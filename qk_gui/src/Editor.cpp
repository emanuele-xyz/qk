#include <qk_gui/PCH.h>
#include <qk_gui/Editor.h>
#include <qk_gui/Commons.h>

#include <imgui.h>

namespace qk_gui
{
    Editor::Editor()
        : m_nodes{}
    {
        // TODO: to be removed
        m_nodes.emplace_back(qk::Node::MakeBackground(qk::v4{ 1.0f, 0.0f, 1.0f, 1.0f }));
        m_nodes.emplace_back(qk::Node::MakeCamera(qk::v3{ 0.0f, 2.0f, 5.0f }, qk::v3{}, qk::v3{ 0.0f, 1.0f, 0.0f }, 45.0f, 0.01f, 100.0f));
        m_nodes.emplace_back(qk::Node::MakeObject(qk::v3{}, qk::v3{ -90.0f, 0.0f, 0.0f }, qk::v3{ 10.0f, 10.0f, 1.0f }, qk::QUAD_MESH_ID));
        m_nodes.emplace_back(qk::Node::MakeObject(qk::v3{}, qk::v3{}, qk::v3{ 1.0f, 1.0f, 1.0f }, qk::CUBE_MESH_ID));
    }
    void Editor::UpdateAndRender()
    {
        ImGui::Begin("QkGUI");
        {
            for (std::size_t i{}; i < m_nodes.size(); i++)
            {
                qk::Node& node{ m_nodes[i] };

                ImGui::PushID(static_cast<int>(i));
                {
                    if (ImGui::CollapsingHeader(qk::NodeTypeStr(node.type), ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        switch (node.type)
                        {
                        case qk::NodeType::Background: { UpdateAndRenderBackground(node); } break;
                        case qk::NodeType::Camera: { UpdateAndRenderCamera(node); } break;
                        case qk::NodeType::Object: { UpdateAndRenderObject(node); } break;
                        default: { qk_gui_Unreachable(); } break;
                        }
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::End();
    }
    void Editor::UpdateAndRenderBackground(qk::Node& node)
    {
        ImGui::ColorEdit3("Color", node.background.color.elems);
    }
    void Editor::UpdateAndRenderCamera(qk::Node& node)
    {
        ImGui::DragFloat3("Eye", node.camera.eye.elems);
        ImGui::DragFloat3("Target", node.camera.target.elems);
        ImGui::DragFloat3("Up", node.camera.up.elems);
        ImGui::DragFloat("FOV (degrees)", &node.camera.fov_deg);
        ImGui::DragFloat("Near Plane", &node.camera.near_plane);
        ImGui::DragFloat("Far Plane", &node.camera.far_plane);
    }
    void Editor::UpdateAndRenderObject(qk::Node& node)
    {
        ImGui::DragFloat3("position", node.object.position.elems);
        ImGui::DragFloat3("rotation", node.object.rotation.elems);
        ImGui::DragFloat3("scaling", node.object.scaling.elems);

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

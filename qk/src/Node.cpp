#include <qk/PCH.h>
#include <qk/Node.h>

namespace qk
{
    Node Node::MakeBackground(v4 color)
    {
        Node node{};
        node.type = NodeType::Background;
        node.background.color = color;
        return node;
    }
    Node Node::MakeCamera(v3 eye, v3 target, v3 up, float fov_deg, float near_plane, float far_plane)
    {
        Node node{};
        node.type = NodeType::Camera;
        node.camera.eye = eye;
        node.camera.target = target;
        node.camera.up = up;
        node.camera.fov_deg = fov_deg;
        node.camera.near_plane = near_plane;
        node.camera.far_plane = far_plane;
        return node;
    }
    Node Node::MakeModel(MeshID mesh_id)
    {
        Node node{};
        node.type = NodeType::Model;
        node.model.mesh_id = mesh_id;
        return node;
    }
}

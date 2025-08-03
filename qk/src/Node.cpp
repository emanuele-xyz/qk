#include <qk/PCH.h>
#include <qk/Node.h>
#include <qk/Commons.h>

namespace qk
{
    const char* NodeTypeStr(NodeType type)
    {
        const char* str{};
        switch (type)
        {
        case NodeType::Background: { str = "Background"; } break;
        case NodeType::Camera: { str = "Camera"; } break;
        case NodeType::Object: { str = "Object"; } break;
        default: { qk_Unreachable(); } break;
        }
        return str;
    }
    Node Node::MakeBackground(v4 color)
    {
        Node node{};
        node.type = NodeType::Background;
        node.background.color = color;
        return node;
    }
    Node Node::MakeCamera(bool is_main, v3 eye, v3 target, v3 up, float fov_deg, float near_plane, float far_plane)
    {
        Node node{};
        node.type = NodeType::Camera;
        node.camera.is_main = is_main;
        node.camera.eye = eye;
        node.camera.target = target;
        node.camera.up = up;
        node.camera.fov_deg = fov_deg;
        node.camera.near_plane = near_plane;
        node.camera.far_plane = far_plane;
        return node;
    }
    Node Node::MakeObject(v3 position, v3 rotation, v3 scaling, MeshID mesh_id)
    {
        Node node{};
        node.type = NodeType::Object;
        node.object.position = position;
        node.object.rotation = rotation;
        node.object.scaling = scaling;
        node.object.mesh_id = mesh_id;
        return node;
    }
}

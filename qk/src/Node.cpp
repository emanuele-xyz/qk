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
    Node Node::MakeModel(MeshID mesh_id)
    {
        Node node{};
        node.type = NodeType::Model;
        node.model.mesh_id = mesh_id;
        return node;
    }
}

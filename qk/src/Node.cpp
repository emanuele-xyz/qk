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
}

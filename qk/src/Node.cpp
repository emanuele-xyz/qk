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
    Node Node::MakeDefault(NodeType type)
    {
        Node node{};
        switch (type)
        {
        case NodeType::Background: { node = DEFAULT_BACKGROUND; } break;
        case NodeType::Camera: { node = DEFAULT_CAMERA; } break;
        case NodeType::Object: { node = DEFAULT_OBJECT; } break;
        default: { qk_Unreachable(); } break;
        }
        return node;
    }
    Node Node::MakeBackground(Background background)
    {
        Node node{};
        node.type = NodeType::Background;
        node.background = background;
        return node;
    }
    Node Node::MakeCamera(Camera camera)
    {
        Node node{};
        node.type = NodeType::Camera;
        node.camera = camera;
        return node;
    }
    Node Node::MakeObject(Object object)
    {
        Node node{};
        node.type = NodeType::Object;
        node.object = object;
        return node;
    }
}

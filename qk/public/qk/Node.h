#pragma once

#include <qk/Vectors.h>

namespace qk
{
    enum class NodeType
    {
        Background
    };

    struct Node
    {
        static Node MakeBackground(v4 color);

        NodeType type;
        union
        {
            struct
            {
                v4 color;
            } background;
        };
    };
}

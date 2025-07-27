#pragma once

#include <qk/Mesh.h>
#include <qk/Vectors.h>

namespace qk
{
    enum class NodeType
    {
        Background,
        Model,
    };

    struct Node
    {
        static Node MakeBackground(v4 color);
        static Node MakeModel(MeshID mesh_id);

        NodeType type;
        union
        {
            struct
            {
                v4 color;
            } background;
            struct
            {
                MeshID mesh_id;
            } model;
        };
    };
}

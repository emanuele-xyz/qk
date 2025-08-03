#pragma once

#include <qk/Mesh.h>
#include <qk/Vectors.h>

namespace qk
{
    enum class NodeType
    {
        Background,
        Camera,
        Object,
    };

    const char* NodeTypeStr(NodeType type);

    struct Node
    {
        static Node MakeBackground(v4 color);
        static Node MakeCamera(bool is_main, v3 eye, v3 target, v3 up, float fov_deg, float near_plane, float far_plane);
        static Node MakeObject(v3 position, v3 rotation, v3 scaling, MeshID mesh_id);

        NodeType type;
        union
        {
            struct
            {
                v4 color;
            } background;
            struct
            {
                bool is_main;
                v3 eye;
                v3 target;
                v3 up;
                float fov_deg;
                float near_plane;
                float far_plane;
            } camera;
            struct
            {
                v3 position;
                v3 rotation;
                v3 scaling;
                MeshID mesh_id;
            } object;
        };
    };
}

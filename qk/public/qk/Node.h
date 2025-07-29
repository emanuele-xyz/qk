#pragma once

#include <qk/Mesh.h>
#include <qk/Vectors.h>

namespace qk
{
    enum class NodeType
    {
        Background,
        Camera,
        Model,
    };

    struct Node
    {
        static Node MakeBackground(v4 color);
        static Node MakeCamera(v3 eye, v3 target, v3 up, float fov_deg, float near_plane, float far_plane);
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
            } model;
        };
    };
}

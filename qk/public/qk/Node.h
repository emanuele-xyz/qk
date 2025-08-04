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
        Count
    };

    const char* NodeTypeStr(NodeType type);

    struct Background
    {
        v4 color;
    };


    struct Camera
    {
        bool is_main;
        v3 eye;
        v3 target;
        v3 up;
        float fov_deg;
        float near_plane;
        float far_plane;
    };

    struct Object
    {
        v3 position{};
        v3 rotation{};
        v3 scaling{ 1.0f, 1.0f, 1.0f };
        MeshID mesh_id{ CUBE_MESH_ID };
    };

    struct Node
    {
        static Node MakeDefault(NodeType type);
        static Node MakeBackground(Background background);
        static Node MakeCamera(Camera camera);
        static Node MakeObject(Object object);

        NodeType type;
        union
        {
            Background background;
            Camera camera;
            Object object;
        };
    };

    constexpr Node DEFAULT_BACKGROUND
    {
        .type{ NodeType::Background },
        .background
        {
            .color{ 1.0f, 0.0f, 1.0f, 1.0f },
        }
    };
    constexpr Node DEFAULT_CAMERA
    {
        .type{ NodeType::Camera },
        .camera
        {
            .is_main{ false },
            .eye{ 0.0f, 1.0f, 5.0f },
            .target{},
            .up{ 0.0f, 1.0f, 0.0f },
            .fov_deg{ 45.0f },
            .near_plane{ 0.01f },
            .far_plane{ 100.0f },
        }
    };
    constexpr Node DEFAULT_OBJECT
    {
        .type{ NodeType::Object },
        .object
        {
            .position{},
            .rotation{},
            .scaling{ 1.0f, 1.0f, 1.0f },
            .mesh_id{ CUBE_MESH_ID },
        }
    };
}

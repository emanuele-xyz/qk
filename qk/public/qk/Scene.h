#pragma once

#include <qk/Mesh.h>
#include <qk/Texture.h>
#include <qk/Vectors.h>

#include <vector>

namespace qk
{
    struct Background
    {
        v4 color{ 1.0f, 0.0f, 1.0f, 1.0f };
    };

    struct Camera
    {
        v3 eye{};
        v3 target{ 0.0f, 1.0f, +5.0f };
        v3 up{ 0.0f, 1.0f, 0.0f };
        float fov_deg{ 45.0f };
        float near_plane{ 0.01f };
        float far_plane{ 100.0f };
    };

    struct Object
    {
        v3 position{};
        v3 rotation{};
        v3 scaling{ 1.0f, 1.0f, 1.0f };
        MeshID mesh_id{ CUBE_MESH_ID };
        TextureID albedo_id{ ALBEDO_BLACK_TEXTURE_ID };
    };

    struct Scene
    {
        Background background{};
        Camera camera{};
        std::vector<Object> objects{};
    };
}

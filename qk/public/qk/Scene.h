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

    struct DirectionalLight
    {
        v3 direction{ -1.0f, -1.0f, -1.0f };
        v3 color{ 1.0f, 1.0f, 1.0f };
    };

    struct PointLight
    {
        v3 position{};
        v3 color{ 1.0f, 1.0f, 1.0f };
        float r_min{ 0.1f };
    };

    struct Object
    {
        v3 position{};
        v3 rotation{};
        v3 scaling{ 1.0f, 1.0f, 1.0f };
        MeshID mesh_id{ CUBE };
        float albedo_mix{}; // 0 for color, 1 for texture
        v3 albedo_color{ 1.0f, 1.0f, 1.0f };
        TextureID albedo_id{ ALBEDO_BLACK };
    };

    struct Scene
    {
        Background background{};
        Camera camera{};
        DirectionalLight directional_light{};
        std::vector<PointLight> point_lights{};
        std::vector<Object> objects{};
    };
}

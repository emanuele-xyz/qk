#pragma once

#include <qk/Math.h>
#include <qk/r3d/Mesh.h>
#include <qk/r3d/Texture.h>

#include <vector>

namespace qk::r3d
{
    enum class SceneTransparencyTechnique
    {
        Sorted,
        WeightedBlendedOIT,
        Count
    };

    struct SceneSettings
    {
        float gamma{ 2.2f };
        SceneTransparencyTechnique transparency{ SceneTransparencyTechnique::Sorted };
    };

    struct Background
    {
        Vector3 color{ 1.0f, 0.0f, 1.0f };
    };

    struct Camera
    {
        Vector3 eye{};
        Vector3 target{ 0.0f, 1.0f, +10.0f };
        Vector3 up{ 0.0f, 1.0f, 0.0f };
        float fov_deg{ 45.0f };
        float near_plane{ 0.01f };
        float far_plane{ 100.0f };
    };

    struct DirectionalLight
    {
        bool render_gizmos{ false };
        Vector3 direction{ -1.0f, -1.0f, -1.0f };
        Vector3 color{ 1.0f, 1.0f, 1.0f };
    };

    struct PointLight
    {
        bool render_gizmos{ false };
        Vector3 position{};
        Vector3 color{ 1.0f, 1.0f, 1.0f };
        float r_min{ 0.1f };
        float r_max{ 5.0f };
    };

    struct SpotLight
    {
        bool render_gizmos{ false };
        Vector3 position{ 1.0f, 1.0f, 1.0f };
        Vector3 direction{ 0.0f, -1.0f, 0.0f };
        Vector3 color{ 1.0f, 1.0f, 1.0f };
        float r_min{ 0.1f };
        float r_max{ 5.0f };
        float umbra_angle_deg{ 45.0f };
        float penumbra_angle_deg{ 30.0f };
    };

    struct Object
    {
        Vector3 position{};
        Vector3 rotation{};
        Vector3 scaling{ 1.0f, 1.0f, 1.0f };
        MeshID mesh_id{ CUBE };
        float albedo_mix{}; // 0 for color, 1 for texture
        Vector3 albedo_color{ 1.0f, 1.0f, 1.0f };
        TextureID albedo_id{ ALBEDO_BLACK };
        float opacity{ 1.0f }; // 0 for fully transparent, 1 for fully opaque
    };

    struct Scene
    {
        SceneSettings settings;
        Background background{};
        Camera camera{};
        DirectionalLight directional_light{};
        std::vector<PointLight> point_lights{};
        std::vector<SpotLight> spot_lights{};
        std::vector<Object> objects{};
    };
}

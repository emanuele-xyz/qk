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

    enum class LightType
    {
        Directional,
        Point,
        Spot,
        Count
    };

    struct Light
    {
        LightType type{ LightType::Directional };
        bool render_gizmos{ false };
        Vector3 position{};
        Vector3 direction{ 0.0f, -1.0f, 0.0f };
        Vector3 color{ 1.0f, 1.0f, 1.0f };
        float r_min{ 0.1f };
        float r_max{ 5.0f };
        float umbra_angle_deg{ 45.0f };
        float penumbra_angle_deg{ 30.0f };
    };

    enum class ShadingMode
    {
        Flat,
        Shaded,
        Count
    };

    enum class SamplerFilter
    {
        Nearest,
        Linear,
        Anisotropic,
        Count
    };

    enum class SamplerAddressMode
    {
        Wrap,
        Mirror,
        Clamp,
        MirrorOnce,
        Count
    };

    struct Sampler
    {
        SamplerFilter filter{ SamplerFilter::Nearest };
        SamplerAddressMode address_mode_u{ SamplerAddressMode::Wrap };
        SamplerAddressMode address_mode_v{ SamplerAddressMode::Wrap };
        int anisotropy{ 1 }; // from 1 to 16
    };

    struct Albedo
    {
        float mix{}; // 0 for color, 1 for texture
        Vector3 color{ 1.0f, 1.0f, 1.0f };
        TextureID id{ ALBEDO_BLACK }; // TODO: rename to texture
        Sampler sampler{};
        bool alpha_test{ false };
        float alpha_test_value{}; // all fragments with alpha < alpha_test are discarded
    };

    struct Opacity
    {
        float opacity{ 1.0f }; // 0 for fully transparent, 1 for fully opaque
        TextureID id{ TEXTURE_ID_INVALID }; // TODO: rename to alpha map
        Sampler sampler{};
        bool alpha_test{ false };
        float alpha_test_value{}; // all fragments with alpha < alpha_test are discarded
    };

    struct Object
    {
        ShadingMode shading_mode{ ShadingMode::Flat };
        Vector3 position{};
        Vector3 rotation{};
        Vector3 scaling{ 1.0f, 1.0f, 1.0f };
        MeshID mesh_id{ CUBE };
        Albedo albedo{};
        Opacity opacity{};
    };

    struct Scene
    {
        SceneSettings settings;
        Background background{};
        Camera camera{};
        std::vector<Light> lights{};
        std::vector<Object> objects{};
    };
}

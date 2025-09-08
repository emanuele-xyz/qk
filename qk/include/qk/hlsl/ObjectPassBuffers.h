#ifndef __QK_OBJECT_PASS_BUFFERS__
#define __QK_OBJECT_PASS_BUFFERS__

#define QK_OBJECT_PASS_OBJECT_CONSTANT_SHADING_MODE_FLAT 0
#define QK_OBJECT_PASS_OBJECT_CONSTANT_SHADING_MODE_SHADED 1

struct ObjectPassSceneConstants
{
    matrix view;
    matrix projection;
    int point_lights_count;
    int spot_lights_count;
    float gamma;
    float _pad[1];
};

struct ObjectPassObjectConstants
{
    matrix model;
    matrix normal;
    float3 albedo_color;
    float albedo_mix;
    float opacity;
    int shading_mode;
    float _pad0[2];
    struct
    {
        float3 direction;
        float _pad0;
        float3 color;
        float _pad1;
    } directional_light;
};

struct ObjectPassPointLight
{
    float3 world_position;
    float3 color;
    float r_min;
    float r_max;
};

struct ObjectPassSpotLight
{
    float3 world_position;
    float3 direction;
    float3 color;
    float r_min;
    float r_max;
    float umbra_rad;
    float penumbra_rad;
};

#endif

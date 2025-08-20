#ifndef __QK_OBJECT_PASS_BUFFERS__
#define __QK_OBJECT_PASS_BUFFERS__

struct ObjectPassSceneConstants
{
    matrix view;
    matrix projection;
    int point_lights_count;
    int spot_lights_count;
    float _pad[2];
};

struct ObjectPassObjectConstants
{
    matrix model;
    matrix normal;
    float3 albedo_color;
    float albedo_mix;
    float opacity;
    float3 _pad;
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

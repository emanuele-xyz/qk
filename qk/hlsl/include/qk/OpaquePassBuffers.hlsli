#ifndef __QK_OPAQUE_PASS_BUFFERS__
#define __QK_OPAQUE_PASS_BUFFERS__

struct OpaquePassSceneConstants
{
    matrix view;
    matrix projection;
    int point_lights_count;
    float _pad[3];
};

struct OpaquePassObjectConstants
{
    matrix model;
    matrix normal;
    float3 albedo_color;
    float albedo_mix;
    struct
    {
        float3 direction;
        float _pad0;
        float3 color;
        float _pad1;
    } directional_light;
};

struct OpaquePassPointLight
{
    float3 world_position;
    float3 color;
    float r_min;
    float r_max;
};

struct OpaquePassSpotLight
{
    float3 position;
    float3 direction;
    float3 color;
    float r_min;
    float r_max;
    float umbra_deg;
    float penumbra_deg;
};

#endif

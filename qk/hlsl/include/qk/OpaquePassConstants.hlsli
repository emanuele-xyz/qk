#ifndef __QK_OPAQUE_PASS_CONSTANTS__
#define __QK_OPAQUE_PASS_CONSTANTS__

struct OpaquePassSceneConstants
{
    matrix view;
    matrix projection;
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

#endif

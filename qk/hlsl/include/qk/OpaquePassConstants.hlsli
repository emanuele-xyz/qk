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
    float3 light_direction; // TODO: directional light direction (move it from here)
    float _pad0;
    float3 light_color; // TODO: directional light color (move it from here)
    float _pad1;
};

#endif

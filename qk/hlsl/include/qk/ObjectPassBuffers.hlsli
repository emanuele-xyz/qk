#ifndef __QK_OBJECT_PASS_BUFFERS__
#define __QK_OBJECT_PASS_BUFFERS__

#define QK_OBJECT_PASS_SHADING_MODE_FLAT 0
#define QK_OBJECT_PASS_SHADING_MODE_SHADED 1
#define QK_OBJECT_PASS_LIGHT_DIRECTIONAL 0
#define QK_OBJECT_PASS_LIGHT_POINT 1
#define QK_OBJECT_PASS_LIGHT_SPOT 2

struct ObjectPassSceneConstants
{
    matrix view;
    matrix projection;
    int lights_count;
    float _pad[3];
};

struct ObjectPassObjectConstants
{
    matrix model;
    matrix normal;
    float3 albedo_color;
    float albedo_mix;
    float opacity;
    float alpha_test_value;
    int alpha_test;
    int shading_mode;
};

struct ObjectPassLight
{
    int type;
    float3 world_position;
    float3 direction;
    float3 color;
    float r_min;
    float r_max;
    float umbra_rad;
    float penumbra_rad;
};

#endif

#ifndef __QK_OPAQUE_PASS__
#define __QK_OPAQUE_PASS__

#include <qk/OpaquePassConstants.hlsli>

struct VSInput
{
    float3 local_position : POSITION;
    float3 local_normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 clip_position : SV_POSITION;
    float3 world_normal : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer CBScene : register(b0)
{
    OpaquePassSceneConstants cb_scene;
};

cbuffer CBObject : register(b1)
{
    OpaquePassObjectConstants cb_object;
};

SamplerState albedo_sampler : register(s0);

Texture2D albedo_texture : register(t0);

#endif

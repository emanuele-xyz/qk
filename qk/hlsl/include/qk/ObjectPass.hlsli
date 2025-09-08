#ifndef __QK_OBJECT_PASS__
#define __QK_OBJECT_PASS__

#include <qk/ObjectPassBuffers.hlsli>
#include <qk/VSInput.hlsli>

struct VSOutput
{
    float4 clip_position : SV_POSITION;
    float3 world_position : POSITION;
    float3 world_normal : NORMAL;
    float2 uv : TEXCOORD;
};

cbuffer CBScene : register(b0)
{
    ObjectPassSceneConstants cb_scene;
};

cbuffer CBObject : register(b1)
{
    ObjectPassObjectConstants cb_object;
};

SamplerState albedo_sampler : register(s0);

Texture2D albedo_texture : register(t0);

StructuredBuffer<ObjectPassLight> sb_lights : register(t1);

#endif

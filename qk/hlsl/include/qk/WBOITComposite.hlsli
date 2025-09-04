#ifndef __QK_WBOIT_COMPOSITE__
#define __QK_WBOIT_COMPOSITE__

#include <qk/WBOITCompositeBuffers.hlsli>
#include <qk/VSInput.hlsli>

struct VSOutput
{
    float4 clip_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer CB : register(b0)
{
    WBOITCompositeConstants cb;
};

SamplerState sampler_accum_reveal : register(s0);

Texture2D texture_accum : register(t0);
Texture2D texture_reveal : register(t1);

#endif
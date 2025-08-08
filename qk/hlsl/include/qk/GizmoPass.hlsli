#ifndef __GIXMO_PASS__
#define __GIXMO_PASS__

#include <qk/GizmoPassBuffers.hlsli>

struct VSInput
{
    float3 local_position : POSITION;
};

struct VSOutput
{
    float4 clip_position : SV_POSITION;
};

cbuffer CBScene : register(b0)
{
    GizmoPassSceneConstants cb_scene;
};

cbuffer CBObject : register(b1)
{
    GizmoPassObjectConstants cb_object;
};

#endif

#ifndef __QK_VS_INPUT__
#define __QK_VS_INPUT__

struct VSInput
{
    float3 local_position : POSITION;
    float3 local_normal : NORMAL;
    float2 uv : TEXCOORD;
};

#endif

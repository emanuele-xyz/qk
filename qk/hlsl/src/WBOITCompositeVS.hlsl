struct VSInput
{
    float3 local_position : POSITION;
    float3 local_normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 clip_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    // we scale by 2 because the local space quad mesh has extremes (-0.5, -0.5, 0.0) and (+0.5, +0.5, 0.0)
    output.clip_position = float4(input.local_position * 2, 1.0);
    output.uv = input.uv;
    
    return output;
}


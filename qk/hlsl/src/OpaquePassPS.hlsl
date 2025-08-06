#include <qk/OpaquePass.hlsli>

float4 main(VSOutput input) : SV_TARGET
{
    float4 color = albedo_texture.Sample(albedo_sampler, input.uv);
    return color;
}

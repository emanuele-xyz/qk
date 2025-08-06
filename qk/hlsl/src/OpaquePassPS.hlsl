#include <qk/OpaquePass.hlsli>

float4 main(VSOutput input) : SV_TARGET
{
    float3 albedo = albedo_texture.Sample(albedo_sampler, input.uv).xyz;
    return float4(albedo, 1.0);
}

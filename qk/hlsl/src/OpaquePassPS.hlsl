#include <qk/OpaquePass.hlsli>

float4 main(VSOutput input) : SV_TARGET
{
    float3 albedo_color = cb_object.albedo_color;
    float3 albedo_sample = albedo_texture.Sample(albedo_sampler, input.uv).xyz;
    float3 albedo = lerp(albedo_color, albedo_sample, cb_object.albedo_mix);
    return float4(albedo, 1.0);
}

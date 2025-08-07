#include <qk/OpaquePass.hlsli>

float4 main(VSOutput input) : SV_TARGET
{
    float3 albedo_color = cb_object.albedo_color;
    float3 albedo_sample = albedo_texture.Sample(albedo_sampler, input.uv).xyz;
    float3 albedo = lerp(albedo_color, albedo_sample, cb_object.albedo_mix);
    
    // lighting model parameters
    float3 l = normalize(-cb_object.light_direction); // negate the directional light direction
    float3 n = normalize(input.world_normal);
    
    // Labert lighting model
    float3 shaded = albedo * cb_object.light_color * max(dot(n, l), 0);
    
    return float4(shaded, 1.0);
}

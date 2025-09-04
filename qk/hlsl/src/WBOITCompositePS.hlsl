#include <qk/WBOITComposite.hlsli>
#include <qk/GammaCorrect.hlsli>

float4 main(VSOutput input) : SV_TARGET
{
    float4 accum = texture_accum.Sample(sampler_accum_reveal, input.uv);
    float reveal = texture_reveal.Sample(sampler_accum_reveal, input.uv).x;
    
    float3 color = accum.rgb / clamp(accum.a, 1e-4, 5e4);
    color = gamma_correct_to_gamma(color, cb.gamma);
    
    return float4(color, reveal);
}

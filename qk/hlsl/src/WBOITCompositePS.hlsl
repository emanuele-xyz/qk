struct VSOutput
{
    float4 clip_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

SamplerState sampler_accum_reveal : register(s0);

Texture2D texture_accum : register(t0);
Texture2D texture_reveal : register(t1);

float4 main(VSOutput input) : SV_TARGET
{
    float4 accum = texture_accum.Sample(sampler_accum_reveal, input.uv);
    float reveal = texture_reveal.Sample(sampler_accum_reveal, input.uv).x;
    
    return float4(accum.rgb / clamp(accum.a, 1e-4, 5e4), reveal);
}

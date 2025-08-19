#include <qk/ObjectPass.hlsli>

VSOutput main(VSInput input)
{
    float4 world_position = mul(cb_object.model, float4(input.local_position, 1.0));
    
    VSOutput output;
    output.clip_position = mul(cb_scene.projection, mul(cb_scene.view, world_position));
    output.world_position = world_position.xyz;
    output.world_normal = normalize(mul(cb_object.normal, float4(input.local_normal, 0.0)).xyz);
    output.uv = input.uv;
	return output;
}

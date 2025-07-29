#include <qk/OpaquePass.hlsli>

VSOutput main(VSInput input)
{
    VSOutput output;
    output.clip_position = mul(cb_scene.projection, mul(cb_scene.view, mul(cb_object.model, float4(input.local_position, 1.0))));
    output.world_normal = mul(cb_object.normal, float4(input.local_normal, 0.0)).xyz;
    output.uv = input.uv;
	return output;
}

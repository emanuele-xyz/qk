#include <qk/GizmoPass.hlsli>

VSOutput main(VSInput input)
{
    float4 world_position = mul(cb_object.model, float4(input.local_position, 1.0));
    
    VSOutput output;
    output.clip_position = mul(cb_scene.projection, mul(cb_scene.view, world_position));
    return output;
}

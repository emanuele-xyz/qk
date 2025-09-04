#include <qk/WBOITComposite.hlsli>

VSOutput main(VSInput input)
{
    VSOutput output;
    // we scale by 2 because the local space quad mesh has extremes (-0.5, -0.5, 0.0) and (+0.5, +0.5, 0.0)
    output.clip_position = float4(input.local_position * 2, 1.0);
    output.uv = input.uv;
    
    return output;
}

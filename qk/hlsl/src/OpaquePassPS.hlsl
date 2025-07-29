#include <qk/OpaquePass.hlsli>

float4 main(VSOutput input) : SV_TARGET
{
	return float4(input.uv, 0.0f, 1.0f);
}

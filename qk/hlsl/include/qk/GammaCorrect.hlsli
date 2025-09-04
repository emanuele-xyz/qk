#ifndef __QK_GAMMA_CORRECT__
#define __QK_GAMMA_CORRECT__

float3 gamma_correct_to_linear(float3 color, float gamma)
{
    return pow(color, gamma);
}
float3 gamma_correct_to_gamma(float3 color, float gamma)
{
    return pow(color, 1 / gamma);
}

#endif
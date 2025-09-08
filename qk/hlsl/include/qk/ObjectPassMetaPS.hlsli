#ifndef __QK_OBJECT_PASS_META_PS__
#define __QK_OBJECT_PASS_META_PS__

#include <qk/ObjectPass.hlsli>
#include <qk/GammaCorrect.hlsli>

/*
    Lambert's illumination model
*/
float3 lambert(float3 l, float3 n, float3 c_light, float3 c_surface)
{
    return c_surface * c_light * max(dot(n, l), 0);
}

/*
    Punctual light distance falloff windowing function
    r: distance between the shaded point and the light source
    r_max: maximum distance between the shaded point and the light source
*/
float f_win(float r, float r_max)
{
    float r_over_r_max = r / r_max;
    float r_over_r_max_fourth = pow(r_over_r_max, 4);
    float clamped = max(0, 1 - r_over_r_max_fourth);
    float clamped_squared = clamped * clamped;
    return clamped_squared;
}

/*
    Punctual light distance falloff function
    r: distance between the shaded point and the light source
    r_min: radius of the physical object emitting the light
    r_max: maximum distance between the shaded point and the light source
*/
float f_dist(float r, float r_min, float r_max)
{
    r = max(r, r_min);
    float one_over_r = 1 / r;
    float one_over_r_squared = one_over_r * one_over_r;
    return f_win(r, r_max) * one_over_r_squared;
}

/*
    Spot light directional falloff function
    l: normalized vector going from the shaded point to the light source
    s: spot light direction vector
    theta_u: umbra angle (rad)
    theta_p: penumbra angle (rad)
*/
float f_dir(float3 l, float3 s, float theta_u, float theta_p)
{
    float cos_theta_s = dot(-l, s);
    float v = (cos_theta_s - cos(theta_u)) / (cos(theta_p) - cos(theta_u));
    float t = clamp(v, 0, 1);
    return t * t;
}

struct PSOutput
{
    float4 color : SV_TARGET0;
#ifdef QK_OBJECT_PASS_WBOIT
    float reveal : SV_TARGET1;
#endif
};

PSOutput main(VSOutput input)
{
    float3 albedo = float3(0, 0, 0);
    {
        float3 albedo_color = cb_object.albedo_color;
        albedo_color = gamma_correct_to_linear(albedo_color, cb_scene.gamma);
        
        float3 albedo_sample = albedo_texture.Sample(albedo_sampler, input.uv).xyz;
        albedo_sample = gamma_correct_to_linear(albedo_sample, cb_scene.gamma);
        
        albedo = lerp(albedo_color, albedo_sample, cb_object.albedo_mix);
    }
    
    float3 n = normalize(input.world_normal);
    
    float3 shaded = float3(0, 0, 0);
    
    if (cb_object.shading_mode == QK_OBJECT_PASS_SHADING_MODE_FLAT)
    {
        shaded = albedo;
    }
    else if (cb_object.shading_mode == QK_OBJECT_PASS_SHADING_MODE_SHADED)
    {
        for (int i = 0; i < cb_scene.lights_count; i++)
        {
            ObjectPassLight light = sb_lights[i];
            
            if (light.type == QK_OBJECT_PASS_LIGHT_DIRECTIONAL)
            {
                float3 l = normalize(-light.direction); // negate the directional light direction
                float3 c_light = light.color;
                c_light = gamma_correct_to_linear(c_light, cb_scene.gamma);
                shaded += lambert(l, n, c_light, albedo);
            }
            else if (light.type == QK_OBJECT_PASS_LIGHT_POINT)
            {
                float3 d = light.world_position - input.world_position;
                float r = length(d);
                float3 l = d / r;
                float3 c_light = light.color;
                c_light = gamma_correct_to_linear(c_light, cb_scene.gamma);
                {
                    float r_min = light.r_min;
                    float r_max = light.r_max;
                    c_light *= f_dist(r, r_min, r_max); // apply distance falloff function
                }
                shaded += lambert(l, n, c_light, albedo);
            }
            else if (light.type == QK_OBJECT_PASS_LIGHT_SPOT)
            {
                float3 d = light.world_position - input.world_position;
                float r = length(d);
                float3 l = d / r;
                float3 c_light = light.color;
                c_light = gamma_correct_to_linear(c_light, cb_scene.gamma);
                {
                    float r_min = light.r_min;
                    float r_max = light.r_max;
                    c_light *= f_dist(r, r_min, r_max); // apply distance falloff function
                }
                {
                    float3 s = normalize(light.direction);
                    float theta_u = light.umbra_rad;
                    float theta_p = light.penumbra_rad;
                    c_light *= f_dir(l, s, theta_u, theta_p); // apply directional falloff function
                }
                shaded += lambert(l, n, c_light, albedo);
            }
        }
    }
    
    PSOutput output;
    
#ifdef QK_OBJECT_PASS_WBOIT
    {
        // NOTE: weight function from https://casual-effects.blogspot.com/2015/03/implemented-weighted-blended-order.html
        float ai = cb_object.opacity;
        float zi = input.clip_position.z;
        float weight = clamp(pow(min(1.0, ai * 10.0) + 0.01, 3.0) * 1e8 * pow(1.0 - zi * 0.9, 3.0), 1e-2, 3e3);
    
        output.color = float4(shaded * cb_object.opacity, cb_object.opacity) * weight;
        output.reveal = cb_object.opacity;
    }
#else
    {
        shaded = gamma_correct_to_gamma(shaded, cb_scene.gamma);
        output.color = float4(shaded, cb_object.opacity);
    }
#endif

    return output;
}

#endif

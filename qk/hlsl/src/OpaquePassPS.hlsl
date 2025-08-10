#include <qk/OpaquePass.hlsli>

/*
    Lambert's illumination model
*/
float3 lambert(float3 l, float3 n, float3 c_light, float3 c_surface)
{
    return c_surface * c_light * max(dot(n, l), 0);
}

/*
    Windowing function
*/
float f_win(float x, float x_max)
{
    float x_over_x_max = x / x_max;
    float x_over_x_max_fourth = pow(x_over_x_max, 4);
    float clamped = max(0, 1 - x_over_x_max_fourth);
    float clamped_squared = clamped * clamped;
    return clamped_squared;
}

/*
    Light distance falloff function
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

float4 main(VSOutput input) : SV_TARGET
{
    float3 albedo = float3(0, 0, 0);
    {
        float3 albedo_color = cb_object.albedo_color;
        float3 albedo_sample = albedo_texture.Sample(albedo_sampler, input.uv).xyz;
        albedo = lerp(albedo_color, albedo_sample, cb_object.albedo_mix);
    }
    
    float3 n = normalize(input.world_normal);
    
    float3 shaded = float3(0, 0, 0);
    
    // Lambert for the directional light
    {
        float3 l = normalize(-cb_object.directional_light.direction); // negate the directional light direction
        shaded += lambert(l, n, cb_object.directional_light.color, albedo);
    }

    // Lambert for the point lights
    for (int i = 0; i < cb_scene.point_lights_count; i++)
    {
        OpaquePassPointLight point_light = sb_point_lights[i];
            
        float3 d = point_light.world_position - input.world_position;
        float r = length(d);
        float3 l = d / r;
        float3 c_light = point_light.color * f_dist(r, point_light.r_min, point_light.r_max);
        shaded += lambert(l, n, c_light, albedo);
    }
    
    // Lambert for the spot lights
    #if 0
    for (int i = 0; i < cb_scene.spot_lights_count; i++)
    {
        OpaquePassSpotLight spot_light = sb_spot_lights[i];
        
        float3 d = spot_light.world_position - input.world_position;
        float r = length(d);
        float3 l = d / r;
        shaded += (l, n, c_)
    }
    #endif
    
    return float4(shaded, 1.0);
}

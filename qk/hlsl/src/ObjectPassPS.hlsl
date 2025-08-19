#include <qk/ObjectPass.hlsli>

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
        float3 c_light = cb_object.directional_light.color;
        shaded += lambert(l, n, c_light, albedo);
    }

    // Lambert for the point lights
    {
        for (int i = 0; i < cb_scene.point_lights_count; i++)
        {
            ObjectPassPointLight point_light = sb_point_lights[i];
            
            float3 d = point_light.world_position - input.world_position;
            float r = length(d);
            float3 l = d / r;
            float3 c_light = point_light.color;
            {
                float r_min = point_light.r_min;
                float r_max = point_light.r_max;
                c_light *= f_dist(r, r_min, r_max); // apply distance falloff function
            }
            shaded += lambert(l, n, c_light, albedo);
        }
    }
    
    // Lambert for the spot lights
    {
        for (int i = 0; i < cb_scene.spot_lights_count; i++)
        {
            ObjectPassSpotLight spot_light = sb_spot_lights[i];
        
            float3 d = spot_light.world_position - input.world_position;
            float r = length(d);
            float3 l = d / r;
            float3 c_light = spot_light.color;
            {
                float r_min = spot_light.r_min;
                float r_max = spot_light.r_max;
                c_light *= f_dist(r, r_min, r_max); // apply distance falloff function
            }
            {
                float3 s = normalize(spot_light.direction);
                float theta_u = spot_light.umbra_rad;
                float theta_p = spot_light.penumbra_rad;
                c_light *= f_dir(l, s, theta_u, theta_p); // apply directional falloff function
            }
            shaded += lambert(l, n, c_light, albedo);
        }
    }
    
    return float4(shaded, 1.0);
}

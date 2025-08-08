#include <qk/OpaquePass.hlsli>

/*
    Lambert's illumination model
*/
float3 lambert(float3 l, float3 n, float3 c_light, float3 c_surface)
{
    return c_surface * c_light * max(dot(n, l), 0);
}

/*
    Compute the light's color based on distance
    r: distance between the shaded point and the light source
    c_light_0: c_light at distance r_0
*/
float3 c_light(float r, float r_0, float3 c_light_0)
{
    return c_light_0 * (r_0 / r) * (r_0 / r);
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
    
    float3 shaded = float3(0, 0,  0);
    
    // Lambert for the directional light
    {
        float3 l = normalize(-cb_object.directional_light.direction); // negate the directional light direction
        shaded += lambert(l, n, cb_object.directional_light.color, albedo);
    }

    // Lambert for the point lights
    {
        for (int i = 0; i < cb_scene.point_lights_count; i++)
        {
            float3 d = sb_point_lights[i].world_position - input.world_position;
            float r = length(d);
            float3 l = d / r;
            shaded += lambert(l, n, c_light(r, 1, sb_point_lights[i].color), albedo);
        }
    }
    
    return float4(shaded, 1.0);
}

# Notes

Notes taken from the Real Time Rendering Book 4th edition about the various techniques implemented by the renderer.

## Shading Basics

Lambert's shading model

`c_shaded = (l dot n) * c_light * c_surface`

### Directional Lights

Directional lights:
- have `l` and `c_light` constants.
- have no position.

We can use a box to represent the volume of influence of a directional light. When one of such boxes is inside another one, we transition, doing an interpolation, between the old and the new directional light. We may even set one directional light as global. In this way we don't have to set a box for it.

### Punctual Lights

Punctual lights:
- have location.
- the light source is a point in space.
- are called point lights when they emit light omnidirectionally around the point.
- `l` is the unit vector going from the shaded point toward the light's location.
- `r` is the distance between the shaded point and the light's location.

#### Point/Omni Lights

- `c_light` varies with `r`.
- Tthe light's color is a funciton of distance `c_light(r) = c_light_0 * (r_0 / r)^2`.
- `c_light_0` is the value of `c_light` at distance `r_0`.
- To avoid a divide by zero Unreal uses, `c_light(r) = c_light_0 * r_0^2 / (r^2 + epsylon)` with `epsylon = 1cm`.
- CryEngine and Frostbite use `c_light(r) = c_light_0 * (r_0 / max(r, r_min))^2`.
- `r_min` is the radius of the physical object emitting the light.
- For efficient rendering, the light's intensity should reach zero at some finite distance (useful for culling).
- To avoid sharp cutoffs, it would be advisable for the intensity and its derivative to reach zero at the same distance.
- We multiply the inverse square equation by a windowing function `f_win(r)`.
- Unreal and Frostbite use `f_win(r) = max((1 - (r / r_max)^4), 0)^2`.
- The general representation is `c_light(r) = c_light_0 * f_dist(r)`.
- `f_dist(r)` is called a distance falloff function.

#### Spot Lights

- illumination varies both by distance and direction.
- `c_light(r) = c_light_0 * f_dist(r) * f_dir(l)`.
- `f_dir(l)` is called a directional falloff function.
- spot lights project light in a circular cone.
- `s` is the spot light direction vector.
- the distance falloff function can be expressed by the angle `theta_s` between `-l` and `s`
- `theta_u` is the umbra angle (useful for culling).
- `f_dir(l) = 0` for all `theta_s >= theta_u`
- `theta_p` is the penumbra angle
- the penumbra angle defines an inner cone where the light is at its full intensity.
- let `t = clamp((cos theta_s - cos theta_u) / (cos theta_p - cos theta_u), 0, 1)`
- `f_dir(r) = smothstep(t)`

### Other Light Types

- Capsule lights use as light source a segment. For each shaded point, the direction to the closest point on the segment is used as the ligt direction `l`.
- Area lights are light sources with size and shape. They illuminate surface points from multiple directions.
- There are two cathegories of area light tecnhiques.
- Those that soften the edges of stahdows, resulting from the area light being partially occluded.
- Those that simulate the effect of the area light on surface shading.

## Implementing Shading Models

### Material Systems

- Material: all the data used to describe the appearance of a surface.
- Idea: implement a material LOD model: for each LOD, possibly use a different material.
- The idea is to use simpler versions of the base material, the further away the object is from the camera.

## Transparency, Alpha and Compositing

- Screen-door transparency: discard some pixels based on a checker pattern. (Order independent, All objects are opaque) (Too limited)
- Stochastic transparency: uses subpixel screen-door masks. (Noisy, needs a lot of samples to look good)
- Blending the transparent object's color with the color of the object behind using alpha blending.
- A pixel's alpha may represent either opacity, coverage of samples, or both, depending on the circumstances.

### Blending Order

- Transparent objects are rendered on top of the existing scene with an alpha of less then 1.
- We blend a fragment's value with the corresponding color in the color buffer using the over operator.
- `c_o = alpha_s * c_s + (1 - alpha_s) * c_d`.
- `c_s` is the color of the transparent object, the source.
- `alpha_s` is the transparent object's alpha.
- `c_d` is the pixel color before blending, the destination.
- `c_o` is the pixel color after  blending.

- There is also additive blending.
- `c_o = alpha_s * c_s + c_d`.

- To render transparent objects we need to draw them after the opaque ones.
- After rendering the opaque objects, we render the transparent ones with the over operator.
- Using the over operator, we need to render transparent objects back to front.
- Objects that interpenetrate cause artifacts.
- It is usually best to turn off writes to the depth buffer when performing transparency.
- In this way, all transparent objects will appear in some form, instead of flickering when their draw order changes.
- We may also render each transparent mesh twice, one draw for the back-faces and another for the front-faces.

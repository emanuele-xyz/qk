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

- If we want to render front to back, we use the under operator.
- `c_o = alpha_d * c_d + (1 - alpha_d) * alpha_s * c_s`.
- `alpha_o = alpha_s * (1 - alpha_d) + alpha_d = alpha_s - alpha_s * alpha_d + alpha_d`.
- The formula for computing alpha is order independent.

- The under operator is used by rendering all transparent objects to a separate buffer and then merging it with the main color buffer.

### Order independent Transparency

- The under operator is used for the order-independent transparency (OIT) algorithm known as depth peeling.
- Order independent = No sorting.
- Another OIT technique is the A-buffer: liked lists of fragments, for every pixel.
- Another OIT technique is multi-layer alpha blending.
- Weighted sum and weighted average are OIT techniques using a single pass. Problem: the order of the objects is not taken into account. Works well for highly transparent surfaces and particles.
- Weighted average is usually preferred.
- `c_sum = sum for i=1..n of alpha_i * c_i`. Result of the color buffer used for rendering only transparent surfaces.
- `alpha_sum = sum for i=1..n of alpha_i`. Result of the alpha buffer used for rendering only transparent surfaces.
- `c_wavg = c_sum / alpha_sum`. Weighted average transparency color.
- `alpha_avg = alpha_sum / n`. Average of all alpha values.
- `u = (1 - alpha_avg)^n`. Estimated visibility of the destination (the opaque scene) after this average alpha is applied `n` times, for `n` transparent surfaces.
- `c_o = (1-u) * c_wavg + u * c_d`. This is the over operator with `1-u` being the source's alpha.
- `n` is the number of transparent objects.
- `c_i` and `alpha_i`represent the ser of transparency values.
- `c_d` is the color of the opaque portion of the scene.
- Weighted average, for identical alphas, blends all colors equally, regardless of order.
- Weighted blended OIT was introduced to give a more convincing result. The distance to the surface also affects the weight, with closer surfaces being given more influence. `u` is also computed by multiplying together the terms `1-alpha_i` together and subtracting from one, giving the true alpha coverage of the set of surfaces.
- McGuire and Mara extend this method.

## Display Encoding

- take shader output colors and raise them by a power of `1/gamma`.
- take shader input colors and raise them by a power of `gamma`.
- usaually `gamma = 2.2`

# Texturing

## Image Texturing

### Magnification

- Magnification happens when there are more fragments sampling the texture than texels in the texture.
- Solution: filtering; two most common solutions are nearest and bilinear (sometimes called linear) filtering.

### Minification

- Minification happens when there are less fragments sampling the texture than texels in the texture.
- This means that we are undersampling the texture: we suffer from aliasing.
- Most common solution: mipmapping.
- Mipmapping builds a chain of mip levels. Level zero is the original texture.
- Level n+1 is built starting from level n, halving its resolution, averaging the colors of all pixels of each 2x2 block of level n into a single pixel of level n+1.
- The GPU selects the mipmap level whose number of texels is closest to the number of fragments sampling the texture.
- To build mip levels we can also use gaussiam, lanczos, kaiser or similar filters.
- If we have sRGB textures, we first must map colors to linear space, build the mip chain, and then convert each level into sRGB space.
- The most common filtering method used with mipmaps is trilinear interpolation.
- One problem of mipmapping is overblurring.
- This is evident looking at a mipmapped textured surface at a grazing viewing angle.
- An alternative to mipmapping is using summed area tables (which is anisotropic) (still suffers from overblurring).
- To fix overblurring, anisotropic filtering is used.

### Volume Textures

- Volume textures are 3d textures, sampled using a set of 3 texture coordinates.
- A volumetric light can be represented as a 3d texture. It assigns an amount of light to each point in the volume.
- Mipmapping is supported.
- We can use 3d vertex coordinates to directly sample a 3d texture.

### Cube Maps

- A cube map is made up of six textures, one for each side of a cube.
- A cube map is sampled using a 3d direction vector.
- We shoot a ray from the center of the cube along the passed direction and we find the intersection point.
- The result of sampling is the color at the found intersection point.
- Most commonly used for environment mapping.

### Texture Representation

- Switching textures is inefficient for the graphics pipeline.
- We can put several textures into a single larger texture, called a texture atlas.
- We need to take special care when generating mipmaps for texture atlases.
- Wrap, repeat and mirroring modes when sampling a texture atlas may cause problems.
- A simpler solution to texture atlases are texture arrays. Texture arrays avoid all the problems of texture atlases.
- All textures of a texture array must have the same dimensions. This wasn't the case for texture atlases.
- We can also use bindless textures, to avoid having to explicitly bind textures when rendering.

### Texture Compression

- GPUs can decode on the fly block compressed textures.
- Block compression is lossy.
- There are various block compression formats, depending on the need.
- Compression of normal maps require some care.
- Compressed formats designed for RGB colors do not work well for normals.
- Most approaches suppose the normal to be unit length and that its z component is positive.
- This allows for only storing the x and y components of a normal.
- `z = sqrt(1 - x^2 - y^2)`
- We can then compress the x and y components texture using BC5.

## Procedural Texturing

- Procedural textures make good volume textures.
- Noise is an essential component of procedural texturing.

## Texture Animation

- We can offset the uvs of a mesh to animate a texture over time/space.
- We can apply a matrix to the uvs to get zoom, rotation, shearing and so on.
- We can also blend between textures.

## Material Mapping

- We can use textures to modify the properties of the surface material.
- We can use a texture to change the surface color (this is called albedo color map or diffuse color map).
- We can also use a texture to change the shading equation we are evaluating for a surface point.
- Textures having linear inputs (color) to the final pixel color can be filtered normally.
- Textures having non-linear inputs (roughness, bump) to the final pixel color require care.

## Alpha Mapping

- We can use alpha blending/testing to implement many effects.
- Some notable effects are foliage, explosions and distant objects.
- Another notable texture effect is decaling.
- This consists into placing a texture onto an object.
- Using textures with alpha, texels with alpha equal to 0 have no effect.
- Typically,a clamp corresponder function is used with a transparent border to apply a single copy of the decal to the surface.
- Another application of alpha is making cutouts.
- We can use a texture for a brush and render it mapped to a textured quad. In this way we have a bush in our scene.
- We render an object with a complex silouette just using a quad.
- If the viewer moves, the bush rendered in this way may appear flat.
- To fix this issue, we may place another bush quad, perpendicular to the first.
- This is what is called a cross tree.
- Another solution to fix the quad bush issue is to use billboarding.
- Combining alpha maps and texture animation can be used for effects such as: flickering torches, plant growth, explosions and atmospheric effects.
- We can use alpha blending to render objects with an alpha map.
- This requires rendering the transparent objects after the opaque ones, from back to front. (NOTE: can we use an OIT technique?)
- If we don't want to use alpha bledning, because we don't want to do the sorting, we can use alpha testing.
- Alpha testing consists in discarding fragments with alpha values below a given threshold in the pixel shader.
- `if (texture.alpha < alpha_threshold) discard`.
- For cutouts, we set the threshold to a value higher than 0, usually 0.5, and don't perform alpha blending at all for those fragments with alpha different from 1 and higher than the threshold.
- Doing so avoids out of order artifacts, but the quality of the rendering is low.
- Another solution is to perform two passes: one for solid cutouts, written to the z buffer, and one for semitransparent samples. (NOTE: how?)
- Problems arise when alpha maps are minified/magnified.
- For example, when mip maps are generated, alpha values are averaged, yielding undesired results.
- This problem can be fixed by using another way to mix alphas when computing the mip chain for alpha maps.
- Another way to fix this is to scale the alphas read from the alpha map accordingly.
- Another way is to alpha test the texel's alpha against a random alpha value. This is a form of stochastic transparency.
- In practice, the random function is replaced with a hash function to avoid temporal and spatial noise.
- Alpha testing produces ripple artifacts under magnification.
- This can be fixed by representing the alpha map as an SDF.
- When computing mip chains for RGBA textures using the standard way yields values with premultiplied alphas.
- If you treat that as an unmultiplied RGBA color, then you get artifacts on the fringe between opaque and transparent texels.
- The best strategy is to alpha premultiply before generating the mipmaps.

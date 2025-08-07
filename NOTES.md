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

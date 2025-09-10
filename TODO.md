# TODOs

[]: build fullscreen quad mesh and use it for WBOIT composite pass
[]: implement table data structure for textures and meshes, instead of using raw std::vectors
[]: repeated code when getting samplers for rendering objects both for the opaque and the wboit transparency pass
[]: log the amount of samplers stored into the samplers cache
[]: make a function for building a D3D11_SAMPLER_DESC from a user facing description of a sampler
[]: remove border and border color from samplers since they are mostly useless to users and they fatten up too much the sampler cache

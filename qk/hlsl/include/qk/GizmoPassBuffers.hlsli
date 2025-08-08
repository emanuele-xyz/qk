#ifndef __GIZMO_PASS_BUFFERS__
#define __GIZMO_PASS_BUFFERS__

struct GizmoPassSceneConstants
{
    matrix view;
    matrix projection;
};

struct GizmoPassObjectConstants
{
    matrix model;
    float3 color;
    float _pad;
};

#endif

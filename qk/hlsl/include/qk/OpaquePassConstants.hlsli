#ifndef __QK_OPAQUE_PASS_CONSTANTS__
#define __QK_OPAQUE_PASS_CONSTANTS__

struct OpaquePassSceneConstants
{
    matrix view;
    matrix projection;
};

struct OpaquePassObjectConstants
{
    matrix model;
    matrix normal;
};

#endif

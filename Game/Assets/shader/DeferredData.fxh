#include "Helper.fxh"

#ifndef DEFERRED_DATA_REGISTER_NUM
#define DEFERRED_DATA_REGISTER_NUM 0
#endif

#ifndef DEFERRED_DATA
cbuffer DeferredCB:register(REGISTER_B(DEFERRED_DATA_REGISTER_NUM))
{
    float4x4 def_mvpi;
    float4 def_camPos;
    float4 def_camDir;
    float def_camNear;
    float def_camFar;
}
#define DEFERRED_DATA
#endif
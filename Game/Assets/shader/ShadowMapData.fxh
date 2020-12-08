
#ifndef SHADOWMAP_DATA_REGIST
#define SHADOWMAP_DATA_REGISTER 3
#endif

cbuffer ShadowMapData:register(b SHADOWMAP_DATA_REGISTER)
{
    float4x4 mLVP[3];
    float4 offset;
    float4 pixSize;
}
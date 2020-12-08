#include "Helper.fxh"

#ifndef SHADOWMAP_DATA_REGISTER
#define SHADOWMAP_DATA_REGISTER 3
#endif

#ifndef MAX_SHADOWMAP_NUM
#define MAX_SHADOWMAP_NUM 3
#endif

#ifndef SHADOWMAP_1
#define SHADOWMAP_1 8
#endif

#ifndef SHADOWMAP_2
#define SHADOWMAP_2 9
#endif

#ifndef SHADOWMAP_3
#define SHADOWMAP_3 10
#endif



cbuffer ShadowMapData:register(REGISTER_B(SHADOWMAP_DATA_REGISTER))
{
    float4x4 shadowMap_mLVP[MAX_SHADOWMAP_NUM];
    float4 shadowMap_offset;
    float2 shadowMap_pixSize[MAX_SHADOWMAP_NUM];
}


Texture2D<float1> g_shadowMap_1      : register(REGISTER_T(SHADOWMAP_1));
Texture2D<float1> g_shadowMap_2      : register(REGISTER_T(SHADOWMAP_2));
Texture2D<float1> g_shadowMap_3      : register(REGISTER_T(SHADOWMAP_3));
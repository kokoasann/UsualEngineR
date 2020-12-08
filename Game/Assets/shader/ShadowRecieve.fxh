
#include "ShadowMapData.fxh"
#include "G_Sampler.fxh"

void GetShadow(float3 worldPos, int ind, Texture2D<float1> shadowMap,out bool isInUV,out bool isShadow)
{
    float4 lvpPos = mul(shadowMap_mLVP[ind],float4(worldPos,1.f));
    lvpPos.xyz *= rcp(lvpPos.w);
    float lvpDepth = lvpPos.z;

    float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

    isInUV = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));

    float depth = shadowMap.Sample(g_sampler,smuv);

    isShadow = 1.f - step(lvpDepth,depth+shadowMap_offset[ind]);
}

// #define GetShadow(worldPos,ind,shadowMapTex,isInUV,isShadow) \
// {\
//     float4 lvpPos = mul(shadowMap_mLVP[ind],float4(worldPos,1.f));\
//     lvpPos.xyz *= rcp(lvpPos.w);\
//     float lvpDepth = lvpPos.z;\
// \
//     float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));\
// \
//     isInUV = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));\
// \
//     float depth = ##shadowMapTex.Sample(g_sampler,smuv);\
// \
//     isShadow = 1.f - step(lvpDepth,depth+shadowMap_offset[ind]);\
// }\

float ShadowRecieve(float3 worldPos)
{
    bool isInUV = 0.f,isShadow = 0.f;
    //GetShadow(worldPos, 0, g_shadowMap_1, isInUV, isShadow);
    if(isInUV)
    {
        return isShadow;
    }

    // GetShadow(worldPos, 1, g_shadowMap_2, isInUV, isShadow);
    // if(isInUV)
    // {
    //     return isShadow;
    // }

    // GetShadow(worldPos, 2, g_shadowMap_3, isInUV, isShadow);
    // if(isInUV)
    // {
    //     return isShadow;
    // }

    return 0.f;
}
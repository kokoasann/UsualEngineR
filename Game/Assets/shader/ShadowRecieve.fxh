
#include "ShadowMapData.fxh"
#include "GBufferData.fxh"
#include "DeferredData.fxh"
#include "G_Sampler.fxh"
#include "Math.fxh"

//ビュー空間でのZに変換。
float GetViewZ(float depth)
{
    return (def_camFar*def_camNear)/((def_camFar-def_camNear)*depth-def_camFar);
}


float2 g_framePixSize = float2(1.f/1280.f, 1.f/720.f);

void GetShadow(float3 worldPos, int ind, Texture2D<float1> shadowMap,out bool isInUV,out bool isShadow)
{
    float4 lvpPos = mul(shadowMap_mLVP[ind],float4(worldPos,1.f));
    lvpPos.xyz *= rcp(lvpPos.w);
    float lvpDepth = lvpPos.z;

    float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

    isInUV = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));

    float depth = shadowMap.Sample(g_sampler,smuv);

    isShadow = 1.f - step(lvpDepth,depth+shadowMap_offset[ind]);
    //isShadow = 1.f - step(lvpDepth,depth);
    //isShadow = step(depth,lvpDepth);
}

int GetShadowMapIndex(float viewZ)
{
    [unroll(3)]
    for(int i=0;i<3;i++)
    {
        if(viewZ < shadowMap_areaDepthInView[i])
            return i;
    }
    return -1;
}

float FetchShadow(float3 worldPos)
{
    bool isInUV = 0.f,isShadow = 0.f;
    GetShadow(worldPos, 0, g_shadowMap_1, isInUV, isShadow);
    if(isInUV)
    {
        return isShadow;
    }

    GetShadow(worldPos, 1, g_shadowMap_2, isInUV, isShadow);
    if(isInUV)
    {
        return isShadow;
    }

    GetShadow(worldPos, 2, g_shadowMap_3, isInUV, isShadow);
    if(isInUV)
    {
        return isShadow;
    }

    return 0.f;
}

float PCF(
    float3 worldPos,
    float2 uv,
    float radius,
    Texture2D<float1> shadowMap,
    float ScreenOffset,
    int ind
)
{
    float4 lvpPos = mul(shadowMap_mLVP[ind],float4(worldPos,1.f));
    lvpPos.xyz *= rcp(lvpPos.w);
    float lvpDepth = lvpPos.z;

    float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

    bool isInUV = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));
    if(!isInUV)
        return 0.f;

    float weight[6][6] = {
        {.125, .05, 0.1, 0.1, .05, .0125},
        {.05, 0.2, 0.4, 0.4, 0.2, .05},
        {0.1, 0.4, 0.8, 0.8, 0.4, 0.1},
        {0.1, 0.4, 0.8, 0.8, 0.4, 0.1},
        {.05, 0.2, 0.4, 0.4, 0.2, .05},
        {.125, .05, 0.1, 0.1, .05, .0125}
    } ;


    float sum = 0.f;
    float count = 0;
    int k = 6;

    [unroll]
    for(int i=0; i<k; i+=1)
    {
        [unroll]
        for(int j=0; j<k; j+=1)
        {
            //float2 suv = uv + g_framePixSize*float2(i,j)*radius;
            //float depth = g_GDepth.Sample(g_sampler, suv);

            // float3 pos = GetWorldPosition(suv,depth,def_mvpi);
            // sum += FetchShadow(pos);
            // count++;

            float2 suv = smuv + float2(i-3,j-3) * (ScreenOffset * radius);
            float shadowDepth = shadowMap.Sample(g_sampler,suv);
            float isShadow = 1.f - step(lvpDepth,shadowDepth+shadowMap_offset[ind]);
            sum += isShadow * weight[i][j];
            count += weight[i][j];
        }
    }
    sum *= rcp(float(count));
    return sum;
}

float PCSS(
    float3 worldPos,
    float2 uv,
    Texture2D<float1> shadowMap,
    float ScreenOffset,
    int ind
)
{
    float4 lvpPos = mul(shadowMap_mLVP[ind],float4(worldPos,1.f));
    lvpPos.xyz *= rcp(lvpPos.w);
    float lvpDepth = lvpPos.z;

    float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

    bool isInUV = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));
    if(!isInUV)
        return 0.f;

    float blocker = shadowMap.Sample(g_sampler,smuv);

    return PCF(worldPos, uv, ((lvpDepth-blocker)/blocker)*150.f, g_shadowMap_1, 1./1024., ind);

    return 0.f;
}

float ShadowRecieve(float3 worldPos, float2 uv,float depth)
{
    float viewZ = GetViewZ(depth);
    int ind = GetShadowMapIndex(abs(viewZ));
    if(ind == 0)
    {
        return PCSS(worldPos, uv, g_shadowMap_1, 1./1024., ind);
    }
    else if(ind == 1)
    {
        return PCF(worldPos, uv, 0.2f, g_shadowMap_2, 1./512., ind);
    }
    else if(ind == 2)
    {
        return PCF(worldPos, uv, 0.2f, g_shadowMap_3, 1./256., ind);
    }
    //return FetchShadow(GetWorldPosition(uv,g_GDepth.Sample(g_sampler, uv),def_mvpi));
    return 0.f;
}

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
int GetShadowMapIndex(float3 worldPos)
{
    [unroll]
    for(int i=0;i<3;i++)
    {
        float4 lvpPos = mul(shadowMap_mLVP[i],float4(worldPos,1.f));
        lvpPos.xyz *= rcp(lvpPos.w);
        float lvpDepth = lvpPos.z;

        float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

        bool isInUV = (step(smuv.x,0.95f)) * (step(smuv.y,0.95f)) * (step(0.05f,smuv.x)) * (step(0.05f,smuv.y));
        if(isInUV)
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
    //if(!isInUV)
        //return 0.f;

    //gauss modoki
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

            float2 suv = smuv + (float2(i-3,j-3)) * (ScreenOffset * radius);
            float shadowDepth = shadowMap.Sample(g_sampler,suv);
            float isShadow = 1.f - step(lvpDepth,shadowDepth+shadowMap_offset[ind]);
            sum += isShadow * weight[i][j];
            count += weight[i][j];
        }
    }
    sum *= rcp(float(count));
    return sum;
}

float GetViewZ_L(float depth)
{
    return (100000.f*0.00001f)/((100000.f-0.00001f)*depth-100000.f);
}
float PCSS(
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

    //float blocker = shadowMap.Sample(g_sampler,smuv);
    //float isShadow = 1.f - step(lvpDepth,blocker+shadowMap_offset[ind]);

    
    float rad = radius;
    float d45 = rad * .7071067f;
    float2 p[9] = 
    {
        smuv+float2(ScreenOffset*-d45, ScreenOffset*d45), smuv+float2(0.f, ScreenOffset*rad), smuv+float2(ScreenOffset*d45, ScreenOffset*d45),
        smuv+float2(ScreenOffset*-rad, 0.f), smuv, smuv+float2(ScreenOffset*rad, 0.f),
        smuv+float2(ScreenOffset*-d45, ScreenOffset*-d45), smuv+float2(0.f, ScreenOffset*-rad), smuv+float2(ScreenOffset*d45, ScreenOffset*-d45),
    };
    float blockerSum = 0.f;
    float count = 0.f;
    [unroll]
    for(int i=0;i<9;i++)
    {
        float blocker = shadowMap.Sample(g_sampler,p[i]);
        float isShadow = 1.f - step(lvpDepth,blocker+shadowMap_offset[ind]);
        count += isShadow;
        blockerSum += isShadow * blocker;
    }

    if(!isInUV || count == 0)
        return 0.f;

    float blocker = blockerSum / count;


    //return PCF(worldPos, uv, 10./3., shadowMap, ScreenOffset, ind);
    return PCF(worldPos, smuv, ((lvpDepth-blocker)/blocker)*200.f, shadowMap, ScreenOffset, ind);

    return 0.f;
}

float ShadowRecieve(float3 worldPos, float2 uv,float depth)
{
    float viewZ = GetViewZ(depth);
    //int ind = GetShadowMapIndex(abs(viewZ));
    int ind = GetShadowMapIndex(worldPos);
    float rad = 10.f;
    if(ind == 0)
    {
        return PCSS(worldPos, uv, rad, g_shadowMap_1, shadowMap_pixSize[0].x, ind);
    }
    else if(ind == 1)
    {
        //return PCF(worldPos, uv, 0.2f, g_shadowMap_2, shadowMap_pixSize[1].x, ind);
        return PCSS(worldPos, uv, rad*0.1666667, g_shadowMap_2, shadowMap_pixSize[1].x*0.1666667, ind);
    }
    else if(ind == 2)
    {
        //return PCF(worldPos, uv, 0.2f, g_shadowMap_3, shadowMap_pixSize[2].x, ind);
        return PCSS(worldPos, uv, rad*0.1666667*0.2, g_shadowMap_3, shadowMap_pixSize[2].x*0.1666667*0.2, ind);
    }
    //return FetchShadow(GetWorldPosition(uv,g_GDepth.Sample(g_sampler, uv),def_mvpi));
    return 0.f;
}


float ViewMap_Debug(int ind,float3 worldPos)
{
    float4 lvpPos = mul(shadowMap_mLVP[ind],float4(worldPos,1.f));
    lvpPos.xyz *= rcp(lvpPos.w);
    float lvpDepth = lvpPos.z;

    float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

    bool isInUV = (step(smuv.x,1.f)) * (step(smuv.y,1.f)) * (step(0.f,smuv.x)) * (step(0.f,smuv.y));
    return isInUV;
}
float3 ShadowRecieve_Debug(float3 worldPos, float2 uv,float depth)
{
    float viewZ = GetViewZ(depth);
    //int ind = GetShadowMapIndex(abs(viewZ));
    int ind = GetShadowMapIndex(worldPos);
    if(ind == 0)
    {
        ind = 0;
        float s = PCSS(worldPos, uv, 5.f, g_shadowMap_1, shadowMap_pixSize[0].x, ind);
        return float3(1.f,0.5,0.5)*ViewMap_Debug(ind,worldPos) * 1.-s;
    }
    else if(ind == 1)
    {
        float s = PCSS(worldPos, uv, 5.f*0.1666667, g_shadowMap_2, shadowMap_pixSize[0].x*0.2, ind);
        return float3(0.5,1.0,0.5)*ViewMap_Debug(ind,worldPos) * 1.-s;
    }
    else if(ind == 2)
    {
        float s = PCSS(worldPos, uv, 5.f*0.1666667*0.2, g_shadowMap_3, shadowMap_pixSize[0].x*0.2*0.5, ind);
        return float3(0.5,0.5,1.0)*ViewMap_Debug(ind,worldPos) * 1.-s;
    }
    return 0.f;
}
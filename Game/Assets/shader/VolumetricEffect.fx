#include "Math.fxh"
#include "G_Sampler.fxh"
#include "PerlinNoise.fxh"
#include "WorleyNoise.fxh"
#include "FBM.fxh"


#define SHADOWMAP_DATA_REGISTER 1
#define SHADOWMAP_1 4
#define SHADOWMAP_2 5
#define SHADOWMAP_3 6
#include "ShadowMapData.fxh"
int GetShadowMapIndex(float3 worldPos,out float2 uv)
{
    uv = 0.f;
    int isInUV = 0.f;
    int num = -1;
    [unroll(3)]
    for(int i=1;i<=3;i++)
    {
        float4 lvpPos = mul(shadowMap_mLVP[i-1],float4(worldPos,1.f));
        lvpPos.xyz *= rcp(lvpPos.w);
        float lvpDepth = lvpPos.z;

        float2 smuv = mad(float2(0.5f, -0.5f), lvpPos.xy, float2(0.5f, 0.5f));

        int notIsInUV = 1-isInUV;
        int isIn = (step(smuv.x,0.95f)) * (step(smuv.y,0.95f)) * (step(0.05f,smuv.x)) * (step(0.05f,smuv.y));
        //isInUV +=  * (1-isInUV);
        num += i * notIsInUV * isIn;
        isInUV += notIsInUV * isIn;
        uv += smuv * (notIsInUV * isIn);
        // if(isInUV)
        // {
        //     uv = smuv.xy;
        //     return i;
        // }
    }
    return num;
}

#define GETSHADOW(worldPos,res) { \
    float2 uv = 0; \
    int ind = GetShadowMapIndex(worldPos,uv); \
\
    res = 0; \
    res = g_shadowMap_1.Sample(g_sampler,uv); \
} \

float GetShadow(float worldPos)
{
    float2 uv = 0;
    int ind = GetShadowMapIndex(worldPos,uv);

    float res=0;
    
    res = g_shadowMap_1.Sample(g_sampler,uv);// * float(ind == 0);
    res += g_shadowMap_2.Sample(g_sampler,uv);// * float(ind == 1);
    res += g_shadowMap_3.Sample(g_sampler,uv);// * float(ind == 2);
    
    return res;
    // [branch]
    // switch(ind)
    // {
    //     case 0:
    //     return g_shadowMap_1.Sample(g_sampler,uv);
    //     break;
    //     case 1:
    //     return g_shadowMap_2.Sample(g_sampler,uv);
    //     break;
    //     case 2:
    //     return g_shadowMap_3.Sample(g_sampler,uv);
    //     break;
    // }
    // return 0.f;
}

struct FogData
{
    uint rayCount;          //レイのステップ回数。
    float rayLen;           //ステップの距離。

    float decayCenterToXZ;        //中心からの減衰。
    float decayCenterToY;
    float centerOffsetY;
    float3 centerPos;
    //int p1;

    float3 centerBottom;
    int p2;

    float3 centerTop;
    int p2_1;
    

    float3 color;           //フォグの色。
    //int p3;
    float ratioParlin;    //パーリンノイズ割合。
    float concentration;    //濃度。
    float perlinScale;      //パーリンノイズのスケール
    float worleyScale;      //ウォーリーノイズのスケール
    float ratioParlinWorley;    //パーリンノイズとウォーリーノイズの合成の割合(0がPerlin、1がWorley)
    

    float3 offset;              //位置のオフセット
    int p4;

};

cbuffer SConstBufferData:register(b0)
{
    float4x4 cb_mvp;
    float4x4 cb_mvp_inv;

    FogData cb_data;
}

struct VSInput
{
    float4 pos  :POSITION;
    float2 uv   :TEXCOORD;
};

struct PSInput
{
    float4 pos  :SV_POSITION;
    float2 uv   :TEXCOORD;

};


PSInput VSMain(VSInput In)
{
    PSInput Out;

    Out.pos = In.pos;
    Out.uv = In.uv;

    return Out;
}

Texture2D<float> g_anchor : register(t0);   	//
Texture2D<float> g_anchor_inv : register(t1);	//
Texture2D<uint> g_stencil : register(t2);	    //
Texture2D<float> g_GDepth : register(t3);	    //



float4 PSMain(PSInput In):SV_TARGET0
{
    float frontDepth = g_anchor.Sample(g_sampler,In.uv).x;
    
    float gDepth = g_GDepth.Sample(g_sampler,In.uv).x;
    uint2 iuv = In.uv * uint2(1280,720);
    float stencil = g_stencil[iuv];
    
    clip(min(gDepth-frontDepth,stencil-0.1f));

    float backDepth = g_anchor_inv.Sample(g_sampler,In.uv).x;

    float3 pos = GetWorldPosition(In.uv,frontDepth,cb_mvp_inv);
    float3 backPos = GetWorldPosition(In.uv,backDepth,cb_mvp_inv);
    //float3 gPos = GetWorldPosition(In.uv,gDepth,cb_mvp_inv);
    float3 dir = normalize(backPos - pos);

    float3 b2t = cb_data.centerTop - cb_data.centerBottom;
    float b2tLen = sqrt(dot(b2t,b2t));
    float3 b2tNor = b2t * rcp(b2tLen);

    float vol = 0.f;

    //float3 bottomH = b2tNor*cb_data.centerBottom;
    float bottomD = dot(b2tNor,cb_data.centerBottom);
    float3 bottomABC = b2tNor;

    float topD = dot(-b2tNor,cb_data.centerTop);
    float3 topABC = -b2tNor;

    float shadow = 0;

    
    //for(int i=0;i<ray_count;i++)
    //[unroll(40)]
    for(int i=0;i<cb_data.rayCount;i++)
    {
        float4 vpPos = mul(cb_mvp,float4(pos,1.f));
        float vpdep = vpPos.z * rcp(vpPos.w);
        [branch]
        if(backDepth < vpdep || gDepth < vpdep)
        {
            break;
        }

        float wn = (1.f-WorleyNoise3D(mad(pos, cb_data.worleyScale, cb_data.offset)));
        //float pn = PerlinNoise3D(mad(pos, cb_data.perlinScale, cb_data.offset));
        float pn = FBM3D_FBMx2(pos*cb_data.perlinScale,cb_data.offset,0.8f);
        
        //float addvol = lerp(wn,pn,cb_data.ratioParlinWorley)*cb_data.concentration;
        float addvol = saturate(pn * cb_data.ratioParlin + wn * cb_data.ratioParlinWorley)*cb_data.concentration;

        
        {
            float shortLen;
            float3 vp = pos - cb_data.centerBottom;
            //float3 shortest = b2t * dot(vp,b2tNor) / b2tLen - vp;
            //float3 shortest = mad(dot(vp,b2tNor) * rcp(b2tLen), b2t, -vp);
            float3 shortest = mad(dot(vp,b2tNor), b2tNor, -vp);
            shortLen = length(shortest);

            addvol = mad(-addvol * shortLen, cb_data.decayCenterToXZ, addvol);

            float3 cp = pos - (cb_data.centerPos + b2tNor * cb_data.centerOffsetY);
            //cp += b2tNor * cb_data.centerOffsetY;
            //cp = normalize(cp);
            //addvol = mad(-addvol * abs(dot(b2tNor,cp)), cb_data.decayCenterToY, addvol);


            float blen = abs(dot(bottomABC,cp)+bottomD);//*rcp(length(bottomABC));
            float tlen = abs(dot(topABC,cp)+topD);//*rcp(length(topABC));

            float delen = 100.f;
            float t = min(blen,tlen);
            t = min(delen,t);
            addvol = mad(-addvol * (t*rcp(delen)), cb_data.decayCenterToY, addvol);
        }
        //shadow += GetShadow(pos);
        // float s;
        // GETSHADOW(pos,s)
        // shadow += s;

        vol += addvol;
        pos = mad(dir, cb_data.rayLen, pos);
    }
    shadow *= rcp(cb_data.rayCount);
    vol = saturate(vol);
    float4 res = float4(cb_data.color * (1.f-shadow),vol);
    
    return res;
}
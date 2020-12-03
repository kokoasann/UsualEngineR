#include "Math.fxh"
#include "PerlinNoise.fxh"
#include "WorleyNoise.fxh"
#include "FBM.fxh"

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

sampler Sampler : register(s0);

float4 PSMain(PSInput In):SV_TARGET0
{
    float frontDepth = g_anchor.Sample(Sampler,In.uv).x;
    
    float gDepth = g_GDepth.Sample(Sampler,In.uv).x;
    uint2 iuv = In.uv * uint2(1280,720);
    float stencil = g_stencil[iuv];
    
    clip(min(gDepth-frontDepth,stencil-0.1f));

    float backDepth = g_anchor_inv.Sample(Sampler,In.uv).x;

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

    //[unroll(ray_count)]
    //for(int i=0;i<ray_count;i++)
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
        

        vol += addvol;
        pos = mad(dir, cb_data.rayLen, pos);
    }
    vol = saturate(vol);
    float4 res = float4(cb_data.color,vol);
    
    return res;
}